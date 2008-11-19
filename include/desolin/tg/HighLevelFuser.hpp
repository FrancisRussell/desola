#ifndef DESOLIN_TG_HIGH_LEVEL_FUSER_HPP
#define DESOLIN_TG_HIGH_LEVEL_FUSER_HPP

#include "Desolin_tg_fwd.hpp"
#include <set>
#include <map>
#include <vector>
#include <algorithm>
#include <iterator>
#include <boost/foreach.hpp>

// Note that a number of vectors are used here when it seems like sets might be
// more appropriate. This is because we require deterministic behaviour in order
// to do recipe caching correctly. The alternative is to use an algorithm that
// can do proper graph matching.

namespace desolin
{

namespace detail
{

template<typename T_element>
class MatrixVectorMultiplyFinder : public TGExpressionNodeVisitor<T_element>
{
private:
  std::vector<TGMatrixVectorMult<T_element>*> multiplies;

public:
  std::vector<TGMatrixVectorMult<T_element>*> getMultiplies() const
  {
    return multiplies;
  }

  void visit(TGElementGet<tg_vector, T_element>& e) {}
  void visit(TGElementGet<tg_matrix, T_element>& e) {}

  void visit(TGElementSet<tg_vector, T_element>& e) {}
  void visit(TGElementSet<tg_matrix, T_element>& e) {}

  void visit(TGLiteral<tg_scalar, T_element>& e) {}
  void visit(TGLiteral<tg_vector, T_element>& e) {}
  void visit(TGLiteral<tg_matrix, T_element>& e) {}

  void visit(TGMatrixMult<T_element>& e) {}

  void visit(TGMatrixVectorMult<T_element>& e)
  {
    multiplies.push_back(&e);
  }

  // We don't record these, we assume this pass is only run once
  void visit(TGMatrixMultiVectorMult<T_element>& e) {}

  void visit(TGVectorDot<T_element>& e) {}
  void visit(TGVectorCross<T_element>& e) {}
  void visit(TGVectorTwoNorm<T_element>& e) {}
  void visit(TGMatrixTranspose<T_element>& e) {}

  void visit(TGPairwise<tg_scalar, T_element>& e) {}
  void visit(TGPairwise<tg_vector, T_element>& e) {}
  void visit(TGPairwise<tg_matrix, T_element>& e) {}

  void visit(TGScalarPiecewise<tg_scalar, T_element>& e) {}
  void visit(TGScalarPiecewise<tg_vector, T_element>& e) {}
  void visit(TGScalarPiecewise<tg_matrix, T_element>& e) {}

  void visit(TGNegate<tg_scalar, T_element>& e) {}
  void visit(TGNegate<tg_vector, T_element>& e) {}
  void visit(TGNegate<tg_matrix, T_element>& e) {}

  void visit(TGAbsolute<T_element>& e) {}
  void visit(TGSquareRoot<T_element>& e) {}
};

template<typename T_element>
class HighLevelFuser
{
private:
  TGExpressionGraph<T_element>& graph;

  // Determines if two sorted ranges are disjoint
  template<typename InputIterator1, typename InputIterator2>
  bool sets_disjoint(InputIterator1 first1, InputIterator1 last1,
                     InputIterator2 first2, InputIterator2 last2)
  {
    while(first1 != last1 && first2 != last2)
    {
      if (*first1 < *first2)
        ++first1;
      else if (*first2 < *first1)
        ++first2;
      else
        return false;
    }

    return true;
  }

  static std::set<TGExpressionNode<T_element>*> getDependencies(TGExpressionNode<T_element>* const node)
  {
    std::set<TGExpressionNode<T_element>*> dependencies;
    getDependenciesHelper(dependencies, node);
    return dependencies;
  }

  static void getDependenciesHelper(std::set<TGExpressionNode<T_element>*>& dependencies,
    TGExpressionNode<T_element>* const node)
  {
    BOOST_FOREACH(TGExpressionNode<T_element>* dependency, node->getDependencies())
    {
      if (dependencies.insert(dependency).second)
        getDependenciesHelper(dependencies, dependency);
    }
  }

  void attemptFusion(const TGOutputReference<tg_matrix, T_element>& matrix, const std::vector<TGMatrixVectorMult<T_element>*>& matVecMuls)
  {
    std::set<TGExpressionNode<T_element>*> combinedDependencies;

    BOOST_FOREACH(TGMatrixVectorMult<T_element>* matVecMul, matVecMuls)
    {
      const std::set<TGExpressionNode<T_element>*> dependencies(getDependencies(matVecMul));
      combinedDependencies.insert(dependencies.begin(), dependencies.end());
    }

    // If the combined set of dependencies of the matrix-vector multiplies do not contain any of the other
    // matrix-vector multiplies, we consider it safe to fuse.
    
    const std::set<TGExpressionNode<T_element>*> matVecMulSet(matVecMuls.begin(), matVecMuls.end());

    if (sets_disjoint(matVecMulSet.begin(), matVecMulSet.end(), combinedDependencies.begin(), combinedDependencies.end()))
    {
      std::vector<typename TGMatrixMultiVectorMult<T_element>::multiply_params> multiNodeParams;

      BOOST_FOREACH(TGMatrixVectorMult<T_element>* matVecMul, matVecMuls)
      {
        // Note that releaseInternal() is called to avoid the TGMatrixMultiVectorMult object deleting the
        // internal representation on destruction.
        typename TGMatrixMultiVectorMult<T_element>::multiply_params matVecParams(matVecMul->getRight(), 
          matVecMul->releaseInternal(), matVecMul->isTranspose());
        multiNodeParams.push_back(matVecParams);
      }

      TGMatrixMultiVectorMult<T_element>* const multiMatVecMul = new TGMatrixMultiVectorMult<T_element>(matrix, multiNodeParams);

      // Replace references to matrix-vector multiply nodes with new combined node
      for(std::size_t index = 0; index < matVecMuls.size(); ++index)
      {
        const TGOutputReference<tg_vector, T_element> from(matVecMuls[index], 0);
        TGOutputReference<tg_vector, T_element> to(multiMatVecMul, index);
        graph.replaceDependency(from, to);
      }

      graph.removeNodes(matVecMulSet);
      graph.add(multiMatVecMul);
      graph.sort();
    }
  }
 
public:
  HighLevelFuser(TGExpressionGraph<T_element>& g) : graph(g)
  {
  }

  void execute()
  {
    MatrixVectorMultiplyFinder<T_element> finder;
    graph.accept(finder);

    typedef std::map<TGOutputReference<tg_matrix, T_element>, std::vector<TGMatrixVectorMult<T_element>*> > PotentialFusionMap;
    PotentialFusionMap potentialFusions;

    const std::vector<TGMatrixVectorMult<T_element>*> multiplies(finder.getMultiplies());
    BOOST_FOREACH(TGMatrixVectorMult<T_element>* multiply, multiplies)
    {
      potentialFusions[multiply->getLeft()].push_back(multiply);
    }

    BOOST_FOREACH(typename PotentialFusionMap::value_type& potentialFusion, potentialFusions)
    {
      if (potentialFusion.second.size() > 1) 
        attemptFusion(potentialFusion.first, potentialFusion.second);
    }
  }
};

}

}

#endif
