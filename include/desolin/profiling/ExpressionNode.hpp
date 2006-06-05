#ifndef DESOLIN_PROFILING_EXPRESSION_NODE_HPP
#define DESOLIN_PROFILING_EXPRESSION_NODE_HPP

#include "Desolin_profiling_fwd.hpp"
#include <boost/shared_ptr.hpp>
#include <map>

namespace desolin_internal
{

template<typename T_element>
class PExpressionNode
{
private:
  PExpressionNode(const PExpressionNode&);
  PExpressionNode& operator=(const PExpressionNode&);

  int liveCount;
  int deadCount;

public:
  PExpressionNode() : liveCount(0), deadCount(0)
  {
  }

  virtual void accept(PExpressionNodeVisitor<T_element>& visitor) = 0;

  void notifyLive()
  {
    ++liveCount;
  }

  void notifyDead()
  {
    ++deadCount;
  }

  EvaluationDirective getSuggestedEvaluationDirective() const
  {
    if (liveCount-deadCount>=0 || (liveCount == deadCount && liveCount == 0))
    {
      return EVALUATE;
    }
    else
    {
      return NO_EVALUATE;
    }
  }

  virtual ~PExpressionNode()
  {
  }
};

template<ExprType exprType, typename T_element>
class PExprNode : public PExpressionNode<T_element>
{
public:
  inline bool isEqual(const PExprNode& node, const std::map<const PExpressionNode<T_element>*, const PExpressionNode<T_element>*>& mappings) const
  {
    return true;
  }
};


// Class for storing references to profiling nodes. Also contains a shared 
// pointer to the parent graph, to prevent it being deleted before the 
// reference.

template<typename T_element>
class PExpressionNodeRef
{
private:
  boost::shared_ptr< PExpressionGraph<T_element> > parentGraph;
  PExpressionNode<T_element>* node;

public:
  PExpressionNodeRef(boost::shared_ptr< PExpressionGraph<T_element> > parent, PExpressionNode<T_element>& n) : parentGraph(parent), node(&n)
  {
  }

  PExpressionNodeRef(const PExpressionNodeRef& ref) : parentGraph(ref.parentGraph), node(ref.node)
  {
  }

  PExpressionNodeRef& operator=(const PExpressionNodeRef& ref)
  {
    parentGraph = ref.parentGraph;
    node = ref.node;
    return *this;
  }
  
  inline PExpressionNode<T_element>& getPExpressionNode() const
  {
    return *node;
  }

  bool operator==(const PExpressionNodeRef& ref) const
  {
    return node == ref.node;
  }

  bool operator<(const PExpressionNodeRef& ref) const
  {
    return node < ref.node;
  }
};

}

#endif
