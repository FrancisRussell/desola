#ifndef DESOLIN_TG_PAIRWISE_HPP
#define DESOLIN_TG_PAIRWISE_HPP

#include <desolin/tg/Desolin_tg_fwd.hpp>

namespace desolin_internal
{

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

  inline bool isEqual(const TGPairwise& node, const std::map<const TGExpressionNode<T_element>*, const TGExpressionNode<T_element>*>& mappings) const
  {
    return  TGBinOp<exprType, exprType, exprType, T_element>::isEqual(node, mappings) &&
	    op == node.op;
  } 

  TGPairwiseOp getOperation() const
  {
    return op;
  }
};

}
#endif
