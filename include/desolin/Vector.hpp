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

#ifndef DESOLIN_VECTOR_HPP
#define DESOLIN_VECTOR_HPP

#include <cstddef>
#include <desolin/Desolin_fwd.hpp>

namespace desolin
{

template<typename T_element>
class Vector : public detail::Var<detail::vector, T_element>
{
public:
  friend class Scalar<T_element>;
  friend class Matrix<T_element>;
	  
  // Typedefs for ITL
  typedef Scalar<T_element> value_type;
  typedef std::size_t size_type;

  static const detail::ExprType expressionType = detail::vector;
    
  Vector()
  {
  }
 
  explicit Vector(const size_type rows) : detail::Var<detail::vector, T_element>(*new detail::Literal<detail::vector, T_element>(new detail::ConventionalVector<T_element>(rows)))
  {
  }
  
  Vector(const size_type rows, const T_element initialValue) : detail::Var<detail::vector, T_element>(*new detail::Literal<detail::vector, T_element>(new detail::ConventionalVector<T_element>(rows, initialValue)))
  {
  }

  Vector(const Vector& v) : detail::Var<detail::vector, T_element>(v.getExpr())
  {
  }

  Vector& operator=(const Vector& right)
  {
    if(this != &right)
    {
      setExpr(right.getExpr());
    }
    return *this;
  }

  const size_type numRows() const
  {
    return this->getExpr().getRowCount();
  }

  const Vector& operator=(const Scalar<T_element>& right)
  {
    using namespace detail;
    setExpr(*new ScalarPiecewise<vector, T_element>(assign, this->getExpr(), right.getExpr()));
    return *this;
  }

  const Vector operator+(const Vector& right) const
  {
    using namespace detail;
    return Vector(*new Pairwise<vector, T_element>(pair_add, this->getExpr(), right.getExpr()));
  }

  const Vector operator-(const Vector& right) const
  {
    using namespace detail;
    return Vector(*new Pairwise<vector, T_element>(pair_sub, this->getExpr(), right.getExpr()));
  }

  const Vector operator-() const
  {
    using namespace detail;
    return Vector(*new Negate<vector, T_element>(this->getExpr()));
  }  

  const Vector operator*(const Scalar<T_element>& right) const
  {
    using namespace detail;
    return Vector(*new ScalarPiecewise<vector, T_element>(multiply, this->getExpr(), right.getExpr()));
  }

  const Vector operator/(const Scalar<T_element>& right) const
  {
    using namespace detail;
    return Vector(*new ScalarPiecewise<vector, T_element>(divide, this->getExpr(), right.getExpr()));
  }
	 
  const Scalar<T_element> dot(const Vector& right) const
  {
    using namespace detail;
    return Scalar<T_element>(*new VectorDot<T_element>(this->getExpr(), right.getExpr()));
  }

  const Vector cross(const Vector& right) const
  {
    using namespace detail;
    return Vector(*new VectorCross<T_element>(this->getExpr(), right.getExpr()));
  }

  const Scalar<T_element> two_norm() const
  {
    using namespace detail;
    return Scalar<T_element>(*new VectorTwoNorm<T_element>(this->getExpr()));
  }

  const Vector ele_mul(const Vector& right) const
  {
    using namespace detail;
    return Vector(*new Pairwise<vector, T_element>(pair_mul, this->getExpr(), right.getExpr()));
  }

  const Vector ele_div(const Vector& right) const
  {
    using namespace detail;
    return Vector(*new Pairwise<vector, T_element>(pair_div, this->getExpr(), right.getExpr()));
  }

  ScalarElement<detail::vector, T_element> operator()(const size_type row) const
  {
    using namespace detail;
    const ElementIndex<vector> index(row);
    return ScalarElement<vector, T_element>(*this, index);
  }
   
protected:
  Vector(detail::ExprNode<detail::vector, T_element>& expr) : detail::Var<detail::vector, T_element>(expr)
  {
  }
  
  virtual detail::ExprNode<detail::vector, T_element>* createDefault() const
  {
    throw NullSizeError("Invalid null size vector usage");
  }

  virtual void internal_update(detail::ExprNode<detail::scalar, T_element>& previous, detail::ExprNode<detail::scalar, T_element>& next) const {}
  virtual void internal_update(detail::ExprNode<detail::vector, T_element>& previous, detail::ExprNode<detail::vector, T_element>& next) const
  {
    if (&this->getExpr() == &previous)
      this->setExpr(next);
  }
  virtual void internal_update(detail::ExprNode<detail::matrix, T_element>& previous, detail::ExprNode<detail::matrix, T_element>& next) const {}    
};

}
#endif
