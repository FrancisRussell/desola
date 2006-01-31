#ifndef DESOLIN_SCALAR_PIECEWISE
#define DESOLIN_SCALAR_PIECEWISE

#include <desolin/Desolin_fwd.hpp>

template<ExprType exprType, typename T_element>
class ScalarPiecewise : public BinOp<exprType, exprType, scalar, T_element>
{
private:
  const ScalarPiecewiseOp operation;

public:  
  ScalarPiecewise(ScalarPiecewiseOp op, ExprNode<exprType, T_element>& o, ExprNode<scalar, T_element>& s) : BinOp<exprType, exprType, scalar, T_element>(o.getDims(), o, s), operation(op)
  {
  }

  ScalarPiecewiseOp getOperation() const
  {
    return operation;
  }

  void accept(ExpressionNodeVisitor<T_element>& v)
  {
    v.visit(*this);
  }
};

#endif
