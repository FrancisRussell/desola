#ifndef DESOLIN_PROFILNG_BINOP_HPP
#define DESOLIN_PROFILNG_BINOP_HPP

#include "Desolin_profiling_fwd.hpp"
#include <map>
#include <cassert>

namespace desolin_internal
{

template<ExprType resultType, ExprType leftType, ExprType rightType, typename T_element>
class PBinOp : public PExprNode<resultType, T_element>
{
private:
  PExprNode<leftType, T_element>* left;
  PExprNode<rightType, T_element>* right;

public:
  inline bool isEqual(const PBinOp& node, const std::map<const PExpressionNode<T_element>*, const PExpressionNode<T_element>*>& mappings) const
  {
    assert(mappings.find(left) != mappings.end());
    assert(mappings.find(right) != mappings.end());

    return PExprNode<resultType, T_element>::isEqual(node, mappings) &&
	   mappings.find(left)->second == node.left &&
	   mappings.find(right)->second == node.right;
  }

  PBinOp(PExprNode<leftType, T_element>& l, PExprNode<rightType, T_element>& r) : PExprNode<resultType, T_element>(), left(&l), right(&r)
  {
  }

  inline PExprNode<leftType, T_element>& getLeft()
  {
    return *left;
  }

  inline const PExprNode<leftType, T_element>& getLeft() const
  {
    return *left;
  }

  inline PExprNode<rightType, T_element>& getRight()
  {
    return *right;
  }

  inline const PExprNode<rightType, T_element>& getRight() const
  {
    return *right;
  }  
};

}

#endif

