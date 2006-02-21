#ifndef DESOLIN_TG_UNOP_HPP
#define DESOLIN_TG_UNOP_HPP

#include <set>
#include <map>
#include <cassert>
#include <desolin/tg/Desolin_tg_fwd.hpp>

namespace desolin_internal
{

template<TGExprType resultType, TGExprType exprType, typename T_element>
class TGUnOp : public TGExprNode<resultType, T_element>
{
private:
  TGExprNode<exprType, T_element>* expr;

public:
  inline bool isEqual(const TGUnOp& node, const std::map<TGExpressionNode<T_element>*, TGExpressionNode<T_element>*>& mappings) const
  {
    assert(mappings.find(expr) != mappings.end());

    return TGExprNode<resultType, T_element>::isEqual(node, mappings) && 
           mappings.find(expr)->second == node.expr;
  }
    
  TGUnOp(typename TGInternalType<resultType, T_element>::type* internal, TGExprNode<exprType, T_element>& e) : TGExprNode<resultType, T_element>(internal), expr(&e)
  {
    this->dependencies.insert(expr);
  }

  inline TGExprNode<exprType, T_element>& getOperand()
  {
    return *expr;
  }

  inline const TGExprNode<exprType, T_element>& getOperand() const
  {
    return *expr;
  }

  virtual bool matches(const TGExpressionNode<T_element>& node, const std::map<TGExpressionNode<T_element>*, TGExpressionNode<T_element>*>& mappings) const
  {
    return TGExpressionNode<T_element>::matches(*this, node, mappings);
  }
  
};

template<TGExprType exprType, typename T_element>
class TGNegate : public TGUnOp<exprType, exprType, T_element>
{
public:
  TGNegate(typename TGInternalType<exprType, T_element>::type* internal, TGExprNode<exprType, T_element>& e) : TGUnOp<exprType, exprType, T_element>(internal, e)
  {
  }

  virtual void accept(TGExpressionNodeVisitor<T_element>& v)
  {
    v.visit(*this);
  }
};

template<typename T_element>
class TGAbsolute : public TGUnOp<tg_scalar, tg_scalar, T_element>
{
public:
  TGAbsolute(typename TGInternalType<tg_scalar, T_element>::type* internal, TGExprNode<tg_scalar, T_element>& e) : TGUnOp<tg_scalar, tg_scalar, T_element>(internal, e)
  {
  }

  virtual void accept(TGExpressionNodeVisitor<T_element>& v)
  {
    v.visit(*this);
  }
};

template<typename T_element>
class TGSquareRoot : public TGUnOp<tg_scalar, tg_scalar, T_element>
{
public:
  TGSquareRoot(typename TGInternalType<tg_scalar, T_element>::type* internal, TGExprNode<tg_scalar, T_element>& e) : TGUnOp<tg_scalar, tg_scalar, T_element>(internal, e)
  {
  }

  virtual void accept(TGExpressionNodeVisitor<T_element>& v)
  {
    v.visit(*this);
  }
};

}
#endif
