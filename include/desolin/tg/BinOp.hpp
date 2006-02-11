#ifndef DESOLIN_TG_BINOP_HPP
#define DESOLIN_TG_BINOP_HPP

#include <set>
#include <desolin/tg/Desolin_tg_fwd.hpp>

namespace desolin_internal
{

template<TGExprType resultType, TGExprType leftType, TGExprType rightType, typename T_element>
class TGBinOp : public TGExprNode<resultType, T_element>
{
private:
  TGExprNode<leftType, T_element>* left;
  TGExprNode<rightType, T_element>* right;
public:
  TGBinOp(typename TGInternalType<resultType, T_element>::type* internal, TGExprNode<leftType, T_element>& l, TGExprNode<rightType, T_element>& r) : TGExprNode<resultType, T_element>(internal), left(&l), right(&r)
  {
  }

  inline TGExprNode<leftType, T_element>& getLeft()
  {
    return *left;
  }

  inline TGExprNode<rightType, T_element>& getRight()
  {
    return *right;
  }

  virtual std::set<TGExpressionNode<T_element>*> getDependencies() const
  {
    std::set<TGExpressionNode<T_element>*> dependencies;
    dependencies.insert(left);
    dependencies.insert(right);
    return dependencies;
  }
};

}
#endif
