#ifndef DESOLIN_EXPRESSION_NODE_HPP
#define DESOLIN_EXPRESSION_NODE_HPP

#include <cassert>
#include <set>
#include <map>
#include <vector>
#include <algorithm>
#include <iterator>
#include <memory>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <boost/function.hpp>
#include <desolin/Desolin_fwd.hpp>
#include <desolin/tg/Desolin_tg_fwd.hpp>
#include <desolin/profiling/Desolin_profiling_fwd.hpp>

namespace desolin_internal
{

template<typename T_element>
class ExpressionNode
{
private:
  // Forbid copying and assignment
  ExpressionNode(const ExpressionNode&);
  ExpressionNode& operator=(const ExpressionNode&);
  
  EvaluationDirective evaluationDirective;
  std::set< PExpressionNodeRef<T_element> > monitors;
  std::multiset<const Variable<T_element>*> external_reqBy;
  std::vector<ExpressionNode*> internal_reqBy;
  std::vector<ExpressionNode*> deps;

public:
  static std::vector<ExpressionNode*> getTopologicallySortedNodes(const std::vector<ExpressionNode*>& leaves)
  { 
    std::vector<ExpressionNode*> nodes;
    std::set<ExpressionNode*> visited; 
    typedef std::back_insert_iterator< std::vector<ExpressionNode*> > OutputIterator;
    OutputIterator out(nodes);
    std::for_each(leaves.begin(), leaves.end(), boost::bind(getTopologicallySortedNodesHelper<OutputIterator>, _1, boost::ref(visited), out));
    return nodes;
  }
    
  ExpressionNode()
  {
  }

  inline std::vector<ExpressionNode*> getInternalRequiredBy() const
  {
    return internal_reqBy;
  }
  
  inline std::multiset<const Variable<T_element>*> getExternalRequiredBy() const
  {
    return external_reqBy;
  }
  
  inline std::vector<ExpressionNode*> getDependencies() const
  {
    return deps;
  }
  
  inline void registerRequiredBy(const Variable<T_element>& e)
  {
    external_reqBy.insert(&e);
  }
  
  inline void unregisterRequiredBy(const Variable<T_element>& v)
  {
    // We only want to erase one instance from the multiset
    const typename std::multiset<const Variable<T_element>*>::iterator location = external_reqBy.find(&v);
    assert(location != external_reqBy.end());
    external_reqBy.erase(location);
    checkSelfDestruct();
  }

  inline void deleteIfUnused()
  {
    checkSelfDestruct();
  }
  
  inline void evaluate()
  {
    for(typename std::set< PExpressionNodeRef<T_element> >::iterator monitorIterator = monitors.begin(); monitorIterator!=monitors.end(); ++monitorIterator)
    {
      monitorIterator->getPExpressionNode().notifyLive();
    }
    monitors.clear();
    internal_evaluate();
  }
    
  virtual void accept(ExpressionNodeVisitor<T_element>& visitor) = 0;
  virtual void accept(ExpressionNodeTypeVisitor<T_element>& visitor) = 0;
  virtual void accept(LiteralVisitor<T_element>& visitor)
  {
  }

  inline void setEvaluationDirective(const EvaluationDirective d)
  {
    evaluationDirective = d;
  }

  inline EvaluationDirective getEvaluationDirective() const
  {
    return evaluationDirective;
  }

  void notifyOfUse(PExpressionNodeRef<T_element>& monitor)
  {
    monitors.insert(monitor);
  }

  virtual ~ExpressionNode() 
  {
    for(typename std::set< PExpressionNodeRef<T_element> >::iterator monitorIterator = monitors.begin(); monitorIterator!=monitors.end(); ++monitorIterator)
    {
      monitorIterator->getPExpressionNode().notifyDead();
    }
  }

protected:
  virtual void update(ExprNode<scalar, T_element>& previous, ExprNode<scalar, T_element>& next) = 0;
  virtual void update(ExprNode<vector, T_element>& previous, ExprNode<vector, T_element>& next) = 0;
  virtual void update(ExprNode<matrix, T_element>& previous, ExprNode<matrix, T_element>& next) = 0;

  template<ExprType exprType>
  static void updateVariable(const Variable<T_element>* variable, ExprNode<exprType, T_element>& previous, ExprNode<exprType, T_element>& next)
  {
    variable->update(previous, next);
  }

  template<ExprType exprType>
  static void updateExpressionNode(ExpressionNode<T_element>* const expressionNode, ExprNode<exprType, T_element>& previous, ExprNode<exprType, T_element>& next)
  {
    expressionNode->update(previous, next);
  }
  
  template<ExprType exprType>
  void internalReplace(ExprNode<exprType, T_element>& previous, ExprNode<exprType, T_element>& next)
  {
    // This method should only be called by a subclass of ExpressionNode with itself as the "previous" parameter
    // TODO: Fix this. This is polymorphism for code reuse.
    assert(&previous == this);
   
    // Inherit monitoring from node being replaced
    next.monitors.insert(previous.monitors.begin(), previous.monitors.end());
    previous.monitors.clear();
    
    // We make copies because this node might be deleted during this method
    const std::multiset<const Variable<T_element>*> localExternalReqBy(external_reqBy);
    const std::vector<ExpressionNode<T_element>*> localInternalReqBy(internal_reqBy);

    std::for_each(localExternalReqBy.begin(), localExternalReqBy.end(), boost::bind(updateVariable<exprType>, _1, boost::ref(previous), boost::ref(next)));
    std::for_each(localInternalReqBy.begin(), localInternalReqBy.end(), boost::bind(updateExpressionNode<exprType>, _1, boost::ref(previous), boost::ref(next)));
  }
  
  inline void registerDependency(ExpressionNode* const e)
  {
    assert(e != NULL);
    
    deps.push_back(e);
    e->registerRequiredBy(this);  
  }
 
  inline void replaceDependency(ExpressionNode* const previous, ExpressionNode* const next)
  {
    assert(previous != NULL);
    assert(next != NULL);
    
    const typename std::vector<ExpressionNode*>::iterator location = std::find(deps.begin(), deps.end(), previous);
    assert(location != deps.end());
    *location = next;
    next->registerRequiredBy(this);
    previous->unregisterRequiredBy(this);
  }
  
  inline void registerRequiredBy(ExpressionNode* const e)
  {
    assert(e != NULL);
    internal_reqBy.push_back(e);
  }
  
  inline void unregisterRequiredBy(ExpressionNode* const e)
  {
    assert(e != NULL);
    // We only want to erase one instance from the multiset
    const typename std::vector<ExpressionNode*>::iterator location = std::find(internal_reqBy.begin(), internal_reqBy.end(), e);
    assert(location != internal_reqBy.end());
    internal_reqBy.erase(location);
    checkSelfDestruct();
  }
  
  void checkSelfDestruct()
  {
    if(internal_reqBy.empty() && external_reqBy.empty())
    {
      typedef typename std::vector<ExpressionNode*>::iterator Iterator;
      for(Iterator i = deps.begin(); i != deps.end(); ++i)
      {
        (*i)->unregisterRequiredBy(this);
      }
      delete this;
    }
  }

  static void getLeavesHelper(std::set<ExpressionNode*>& visited, std::vector<ExpressionNode*>& leaves, ExpressionNode* const node)
  {
    if (visited.insert(node).second)
    {
      if (node->internal_reqBy.empty())
      {
        leaves.push_back(node);
      }

      std::for_each(node->deps.begin(), node->deps.end(), boost::bind(getLeavesHelper, boost::ref(visited), boost::ref(leaves), _1));
      std::for_each(node->internal_reqBy.begin(), node->internal_reqBy.end(), boost::bind(getLeavesHelper, boost::ref(visited), boost::ref(leaves), _1));
    }	   
  }

  std::vector<ExpressionNode*> getLeaves() 
  {
    std::set<ExpressionNode*> visited;
    std::vector<ExpressionNode*> leaves;
    getLeavesHelper(visited, leaves, this);
    return leaves;
  }

  template<typename OutputIterator>
  static void getTopologicallySortedNodesHelper(ExpressionNode* const node, std::set<ExpressionNode*>& visited, OutputIterator out)
  {
    if (visited.insert(node).second)
    {
      std::for_each(node->deps.begin(), node->deps.end(), boost::bind(getTopologicallySortedNodesHelper<OutputIterator>, _1, boost::ref(visited), out));
      *out++ = node;
    }
  }

  virtual void internal_evaluate()
  {
    const std::vector<ExpressionNode*> leaves(getLeaves());
    const std::vector<ExpressionNode*> nodes(getTopologicallySortedNodes(leaves));
    ExpressionGraph<T_element> expressionGraph(nodes.begin(), nodes.end());

    Profiler<T_element>& profiler = Profiler<T_element>::getProfiler();
    std::auto_ptr< ExpressionGraph<T_element> > profiledGraph = profiler.getAnnotatedExpressionGraph(expressionGraph, *this);

    boost::shared_ptr< EvaluationStrategy<T_element> > strategy = profiledGraph->createEvaluationStrategy();
    TGEvaluatorFactory<T_element> tgEvaluatorFactory;
    strategy->addEvaluator(tgEvaluatorFactory);
    strategy->execute();
  }
};

}
#endif
