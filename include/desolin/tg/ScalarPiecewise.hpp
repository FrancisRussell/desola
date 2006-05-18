#ifndef DESOLIN_TG_SCALAR_PIECEWISE_HPP
#define DESOLIN_TG_SCALAR_PIECEWISE_HPP

#include <desolin/tg/Desolin_tg_fwd.hpp>

namespace desolin_internal
{

template<TGExprType exprType, typename T_element>
class TGScalarPiecewise : public TGBinOp<exprType, exprType, tg_scalar, T_element>
{
private:
  const TGScalarPiecewiseOp op;

public:  
  TGScalarPiecewise(typename TGInternalType<exprType, T_element>::type* internal,
		  const TGScalarPiecewiseOp o,
		  TGExprNode<exprType, T_element>& operand,
		  TGExprNode<tg_scalar, T_element>& s) : TGBinOp<exprType, exprType, tg_scalar, T_element>(internal, operand, s), op(o)
  {
  }

  inline bool isEqual(const TGScalarPiecewise& node, const std::map<const TGExpressionNode<T_element>*, const TGExpressionNode<T_element>*>& mappings) const
  {
    return  TGBinOp<exprType, exprType, tg_scalar, T_element>::isEqual(node, mappings) &&
	    op == node.op;
  }

  const TGScalarPiecewiseOp getOperation() const
  {
    return op;
  }

  void accept(TGExpressionNodeVisitor<T_element>& v)
  {
    v.visit(*this);
  }
};

}
#endif
