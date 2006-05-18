#ifndef DESOLIN_PROFILING_SCALAR_PIECEWISE_HPP
#define DESOLIN_PROFILING_SCALAR_PIECEWISE_HPP

#include "Desolin_profiling_fwd.hpp"
#include <map>

namespace desolin_internal
{

template<ExprType exprType, typename T_element>
class PScalarPiecewise : public PBinOp<exprType, exprType, exprType, T_element>
{
private:
  const ScalarPiecewiseOp op;

public:
  PScalarPiecewise(const ScalarPiecewiseOp o, PExprNode<exprType, T_element>& l, PExprNode<exprType, T_element>& r) : PBinOp<exprType, exprType, exprType, T_element>(l, r), op(o)
  {
  }

  inline bool isEqual(const PScalarPiecewise& node, const std::map<const PExpressionNode<T_element>*, const PExpressionNode<T_element>*>& mappings) const
  {
    return PBinOp<exprType, exprType, exprType, T_element>::isEqual(node, mappings) &&
	   op == node.op;
  }

  const ScalarPiecewiseOp getOperation() const
  {
    return op;
  }

  virtual void accept(PExpressionNodeVisitor<T_element>& visitor)
  {
    visitor.visit(*this);
  } 
};

}

#endif
