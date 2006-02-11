#ifndef DESOLIN_PAIRWISE_HPP
#define DESOLIN_PAIRWISE_HPP

#include <desolin/Desolin_fwd.hpp>

namespace desolin_internal
{

template<ExprType exprType, typename T_element>
class Pairwise : public BinOp<exprType, exprType, exprType, T_element>
{
private:
  const PairwiseOp operation;
  
  static const boost::array<int, ExprDimensions<exprType>::dims> getDims(const ExprNode<exprType, T_element>& l, const ExprNode<exprType, T_element>& r)
  {
    return l.getDims();
  }
public:  
  Pairwise(const PairwiseOp op, ExprNode<exprType, T_element>& l, ExprNode<exprType, T_element>& r) : BinOp<exprType, exprType, exprType, T_element>(getDims(l, r), l, r), operation(op)
  {
  }

  PairwiseOp getOperation() const
  {
    return operation;
  }

  void accept(ExpressionNodeVisitor<T_element>& v)
  {
    v.visit(*this);
  }
};

}
#endif
