#ifndef DESOLIN_EXPRESSION_NODE_HPP
#define DESOLIN_EXPRESSION_NODE_HPP

#include <iostream>
#include <cassert>
#include <set>
#include <map>
#include <vector>
#include <algorithm>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <boost/function.hpp>
#include <desolin/Desolin_fwd.hpp>
#include <desolin/tg/Desolin_tg_fwd.hpp>

namespace desolin_internal
{

template<typename T_element>
class ExpressionNode
{
private:
  // Forbid copying and assignment
  ExpressionNode(const ExpressionNode&);
  ExpressionNode& operator=(const ExpressionNode&);
  
  std::multiset<const Variable<T_element>*> external_reqBy;
  std::multiset<ExpressionNode*> internal_reqBy;
  std::multiset<ExpressionNode*> deps;

public:
  ExpressionNode()
  {
  }

  inline std::multiset<ExpressionNode*> getInternalRequiredBy() const
  {
    return internal_reqBy;
  }
  
  inline std::multiset<const Variable<T_element>*> getExternalRequiredBy() const
  {
    return external_reqBy;
  }
  
  inline std::multiset<ExpressionNode*> getDependencies() const
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
    external_reqBy.erase(external_reqBy.find(&v));
    checkSelfDestruct();
  }

  inline void deleteIfUnused()
  {
    checkSelfDestruct();
  }
  
  inline void evaluate()
  {
    internal_evaluate();
  }
    
  virtual void accept(ExpressionNodeVisitor<T_element>& visitor) = 0;
  virtual void accept(ExpressionNodeTypeVisitor<T_element>& visitor) = 0;
  virtual void accept(LiteralVisitor<T_element>& visitor)
  {
  }
  
  virtual ~ExpressionNode() {}

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
  static void updateExpressionNode(ExpressionNode<T_element>* expressionNode, ExprNode<exprType, T_element>& previous, ExprNode<exprType, T_element>& next)
  {
    expressionNode->update(previous, next);
  }
  
  template<ExprType exprType>
  void internalReplace(ExprNode<exprType, T_element>& previous, ExprNode<exprType, T_element>& next)
  {
    // This method should only be called by a subclass of ExpressionNode with itself as the "previous" parameter
    assert(&previous == this);
    
    // We make copies because this node might be deleted during this method
    const std::multiset<const Variable<T_element>*> localExternalReqBy(external_reqBy);
    const std::multiset<ExpressionNode<T_element>*> localInternalReqBy(internal_reqBy);

    std::for_each(localExternalReqBy.begin(), localExternalReqBy.end(), boost::bind(updateVariable<exprType>, _1, boost::ref(previous), boost::ref(next)));
    std::for_each(localInternalReqBy.begin(), localInternalReqBy.end(), boost::bind(updateExpressionNode<exprType>, _1, boost::ref(previous), boost::ref(next)));
  }
  
  inline void registerDependency(ExpressionNode* e)
  {
    deps.insert(e);
    e->registerRequiredBy(this);  
  }
  
  inline void unregisterDependency(ExpressionNode* e)
  {
    // We only want to erase one instance from the multiset
    deps.erase(deps.find(e));
    e->unregisterRequiredBy(this);  
  }
  
  inline void registerRequiredBy(ExpressionNode* e)
  {
    internal_reqBy.insert(e);
  }
  
  inline void unregisterRequiredBy(ExpressionNode* e)
  {
    // We only want to erase one instance from the multiset
    internal_reqBy.erase(internal_reqBy.find(e));
    checkSelfDestruct();
  }
  
  void checkSelfDestruct()
  {
    if(internal_reqBy.empty() && external_reqBy.empty())
    {
      std::for_each(deps.begin(), deps.end(), boost::bind(&ExpressionNode::unregisterDependency, this, _1));
      delete this;
    }
  }

  static void getLeavesHelper(std::set<ExpressionNode*>& visited, std::set<ExpressionNode*>& leaves, ExpressionNode* node)
  {
    if (visited.insert(node).second)
    {
      if (node->internal_reqBy.empty())
      {
        leaves.insert(node);
      }

      std::for_each(node->deps.begin(), node->deps.end(), boost::bind(getLeavesHelper, boost::ref(visited), boost::ref(leaves), _1));
      std::for_each(node->internal_reqBy.begin(), node->internal_reqBy.end(), boost::bind(getLeavesHelper, boost::ref(visited), boost::ref(leaves), _1));
    }	   
  }

  std::set<ExpressionNode*> getLeaves() 
  {
    std::set<ExpressionNode*> visited;
    std::set<ExpressionNode*> leaves;
    getLeavesHelper(visited, leaves, this);
    return leaves;
  }

  template<typename OutputIterator>
  static void getTopologicallySortedNodesHelper(ExpressionNode* node, std::set<ExpressionNode*>& visited, OutputIterator out)
  {
    if (visited.insert(node).second)
    {
      std::for_each(node->deps.begin(), node->deps.end(), boost::bind(getTopologicallySortedNodesHelper<OutputIterator>, _1, boost::ref(visited), out));
      *out++ = node;
    }
  }

  std::vector<ExpressionNode*> getTopologicallySortedNodes() 
  {
    const std::set<ExpressionNode*> leaves(getLeaves());
    std::vector<ExpressionNode*> nodes;
    std::set<ExpressionNode*> visited;
    typedef std::back_insert_iterator< std::vector<ExpressionNode*> > OutputIterator;
    OutputIterator out(nodes);

    std::for_each(leaves.begin(), leaves.end(), boost::bind(getTopologicallySortedNodesHelper<OutputIterator>, _1, boost::ref(visited), out));
    return nodes;
  }

  virtual void internal_evaluate()
  {
    std::vector<ExpressionNode*> nodes(getTopologicallySortedNodes());
    ExpressionGraph<T_element> expressionGraph(nodes.begin(), nodes.end());
    boost::shared_ptr< EvaluationStrategy<T_element> > strategy = expressionGraph.createEvaluationStrategy();
    TGEvaluatorFactory<T_element> tgEvaluatorFactory;
    strategy->addEvaluator(tgEvaluatorFactory);
    strategy->execute();
  }
};

}
#endif
