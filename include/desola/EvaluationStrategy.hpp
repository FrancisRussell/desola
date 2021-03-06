/****************************************************************************/
/* Copyright 2005-2006, Francis Russell                                     */
/*                                                                          */
/* Licensed under the Apache License, Version 2.0 (the License);            */
/* you may not use this file except in compliance with the License.         */
/* You may obtain a copy of the License at                                  */
/*                                                                          */
/*     http://www.apache.org/licenses/LICENSE-2.0                           */
/*                                                                          */
/* Unless required by applicable law or agreed to in writing, software      */
/* distributed under the License is distributed on an AS IS BASIS,          */
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. */
/* See the License for the specific language governing permissions and      */
/* limitations under the License.                                           */
/*                                                                          */
/****************************************************************************/

#ifndef DESOLA_EVALUATION_STRATEGY_HPP
#define DESOLA_EVALUATION_STRATEGY_HPP

#include <utility>
#include <iterator>
#include <map>
#include <set>
#include <vector>
#include <algorithm>
#include <boost/bind.hpp>
#include <cassert>
#include <boost/shared_ptr.hpp>
#include <desola/Desola_fwd.hpp>

namespace desola
{

namespace detail
{

template<typename T_element>
class LiteralReplacer : public ExpressionNodeTypeVisitor<T_element>
{
private:
  LiteralReplacer(const LiteralReplacer&);
  LiteralReplacer& operator=(const LiteralReplacer&);
  
  const std::map< ExprNode<scalar, T_element>*, Literal<scalar, T_element>* >& scalarMap;
  const std::map< ExprNode<vector, T_element>*, Literal<vector, T_element>* >& vectorMap;
  const std::map< ExprNode<matrix, T_element>*, Literal<matrix, T_element>* >& matrixMap;
	
public:
  LiteralReplacer( const std::map< ExprNode<scalar, T_element>*, Literal<scalar, T_element>* >& sMap,
		   const std::map< ExprNode<vector, T_element>*, Literal<vector, T_element>* >& vMap,
		   const std::map< ExprNode<matrix, T_element>*, Literal<matrix, T_element>* >& mMap) : 
	           scalarMap(sMap), vectorMap(vMap), matrixMap(mMap)
  {
  }
  
  void visit(ExprNode<scalar, T_element>& expr)
  {
    const typename std::map<ExprNode<scalar, T_element>*, Literal<scalar, T_element>*>::const_iterator i = scalarMap.find(&expr);
    if (i != scalarMap.end())
      i->first->replace(*(i->second));
  }

  void visit(ExprNode<vector, T_element>& expr)
  {
    const typename std::map<ExprNode<vector, T_element>*, Literal<vector, T_element>*>::const_iterator i = vectorMap.find(&expr);
    if (i != vectorMap.end())
      i->first->replace(*(i->second));
  }

  void visit(ExprNode<matrix, T_element>& expr)
  {
    const typename std::map<ExprNode<matrix, T_element>*, Literal<matrix, T_element>*>::const_iterator i = matrixMap.find(&expr);
    if (i != matrixMap.end())
      i->first->replace(*(i->second));
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
  
  template<typename exprType>
  static void freeLiteral(const std::pair<const ExprNode<exprType, T_element>*, Literal<exprType, T_element>*>& pair)
  {
    pair.second->deleteIfUnused();
  }
	
  template<typename exprType>
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

  template<typename exprType>
  static void allocateLiteral(const std::pair<const ExprNode<exprType, T_element>*, Literal<exprType, T_element>*>& pair)
  {
    pair.second->getValue().allocate();
  }

  template<typename exprType>
  static void allocateLiteralsHelper(const std::map<ExprNode<exprType, T_element>*, Literal<exprType, T_element>*> map)
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
  EvaluationStrategy(ExpressionGraph<T_element>& graph) : hasEvaluated(false), expressionGraph(graph), 
    sortedUnclaimed(graph.sortedNodesBegin(), graph.sortedNodesEnd())
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
      claimedMap[evaluator.get()] = claimed;
      evaluators.push_back(evaluator);
      
      // Removes all the claimed nodes from the topologically sorted list of unclaimed nodes
      std::vector<ExpressionNode<T_element>*> newSortedUnclaimed;
      std::back_insert_iterator< std::vector<ExpressionNode<T_element>*> > out(newSortedUnclaimed);
      std::remove_copy_if(sortedUnclaimed.begin(), sortedUnclaimed.end(), out, boost::bind(&std::set<ExpressionNode<T_element>*>::count, &claimed, _1));
      sortedUnclaimed.swap(newSortedUnclaimed);

      // This should come last, so the generatedEvaluatedNodes call can see all the changes already made and nodes claimed
      evaluator->generateEvaluatedNodes();	    
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
        (*evaluatorIterator)->evaluate();

      expressionGraph.accept(replacer);
    }
  }
 
  bool mustEvaluate(Evaluator<T_element>& evaluator, ExpressionNode<T_element>& node) const
  {
    assert(claimedMap.find(&evaluator) != claimedMap.end());

    // It's an output if it has nodes depending on it that are not evaluated by the evaluator
    const std::vector<ExpressionNode<T_element>*> internal_reqBy(node.getInternalRequiredBy());
    const std::set<ExpressionNode<T_element>*>& claimed = claimedMap.find(&evaluator)->second;
       
    for(typename std::vector<ExpressionNode<T_element>*>::const_iterator iterator=internal_reqBy.begin(); iterator!=internal_reqBy.end(); ++iterator)
    {
      if (claimed.find(*iterator) == claimed.end())
        return true;
    }
	
    return false;
  }

  // Given a pointer to a Literal, these methods return whether or not the literal contains actual data. At
  // the moment, we can just check to see if they're a key in the replacement map as the original literals in
  // the ExpressionGraph are mapped to themselves.
  bool hasData(Literal<scalar, T_element>* const l) const
  {
    return scalarMap.find(l) != scalarMap.end();
  }

  bool hasData(Literal<vector, T_element>* const l) const
  {
    return vectorMap.find(l) != vectorMap.end();
  }

  bool hasData(Literal<matrix, T_element>* const l) const
  {
    return matrixMap.find(l) != matrixMap.end();
  }

  inline ExpressionGraph<T_element>& getExpressionGraph()
  {
    return expressionGraph;
  }

  void addEvaluatedExprMapping(ExprNode<scalar, T_element>* const e, Literal<scalar, T_element>* const l)
  {
    assert(e != NULL);
    assert(l != NULL);
    scalarMap[e] = l;
  }

  void addEvaluatedExprMapping(ExprNode<vector, T_element>* const e, Literal<vector, T_element>* const l)
  {
    assert(e != NULL);
    assert(l != NULL);
    vectorMap[e] = l;
  }

  void addEvaluatedExprMapping(ExprNode<matrix, T_element>* const e, Literal<matrix, T_element>* const l)
  {
    assert(e != NULL);
    assert(l != NULL);
    matrixMap[e] = l;
  }

  Literal<scalar, T_element>* getEvaluatedExpr(ExprNode<scalar, T_element>* const e) 
  {
    assert(e != NULL);
    Literal<scalar, T_element>* const evaluated = scalarMap[e];
    assert(evaluated != NULL);
    return evaluated;
  }

  Literal<vector, T_element>* getEvaluatedExpr(ExprNode<vector, T_element>* const e)
  {
    assert(e != NULL);
    Literal<vector, T_element>* const evaluated = vectorMap[e];
    assert(evaluated != NULL);
    return evaluated;
  }

  Literal<matrix, T_element>* getEvaluatedExpr(ExprNode<matrix, T_element>* const e)
  {
    assert(e != NULL);
    Literal<matrix, T_element>* const evaluated = matrixMap[e];
    assert(evaluated != NULL);
    return evaluated;
  }

  ~EvaluationStrategy()
  {
    if (!hasEvaluated)
      freeLiterals();
  }
};

}

}
#endif
