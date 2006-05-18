#ifndef DESOLIN_PROFILING_EXPRESSION_NODE_HPP
#define DESOLIN_PROFILING_EXPRESSION_NODE_HPP

#include "Desolin_profiling_fwd.hpp"
#include <map>

namespace desolin_internal
{

template<typename T_element>
class PExpressionNode
{
private:
  PExpressionNode(const PExpressionNode&);
  PExpressionNode operator=(const PExpressionNode&);

public:
  PExpressionNode()
  {
  }

  virtual void accept(PExpressionNodeVisitor<T_element>& visitor) = 0;

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

}

#endif
