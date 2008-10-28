#ifndef DESOLIN_TG_HIGH_LEVEL_FUSER_HPP
#define DESOLIN_TG_HIGH_LEVEL_FUSER_HPP

#include "Desolin_tg_fwd.hpp"
#include <utility>
#include <set>
#include <map>
#include <vector>
#include <algorithm>
#include <iterator>
#include <boost/foreach.hpp>

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

  void attemptFusion(const std::set<TGMatrixVectorMult<T_element>*>& matVecMuls)
  {
    std::set<TGExpressionNode<T_element>*> combinedDependencies;

    BOOST_FOREACH(TGMatrixVectorMult<T_element>* matVecMul, matVecMuls)
    {
      const std::set<TGExpressionNode<T_element>*> dependencies(getDependencies(matVecMul));
      combinedDependencies.insert(dependencies.begin(), dependencies.end());
    }

    // If the combined set of dependencies of the matrix-vector multiplies do not contain any of the other
    // matrix-vector multiplies, we consider it safe to fuse.
    
    std::set<TGMatrixVectorMult<T_element>*> intersection;
    std::set_intersection(matVecMuls.begin(), matVecMuls.end(), combinedDependencies.begin(), combinedDependencies.end(), 
      std::inserter(intersection, intersection.begin()));

    if (intersection.empty())
    {
      std::cout << "High-level fusion is possible!" << std::endl;
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
    const std::vector<TGMatrixVectorMult<T_element>*> multiplies(finder.getMultiplies());

    typedef std::map<TGOutputReference<tg_matrix, T_element>, std::set<TGMatrixVectorMult<T_element>*> > PotentialFusionMap;
    PotentialFusionMap potentialFusions;

    BOOST_FOREACH(TGMatrixVectorMult<T_element>* multiply, multiplies)
    {
      potentialFusions[multiply->getLeft()].insert(multiply);
    }

    BOOST_FOREACH(typename PotentialFusionMap::value_type& potentialFusion, potentialFusions)
    {
      if (potentialFusion.second.size() > 1) 
        attemptFusion(potentialFusion.second);
    }
  }
};

}

}

#endif
