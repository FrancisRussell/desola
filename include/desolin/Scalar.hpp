#ifndef DESOLIN_SCALAR_HPP
#define DESOLIN_SCALAR_HPP

#include <iostream>
#include <desolin/Desolin_fwd.hpp>

namespace desolin
{

template<typename T_element>
class Scalar : public desolin_internal::Var<desolin_internal::scalar, T_element>
{
public:
  friend class Matrix<T_element>;
  friend class Vector<T_element>;

  static const desolin_internal::ExprType expressionType = desolin_internal::scalar;
  
  Scalar()
  {
  }

  Scalar(const T_element initialValue) : desolin_internal::Var<desolin_internal::scalar, T_element>(*new desolin_internal::Literal<desolin_internal::scalar, T_element>(new desolin_internal::ConventionalScalar<T_element>(initialValue)))
  {
  }

  Scalar(const Scalar& s) : desolin_internal::Var<desolin_internal::scalar, T_element>(s.getExpr())
  {
  }

  Scalar& operator=(const Scalar& right)
  {
    using namespace desolin_internal;
    if(this != &right)
    {
      this->setExpr(right.getExpr());
    }
    return *this;
  }

  const Scalar operator+(const Scalar& right) const
  {
    using namespace desolin_internal;
    return Scalar(*new Pairwise<scalar, T_element>(add, this->getExpr(), right.getExpr()));
  }

  const Scalar operator-(const Scalar& right) const
  {
    using namespace desolin_internal;
    return Scalar(*new Pairwise<scalar, T_element>(sub, this->getExpr(), right.getExpr()));
  }

  template<typename T_rightType>
  const T_rightType operator*(const T_rightType& right) const
  {
    using namespace desolin_internal;
    return T_rightType(*new ScalarPiecewise<T_rightType::expressionType, T_element>(multiply, this->getExpr(), right.getExpr()));
  }

  template<typename T_rightType>
  const Scalar operator/(const T_rightType& right) const
  {
    using namespace desolin_internal;
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
  Scalar(desolin_internal::ExprNode<desolin_internal::scalar, T_element>& expr) : desolin_internal::Var<desolin_internal::scalar, T_element>(expr)
  {
  }

  virtual desolin_internal::ExprNode<desolin_internal::scalar, T_element>* createDefault() const
  {
    using namespace desolin_internal;
    InternalScalar<T_element>* internal = new ConventionalScalar<T_element>(0);
    internal->allocate();
    return new Literal<scalar, T_element>(internal);
  }

  virtual void internal_update(desolin_internal::ExprNode<desolin_internal::scalar, T_element>& previous, desolin_internal::ExprNode<desolin_internal::scalar, T_element>& next) const
  {
    using namespace desolin_internal;
    if (&this->getExpr() == &previous)
    {
      this->setExpr(next);
    }
  }
  
  virtual void internal_update(desolin_internal::ExprNode<desolin_internal::vector, T_element>& previous, desolin_internal::ExprNode<desolin_internal::vector, T_element>& next) const {}
  virtual void internal_update(desolin_internal::ExprNode<desolin_internal::matrix, T_element>& previous, desolin_internal::ExprNode<desolin_internal::matrix, T_element>& next) const {}
};

template<desolin_internal::ExprType exprType, typename T_element>
class ScalarElement : public Scalar<T_element>
{
private:
  const desolin_internal::Var<exprType, T_element>& node;
  const desolin_internal::ElementIndex<exprType> index;

public:
  ScalarElement(const ScalarElement& n) : Scalar<T_element>(n.getExpr()), node(n.node), index(n.index)
  {
  }

  ScalarElement(const desolin_internal::Var<exprType, T_element>& n, const desolin_internal::ElementIndex<exprType>& i) :  Scalar<T_element>(*new desolin_internal::ElementGet<exprType, T_element>(n.getExpr(), i)), node(n), index(i)
  {
  }
  
  const ScalarElement& operator=(const Scalar<T_element>& right)
  {
    using namespace desolin_internal;
    if(this != &right)
    {
      node.setExpr(*new ElementSet<exprType, T_element>(node.getExpr(), right.getExpr(), index));
    }
    return *this;
  }
};

}
#endif
