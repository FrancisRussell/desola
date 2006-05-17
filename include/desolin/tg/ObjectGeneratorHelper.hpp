#ifndef DESOLIN_TG_OBJECT_GENERATOR_HELPER_HPP
#define DESOLIN_TG_OBJECT_GENERATOR_HELPER_HPP

#include <boost/shared_ptr.hpp>
#include <map>
#include <set>
#include <TaskGraph>
#include <desolin/tg/Desolin_tg_fwd.hpp>

namespace desolin_internal
{

template<ExprType exprType, typename T_element>
class TGObjectGeneratorHelper
{
private:
  TGObjectGeneratorHelper(const TGObjectGeneratorHelper&);
  TGObjectGeneratorHelper& operator=(const TGObjectGeneratorHelper&);
   
  static const TGExprType tgExprType = ExprTGTraits<exprType, T_element>::tgExprType;
  std::map<ExprNode<exprType, T_element>*, TGExprNode<tgExprType, T_element>*> nodeMap;
  TGEvaluator<T_element>& evaluator;
	 
  // We use these getters instead of storing references to the graph and
  // strategy because at the time EvaluationHandler is constructed, the
  // TGEvaluator is not yet fully constructed.
   
  inline TGExpressionGraph<T_element>& getGraph()
  {
    return evaluator.getTGExpressionGraph();
  }

  inline EvaluationStrategy<T_element>& getStrategy()
  {
    return evaluator.getStrategy();
  }

  // Creates a storage representation for a new TGScalar/TGVector/TGMatrix
  typename ExprTGTraits<exprType, T_element>::internalRep* createTGInternalRep(const bool external, const ExprNode<exprType, T_element>& e)
  {
    return new typename ExprTGTraits<exprType, T_element>::conventionalStorage(external, getGraph().getNameGenerator(), e);
  }

public:
  TGObjectGeneratorHelper(TGEvaluator<T_element>& e) : evaluator(e)
  {
  }
 
  // Handle the addition of a new TGExprNode corresponding to an ExprNode
  void handleNode(ExprNode<exprType, T_element>& e, TGExprNode<tgExprType, T_element>* const tge)
  {
    getGraph().add(tge);
    nodeMap[&e] = tge;
  }

  // Locates the TGExprNode corresponding to an ExprNode.
  TGExprNode<tgExprType, T_element>& getTGExprNode(ExprNode<exprType, T_element>& e)
  {
    const typename std::map<ExprNode<exprType, T_element>*, TGExprNode<tgExprType, T_element>*>::iterator nodeIterator(nodeMap.find(&e));
    // Either find corresponding TGExprNode, or the ExprNode node has never been
    // encountered so we assume it will be evaluated by some other evaluator
    // and map it to a TGLiteral.
    if (nodeIterator != nodeMap.end())
    {
      return *nodeIterator->second;
    }
    else
    {
      Literal<exprType, T_element>* const literal = getStrategy().getEvaluatedExpr(&e);
      typename ExprTGTraits<exprType, T_element>::internalRepCreator creator(getGraph().getNameGenerator());
      literal->getValue().accept(creator);
      handleNode(e, new TGLiteral<tgExprType, T_element>(creator.getResult()));
      return *nodeMap[&e];
    }
  }

  // Creates the TGScalar/TGVector/TGMatrix storage representation for an
  // ExprNode, and maps this to a new Literal with the same storage 
  // representation if necessary.
  typename ExprTGTraits<exprType, T_element>::internalRep* createTGRep(ExprNode<exprType, T_element>& e) 
  {
    const bool external = getStrategy().isExternal(evaluator, e);
    typename ExprTGTraits<exprType, T_element>::internalRep* const tgInternalRep = createTGInternalRep(external, e);
    if (external)
    {
      getStrategy().addEvaluatedExprMapping(&e, new Literal<exprType, T_element>(tgInternalRep->createInternalRep()));
    }
    return tgInternalRep;			 
  }

  // Adds mappings between ExprNode storage representations and TGExprNode storage representations
  // to a ParameterHolder.
  void addTaskGraphMappings(ParameterHolder& parameterHolder)
  {
    for(typename std::map<ExprNode<exprType, T_element>*, TGExprNode<tgExprType, T_element>*>::iterator iterator=nodeMap.begin(); iterator!=nodeMap.end(); ++iterator)
    {
      if (iterator->second->isParameter())
      {
         Literal<exprType, T_element>* const literal = getStrategy().getEvaluatedExpr(iterator->first);
         typename ExprTraits<exprType, T_element>::internalRep& internal = literal->getValue();
         typename ExprTGTraits<exprType, T_element>::internalRep& tgInternal = (iterator->second)->getInternal();
         tgInternal.addParameterMappings(internal, parameterHolder);
      }
    }
  }
};

}
#endif
