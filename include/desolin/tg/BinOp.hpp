#ifndef DESOLIN_TG_BINOP_HPP
#define DESOLIN_TG_BINOP_HPP

#include <set>
#include <map>
#include <cassert>
#include <boost/functional/hash.hpp>
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
    this->dependencies.insert(left);
    this->dependencies.insert(right);
  }

  inline TGExprNode<leftType, T_element>& getLeft()
  {
    return *left;
  }

  inline TGExprNode<rightType, T_element>& getRight()
  {
    return *right;
  }
  
  inline virtual std::size_t hashValue(const std::map<TGExpressionNode<T_element>*, int>& nodeNumberings) const
  {
    std::size_t seed = TGExprNode<resultType, T_element>::hashValue(nodeNumberings);
    assert(nodeNumberings.find(left) != nodeNumberings.end());
    assert(nodeNumberings.find(right) != nodeNumberings.end());
    boost::hash_combine(seed, nodeNumberings.find(left)->second);
    boost::hash_combine(seed, nodeNumberings.find(right)->second);
    return seed;
  }
  
};

}
#endif
