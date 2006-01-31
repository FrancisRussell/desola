#ifndef DESOLIN_EVALUATION_STRATEGY_HPP
#define DESOLIN_EVALUATION_STRATEGY_HPP

#include <map>
#include <set>
#include <vector>
#include <algorithm>
#include <boost/bind.hpp>
#include <cassert>
#include <boost/shared_ptr.hpp>
#include <desolin/Desolin_fwd.hpp>

template<typename T_element>
class LiteralReplacer : public ExpressionNodeTypeVisitor<T_element>
{
private:
  LiteralReplacer(const LiteralReplacer&);
  LiteralReplacer& operator=(const LiteralReplacer&);
  
  std::map< ExprNode<scalar, T_element>*, Literal<scalar, T_element>* >& scalarMap;
  std::map< ExprNode<vector, T_element>*, Literal<vector, T_element>* >& vectorMap;
  std::map< ExprNode<matrix, T_element>*, Literal<matrix, T_element>* >& matrixMap;
	
public:
  LiteralReplacer( std::map< ExprNode<scalar, T_element>*, Literal<scalar, T_element>* >& sMap,
		   std::map< ExprNode<vector, T_element>*, Literal<vector, T_element>* >& vMap,
		   std::map< ExprNode<matrix, T_element>*, Literal<matrix, T_element>* >& mMap) : 
	           scalarMap(sMap), vectorMap(vMap), matrixMap(mMap)
  {
  }
  
  void visit(ExprNode<scalar, T_element>& expr)
  {
    const typename std::map<ExprNode<scalar, T_element>*, Literal<scalar, T_element>*>::iterator i = scalarMap.find(&expr);
    if (i != scalarMap.end())
    {
      i->first->replace(*(i->second));
    }
  }

  void visit(ExprNode<vector, T_element>& expr)
  {
    const typename std::map<ExprNode<vector, T_element>*, Literal<vector, T_element>*>::iterator i = vectorMap.find(&expr);
    if (i != vectorMap.end())
    {
      i->first->replace(*(i->second));
    }
  }

  void visit(ExprNode<matrix, T_element>& expr)
  {
    const typename std::map<ExprNode<matrix, T_element>*, Literal<matrix, T_element>*>::iterator i = matrixMap.find(&expr);
    if (i != matrixMap.end())
    {
      i->first->replace(*(i->second));
    }	
  }      
};

template<typename T_element>
class EvaluationStrategy
{
private:
  EvaluationStrategy(const EvaluationStrategy&);
  EvaluationStrategy& operator=(const EvaluationStrategy&);
	
  bool hasEvaluated;
  ExpressionGraph<T_element>& expressionGraph;

  std::vector< ExpressionNode<T_element>* > sortedUnclaimed;
  std::vector< boost::shared_ptr< Evaluator<T_element> > > evaluators;
  std::map< Evaluator<T_element>*, std::set<ExpressionNode<T_element>*> > claimedMap;
	  
  std::map< ExprNode<scalar, T_element>*, Literal<scalar, T_element>* > scalarMap;
  std::map< ExprNode<vector, T_element>*, Literal<vector, T_element>* > vectorMap;
  std::map< ExprNode<matrix, T_element>*, Literal<matrix, T_element>* > matrixMap;
  
  template<ExprType exprType>
  static void freeLiteral(const std::pair<ExprNode<exprType, T_element>*, Literal<exprType, T_element>*>& pair)
  {
    pair.second->deleteIfUnused();
  }
	
  template<ExprType exprType>
  inline static void freeLiteralsHelper(const std::map<ExprNode<exprType, T_element>*, Literal<exprType, T_element>*> map)
  {
    std::for_each(map.begin(), map.end(), boost::bind(freeLiteral<exprType>, _1));
  }
  
  void freeLiterals()
  {
    freeLiteralsHelper(scalarMap);
    freeLiteralsHelper(vectorMap);
    freeLiteralsHelper(matrixMap);
  }

  template<ExprType exprType>
  static void allocateLiteral(const std::pair<ExprNode<exprType, T_element>*, Literal<exprType, T_element>*>& pair)
  {
    pair.second->getValue()->allocate();
  }

  template<ExprType exprType>
  inline static void allocateLiteralsHelper(const std::map<ExprNode<exprType, T_element>*, Literal<exprType, T_element>*> map)
  {
    std::for_each(map.begin(), map.end(), boost::bind(allocateLiteral<exprType>, _1));
  }
			  
  void allocateLiterals()
  {
    allocateLiteralsHelper(scalarMap);
    allocateLiteralsHelper(vectorMap);
    allocateLiteralsHelper(matrixMap);
  }

public:
  EvaluationStrategy(ExpressionGraph<T_element>& graph) : hasEvaluated(false), expressionGraph(graph), sortedUnclaimed(graph.getSortedNodes())
  {
    NullEvaluatorFactory<T_element> nullEvaluatorFactory;
    addEvaluator(nullEvaluatorFactory);
  }

  void addEvaluator(EvaluatorFactory<T_element>& factory)
  {
    boost::shared_ptr< Evaluator<T_element> > evaluator(factory.createEvaluator(*this));
    const std::set<ExpressionNode<T_element>*> claimed(evaluator->claimNodes(sortedUnclaimed));
   
    if(!claimed.empty())
    {
      evaluator->generateEvaluatedNodes();
      evaluators.push_back(evaluator);
      claimedMap[evaluator.get()] = claimed;

      // Removes all the claimed nodes from the topologically sorted list of unclaimed nodes
      std::vector<ExpressionNode<T_element>*> newSortedUnclaimed;
      std::back_insert_iterator< std::vector<ExpressionNode<T_element>*> > out(newSortedUnclaimed);
      std::remove_copy_if(sortedUnclaimed.begin(), sortedUnclaimed.end(), out, boost::bind(&std::set<ExpressionNode<T_element>*>::count, &claimed, _1));
      sortedUnclaimed = newSortedUnclaimed;
    }
  }
  
  void execute()
  {
    assert(sortedUnclaimed.empty());
    
    if (!hasEvaluated)
    {
      hasEvaluated=true;

      allocateLiterals();
      LiteralReplacer<T_element> replacer(scalarMap, vectorMap, matrixMap);
      for(typename std::vector< boost::shared_ptr< Evaluator<T_element> > >::iterator evaluatorIterator = evaluators.begin(); evaluatorIterator != evaluators.end(); ++evaluatorIterator)
      {
        (*evaluatorIterator)->evaluate();
	const std::set< ExpressionNode<T_element>* > evaluatedNodes(claimedMap[evaluatorIterator->get()]);
	for(typename std::set<ExpressionNode<T_element>*>::iterator evaluatedIterator = evaluatedNodes.begin(); evaluatedIterator != evaluatedNodes.end(); ++evaluatedIterator)
	{
          (*evaluatedIterator)->accept(replacer);
        }
      }
    }
  }

  bool isExternal(Evaluator<T_element>& evaluator, ExpressionNode<T_element>& node) const
  {
    // It's an output if it has variables depending on it
    if (!node.getExternalRequiredBy().empty())
      return true;

    // It's an output if it has nodes depending on it that are node evaluated by the evaluator
    const std::multiset<ExpressionNode<T_element>*> internal_reqBy(node.getInternalRequiredBy());
    const std::set<ExpressionNode<T_element>*>& claimed = claimedMap.find(&evaluator)->second;
    for(typename std::multiset<ExpressionNode<T_element>*>::const_iterator iterator=internal_reqBy.begin(); iterator!=internal_reqBy.end(); ++iterator)
    {
      if (claimed.find(*iterator) == claimed.end())
        return true;
    }
    return false;
  }

  inline ExpressionGraph<T_element>& getExpressionGraph()
  {
    return expressionGraph;
  }

  inline void addEvaluatedExprMapping(ExprNode<scalar, T_element>* e, Literal<scalar, T_element>* l)
  {
    assert(e != NULL);
    assert(l != NULL);
    scalarMap[e] = l;
  }

  inline void addEvaluatedExprMapping(ExprNode<vector, T_element>* e, Literal<vector, T_element>* l)
  {
    assert(e != NULL);
    assert(l != NULL);
    vectorMap[e] = l;
  }

  inline void addEvaluatedExprMapping(ExprNode<matrix, T_element>* e, Literal<matrix, T_element>* l)
  {
    assert(e != NULL);
    assert(l != NULL);
    matrixMap[e] = l;
  }

  inline Literal<scalar, T_element>* getEvaluatedExpr(ExprNode<scalar, T_element>* e) 
  {
    assert(e != NULL);
    assert(scalarMap[e] != NULL);
    return scalarMap[e];
  }

  inline Literal<vector, T_element>* getEvaluatedExpr(ExprNode<vector, T_element>* e)
  {
    assert(e != NULL);
    assert(vectorMap[e] != NULL);
    return vectorMap[e];
  }

  inline Literal<matrix, T_element>* getEvaluatedExpr(ExprNode<matrix, T_element>* e)
  {
    assert(e != NULL);
    assert(matrixMap[e] != NULL);
    return matrixMap[e];
  }

  ~EvaluationStrategy()
  {
    if (!hasEvaluated)
    {
      freeLiterals();
    }
  }
};

#endif
