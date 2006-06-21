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

#ifndef DESOLIN_MATRIX_HPP
#define DESOLIN_MATRIX_HPP

#include <desolin/Desolin_fwd.hpp>

namespace desolin
{

template<typename T_element>
class Matrix : public desolin_internal::Var<desolin_internal::matrix, T_element>
{
public:
  friend class Scalar<T_element>;
  friend class Vector<T_element>;
	  
  // Typedefs for ITL
  typedef Scalar<T_element> value_type;
  typedef int size_type;
	
  static const desolin_internal::ExprType expressionType = desolin_internal::matrix;
    
  Matrix()
  {
  }
 
  explicit Matrix(const int rows, const int cols) : desolin_internal::Var<desolin_internal::matrix, T_element>(*new desolin_internal::Literal<desolin_internal::matrix, T_element>(new desolin_internal::ConventionalMatrix<T_element>(rows, cols, 0)))
  {
  }
  
  
  Matrix(const int rows, const int cols, const T_element initialValue) : desolin_internal::Var<desolin_internal::matrix, T_element>(*new desolin_internal::Literal<desolin_internal::matrix, T_element>(new desolin_internal::ConventionalMatrix<T_element>(rows, cols, initialValue)))
  {
  }

  Matrix(const Matrix& m) : desolin_internal::Var<desolin_internal::matrix, T_element>(m.getExpr())
  {
  }

  explicit Matrix(harwell_boeing_stream<T_element>& stream) : desolin_internal::Var<desolin_internal::matrix, T_element>(*new desolin_internal::Literal<desolin_internal::matrix, T_element>(new desolin_internal::ConventionalMatrix<T_element>(stream)))
  {
  }

  explicit Matrix(matrix_market_stream<T_element>& stream) : desolin_internal::Var<desolin_internal::matrix, T_element>(*new desolin_internal::Literal<desolin_internal::matrix, T_element>(new desolin_internal::ConventionalMatrix<T_element>(stream)))
  {
  }

  const int numRows() const
  {
    return this->getExpr().getRowCount();
  }

  const int numCols() const
  {
    return this->getExpr().getColCount();
  }

  Matrix& operator=(const Matrix& right)
  {
    if(this != &right)
    {
      setExpr(right.getExpr());
    }
    return *this;
  }

  const Matrix& operator=(const Scalar<T_element>& right)
  {
    using namespace desolin_internal;
    setExpr(*new ScalarPiecewise<matrix, T_element>(assign, this->getExpr(), right.getExpr()));
    return *this;
  }

  const Matrix operator+(const Matrix& right) const
  {
    using namespace desolin_internal;
    return Matrix(*new Pairwise<matrix, T_element>(pair_add, this->getExpr(), right.getExpr()));
  }

  const Matrix operator-(const Matrix& right) const
  {
    using namespace desolin_internal;
    return Matrix(*new Pairwise<matrix, T_element>(pair_sub, this->getExpr(), right.getExpr()));
  }

  const Matrix operator-() const
  {
    using namespace desolin_internal;
    return Matrix(*new Negate<matrix, T_element>(this->getExpr()));
  }
  
    
  const Matrix operator*(const Scalar<T_element>& right) const
  {
    using namespace desolin_internal;
    return Matrix(*new ScalarPiecewise<matrix, T_element>(multiply, this->getExpr(), right.getExpr()));
  }

  const Matrix operator/(const Scalar<T_element>& right) const
  {
    using namespace desolin_internal;
    return Matrix(*new ScalarPiecewise<matrix, T_element>(divide, this->getExpr(), right.getExpr()));
  }      

  const Matrix operator*(const Matrix& right) const
  {
    using namespace desolin_internal;
    return Matrix(*new MatrixMult<T_element>(this->getExpr(), right.getExpr()));
  }

  const Vector<T_element> operator*(const Vector<T_element>& right) const
  {
    using namespace desolin_internal;
    return Vector<T_element>(*new MatrixVectorMult<T_element>(this->getExpr(), right.getExpr()));
  }

  const Matrix transpose() const
  {
    using namespace desolin_internal;
    return Matrix(*new MatrixTranspose<T_element>(this->getExpr()));
  }

  const Vector<T_element> trans_mult(const Vector<T_element>& right) const
  {
    using namespace desolin_internal;
    return Vector<T_element>(*new TransposeMatrixVectorMult<T_element>(this->getExpr(), right.getExpr()));
  }

  const Matrix ele_mul(const Matrix& right) const
  {
    using namespace desolin_internal;
    return Matrix(*new Pairwise<matrix, T_element>(pair_mul, this->getExpr(), right.getExpr()));
  }

  const Matrix ele_div(const Matrix& right) const
  {  
    using namespace desolin_internal;
    return Matrix(*new Pairwise<matrix, T_element>(pair_div, this->getExpr(), right.getExpr()));
  }

  ScalarElement<desolin_internal::matrix, T_element> operator()(const int row, const int col) const
  {
    using namespace desolin_internal;
    const ElementIndex<matrix> index(row, col);
    return ScalarElement<matrix, T_element>(*this, index);
  }

protected:
  Matrix(desolin_internal::ExprNode<desolin_internal::matrix, T_element>& expr) : desolin_internal::Var<desolin_internal::matrix, T_element>(expr)
  {
  }

  virtual desolin_internal::ExprNode<desolin_internal::matrix, T_element>* createDefault() const
  { 
    throw NullSizeError("Invalid null size matrix usage");
  }
  
  virtual void internal_update(desolin_internal::ExprNode<desolin_internal::scalar, T_element>& previous, desolin_internal::ExprNode<desolin_internal::scalar, T_element>& next) const {}
  virtual void internal_update(desolin_internal::ExprNode<desolin_internal::vector, T_element>& previous, desolin_internal::ExprNode<desolin_internal::vector, T_element>& next) const {}
  virtual void internal_update(desolin_internal::ExprNode<desolin_internal::matrix, T_element>& previous, desolin_internal::ExprNode<desolin_internal::matrix, T_element>& next) const
  {
    if (&this->getExpr() == &previous)
    {
      this->setExpr(next);
    }
  }   
};

}
#endif
