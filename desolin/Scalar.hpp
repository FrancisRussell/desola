#ifndef DESOLIN_SCALAR_HPP
#define DESOLIN_SCALAR_HPP

#include <iostream>
#include <desolin/Desolin_fwd.hpp>

template<typename T_element>
class Scalar : public Var<scalar, T_element>
{
public:
  static const ExprType expressionType = scalar;
  
  Scalar()
  {
  }

  Scalar(const T_element initialValue) : Var<scalar, T_element>(*new Literal<scalar, T_element>(new ConventionalScalar<T_element>(initialValue)))
  {
  }

  Scalar(const Scalar& s) : Var<scalar, T_element>(s.getExpr())
  {
  }

  Scalar& operator=(const Scalar& right)
  {
    if(this != &right)
    {
      this->setExpr(right.getExpr());
    }
    return *this;
  }

  const Scalar operator+(const Scalar& right) const
  {
    return Scalar(*new Pairwise<scalar, T_element>(add, this->getExpr(), right.getExpr()));
  }

  const Scalar operator-(const Scalar& right) const
  {
    return Scalar(*new Pairwise<scalar, T_element>(sub, this->getExpr(), right.getExpr()));
  }

  template<typename T_rightType>
  const T_rightType operator*(const T_rightType& right) const
  {
    return T_rightType(*new ScalarPiecewise<T_rightType::expressionType, T_element>(multiply, this->getExpr(), right.getExpr()));
  }

  template<typename T_rightType>
  const Scalar operator/(const T_rightType& right) const
  {
    return T_rightType(*new ScalarPiecewise<T_rightType::expressionType, T_element>(divide, this->getExpr(), right.getExpr()));
  }

  bool operator==(const Scalar& right) const
  {
    return value() == right.value();
  }

  bool operator!=(const Scalar& right) const
  {
    return value() != right.value();
  }

  bool operator<(const Scalar& right) const
  {
    return value() < right.value();
  }

  bool operator<=(const Scalar& right) const
  {
    return value() <= right.value();
  }

  bool operator>(const Scalar& right) const
  {
    return value() > right.value();
  }

  bool operator>=(const Scalar& right) const
  {
    return value() >= right.value();
  }

  inline const T_element value() const
  {
    this->getExpr().evaluate();
    return this->getExpr().getElementValue();
  }
    
protected:
  Scalar(ExprNode<scalar, T_element>& expr) : Var<scalar, T_element>(expr)
  {
  }

  virtual ExprNode<scalar, T_element>& createDefault() const
  {
    InternalScalar<T_element>* internal = new ConventionalScalar<T_element>();
    internal->allocate();
    return *new Literal<scalar, T_element>(internal);
  }

  virtual void internal_update(ExprNode<scalar, T_element>& previous, ExprNode<scalar, T_element>& next) const
  {
    if (&this->getExpr() == &previous)
    {
      this->setExpr(next);
    }
  }
  
  virtual void internal_update(ExprNode<vector, T_element>& previous, ExprNode<vector, T_element>& next) const {}
  virtual void internal_update(ExprNode<matrix, T_element>& previous, ExprNode<matrix, T_element>& next) const {}
};

template<ExprType exprType, typename T_element>
class ScalarElement : public Scalar<T_element>
{
private:
  const Var<exprType, T_element>& node;
  const ElementIndex<exprType> index;

public:
  ScalarElement(const ScalarElement& n) : Scalar<T_element>(n.getExpr()), node(n.node), index(n.index)
  {
  }

  ScalarElement(const Var<exprType, T_element>& n, const ElementIndex<exprType>& i) :  Scalar<T_element>(*new ElementGet<exprType, T_element>(n.getExpr(), i)), node(n), index(i)
  {
  }
  
  const ScalarElement& operator=(const Scalar<T_element>& right)
  {
    if(this != &right)
    {
      node.setExpr(*new ElementSet<exprType, T_element>(node.getExpr(), right.getExpr(), index));
    }
    return *this;
  }
};

#endif
