/****************************************************************************/
/* Copyright 2005-2006, Francis Russell                                     */
/*                                                                          */
/* Licensed under the Apache License, Version 2.0 (the License);            */
/* you may not use this file except in compliance with the License.         */
/* You may obtain a copy of the License at                                  */
/*                                                                          */
/*     http://www.apache.org/licenses/LICENSE-2.0                           */
/*                                                                          */
/* Unless required by applicable law or agreed to in writing, software      */
/* distributed under the License is distributed on an AS IS BASIS,          */
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. */
/* See the License for the specific language governing permissions and      */
/* limitations under the License.                                           */
/*                                                                          */
/****************************************************************************/

#ifndef DESOLIN_SCALAR_HPP
#define DESOLIN_SCALAR_HPP

#include <map>
#include <desolin/Desolin_fwd.hpp>

namespace desolin
{

template<typename T_element>
class Scalar : public detail::Var<detail::scalar, T_element>
{
public:
  friend class Matrix<T_element>;
  friend class Vector<T_element>;

  static const detail::ExprType expressionType = detail::scalar;
  
  Scalar()
  {
  }

  Scalar(const T_element initialValue) : detail::Var<detail::scalar, T_element>(*new detail::Literal<detail::scalar, T_element>(new detail::ConventionalScalar<T_element>(initialValue)))
  {
  }

  Scalar(const Scalar& s) : detail::Var<detail::scalar, T_element>(s.getExpr())
  {
  }

  Scalar& operator=(const Scalar& right)
  {
    using namespace detail;
    if(this != &right)
    {
      this->setExpr(right.getExpr());
    }
    return *this;
  }

  const Scalar operator-() const
  {
    using namespace detail;
    return Scalar(*new Negate<scalar, T_element>(this->getExpr()));
  }

  const Scalar abs() const
  {
    using namespace detail;
    return Scalar(*new Absolute<T_element>(this->getExpr()));
  }
      
  const Scalar sqrt() const
  { 
    using namespace detail;
    return Scalar(*new SquareRoot<T_element>(this->getExpr()));
  }     

  friend const Scalar operator+(const Scalar& left, const Scalar& right)
  {
    using namespace detail;
    return Scalar(*new Pairwise<scalar, T_element>(pair_add, left.getExpr(), right.getExpr()));
  }

  friend const Scalar operator-(const Scalar& left, const Scalar& right)
  {
    using namespace detail;
    return Scalar(*new Pairwise<scalar, T_element>(pair_sub, left.getExpr(), right.getExpr()));
  }

  friend const Scalar operator*(const Scalar& left, const Scalar& right)
  {
    using namespace detail;
    return Scalar(*new ScalarPiecewise<scalar, T_element>(multiply, left.getExpr(), right.getExpr()));
  }

  const Vector<T_element> operator*(const Vector<T_element>& right) const
  {
    using namespace detail;
    return Vector<T_element>(*new ScalarPiecewise<vector, T_element>(multiply, this->getExpr(), right.getExpr()));
  }

  const Matrix<T_element> operator*(const Matrix<T_element>& right) const
  {
    using namespace detail;
    return Matrix<T_element>(*new ScalarPiecewise<matrix, T_element>(multiply, this->getExpr(), right.getExpr()));
  }

  friend const Scalar operator/(const Scalar& left, const Scalar& right)
  {
    using namespace detail;
    return Scalar(*new ScalarPiecewise<scalar, T_element>(divide, left.getExpr(), right.getExpr()));
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
  Scalar(detail::ExprNode<detail::scalar, T_element>& expr) : detail::Var<detail::scalar, T_element>(expr)
  {
  }

  virtual detail::ExprNode<detail::scalar, T_element>* createDefault() const
  {
    using namespace detail;
    InternalScalar<T_element>* internal = new ConventionalScalar<T_element>(0);
    internal->allocate();
    return new Literal<scalar, T_element>(internal);
  }

  virtual void internal_update(detail::ExprNode<detail::scalar, T_element>& previous, detail::ExprNode<detail::scalar, T_element>& next) const
  {
    using namespace detail;
    if (&this->getExpr() == &previous)
    {
      this->setExpr(next);
    }
  }
  
  virtual void internal_update(detail::ExprNode<detail::vector, T_element>& previous, detail::ExprNode<detail::vector, T_element>& next) const {}
  virtual void internal_update(detail::ExprNode<detail::matrix, T_element>& previous, detail::ExprNode<detail::matrix, T_element>& next) const {}
};

template<detail::ExprType exprType, typename T_element>
class ScalarElement : public Scalar<T_element>
{
private:
  const detail::Var<exprType, T_element>& node;
  const detail::ElementIndex<exprType> index;

public:
  ScalarElement(const ScalarElement& n) : Scalar<T_element>(n.getExpr()), node(n.node), index(n.index)
  {
  }

  ScalarElement(const detail::Var<exprType, T_element>& n, const detail::ElementIndex<exprType>& i) :  Scalar<T_element>(*new detail::ElementGet<exprType, T_element>(n.getExpr(), i)), node(n), index(i)
  {
  }
  
  const ScalarElement& operator=(const Scalar<T_element>& right)
  {
    using namespace detail;
    if(this != &right)
    {
      std::map<detail::ElementIndex<exprType>, detail::ExprNode<detail::scalar, T_element>*> mappings;
      mappings[index] = &right.getExpr();
      node.setExpr(*new ElementSet<exprType, T_element>(node.getExpr(), mappings));
    }
    return *this;
  }
};

}

namespace std
{
  template<typename T_element>
  const desolin::Scalar<T_element> abs(const desolin::Scalar<T_element>& operand)
  {
    return operand.abs();
  }

  template<typename T_element>
  const desolin::Scalar<T_element> sqrt(const desolin::Scalar<T_element>& operand)
  {
    return operand.sqrt();
  }
}
#endif
