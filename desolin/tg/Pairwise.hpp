#ifndef DESOLIN_TG_PAIRWISE_HPP
#define DESOLIN_TG_PAIRWISE_HPP

#include <desolin/tg/Desolin_tg_fwd.hpp>

template<TGExprType exprType, typename T_element>
class TGPairwise : public TGBinOp<exprType, exprType, exprType, T_element>
{
private:
  const TGPairwiseOp op;

public:  
  TGPairwise(typename TGInternalType<exprType, T_element>::type* internal, 
		  TGPairwiseOp o,
		  TGExprNode<exprType, T_element>& l, 
		  TGExprNode<exprType, T_element>& r) : TGBinOp<exprType, exprType, exprType, T_element>(internal, l, r), op(o)
  {
  }


  void accept(TGExpressionNodeVisitor<T_element>& v)
  {
    v.visit(*this);
  }

  TGPairwiseOp getOperation() const
  {
    return op;
  }
};

#endif
