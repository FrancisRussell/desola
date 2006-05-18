#ifndef DESOLIN_PROFILING_LITERAL_HPP
#define DESOLIN_PROFILING_LITERAL_HPP

#include "Desolin_profiling_fwd.hpp"
#include <map>

namespace desolin_interal
{

template<ExprType exprType, typename T_element>
class PLiteral : public PExprNode<exprType, T_element>
{
public:
  inline bool isEqual(const PLiteral& node, const std::map<const PExpressionNode<T_element>*, const PExpressionNode<T_element>*>& mappings)
  {
    return PExprNode<exprType, T_element>::isEqual(node, mappings);
  }

  PLiteral() : PExprNode<exprType, T_element>()
  {
  }

  virtual void accept(PExpressionNodeVisitor<T_element>& visitor)
  {
    visitor.visit(*this);
  }
};

}

#endif
