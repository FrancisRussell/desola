#ifndef DESOLIN_ELEMENTAL_HPP
#define DESOLIN_ELEMENTAL_HPP

#include <desolin/Desolin_fwd.hpp>

namespace desolin_internal
{

template<ExprType exprType, typename T_element>
class ElementGet : public UnOp<scalar, exprType, T_element>
{
private:
  const ElementIndex<exprType> index;

public:
  ElementGet(ExprNode<exprType, T_element>& e, const ElementIndex<exprType>& i) : UnOp<scalar, exprType, T_element>(boost::array<int, 0>(), e), index(i)
  {
  }
  
  inline const ElementIndex<exprType>& getIndex() const
  { 
    return index;
  }
  
  virtual void accept(ExpressionNodeVisitor<T_element>& visitor)
  { 
    visitor.visit(*this);
  }

  virtual void internal_evaluate()
  {
    this->getOperand().evaluate();
  }

  virtual T_element getElementValue()
  {
    return this->getOperand().getElementValue(index);
  }   
};

template<ExprType exprType, typename T_element>
class ElementSet : public BinOp<exprType, exprType, scalar, T_element>
{
private:
  const ElementIndex<exprType> index;

public:
  ElementSet(ExprNode<exprType, T_element>& e, ExprNode<scalar, T_element>& s, const ElementIndex<exprType>& i) : BinOp<exprType, exprType, scalar, T_element>(e.getDims(), e, s), index(i)
  { 
  }
  
  inline const ElementIndex<exprType>& getIndex() const
  { 
    return index;
  }
  
  virtual void accept(ExpressionNodeVisitor<T_element>& visitor)
  { 
    visitor.visit(*this);
  }
};

}
#endif
