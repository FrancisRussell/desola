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

#ifndef DESOLA_MATRIX_HPP
#define DESOLA_MATRIX_HPP

#include <cstddef>
#include <desola/Desola_fwd.hpp>

namespace desola
{

template<typename T_element>
class Matrix : public detail::Var<detail::matrix, T_element>
{
public:
  friend class Scalar<T_element>;
  friend class Vector<T_element>;
	  
  // Typedefs for ITL
  typedef Scalar<T_element> value_type;
  typedef std::size_t size_type;
	
  typedef detail::matrix expressionType;

  static Matrix loadDense(harwell_boeing_stream<T_element>& stream)
  {
    return Matrix(*new detail::Literal<detail::matrix, T_element>(new detail::ConventionalMatrix<T_element>(stream)));
  }

  static Matrix loadSparse(harwell_boeing_stream<T_element>& stream)
  {
    return Matrix(*new detail::Literal<detail::matrix, T_element>(new detail::CRSMatrix<T_element>(stream)));
  }
    
  static Matrix loadDense(matrix_market_stream<T_element>& stream)
  {
    return Matrix(*new detail::Literal<detail::matrix, T_element>(new detail::ConventionalMatrix<T_element>(stream)));
  }

  static Matrix loadSparse(matrix_market_stream<T_element>& stream)
  {
    return Matrix(*new detail::Literal<detail::matrix, T_element>(new detail::CRSMatrix<T_element>(stream)));
  }

  Matrix()
  {
  }
 
  explicit Matrix(const size_type rows, const size_type cols) : detail::Var<detail::matrix, T_element>(*new detail::Literal<detail::matrix, T_element>(new detail::ConventionalMatrix<T_element>(rows, cols)))
  {
  }
  
  Matrix(const Matrix& m) : detail::Var<detail::matrix, T_element>(m.getExpr())
  {
  }

  const size_type numRows() const
  {
    return this->getExpr().getRowCount();
  }

  const size_type numCols() const
  {
    return this->getExpr().getColCount();
  }

  const Maybe<size_type> nnz() const
  {
    this->getExpr().evaluate();
    return this->getExpr().nnz();
  }

  Matrix& operator=(const Matrix& right)
  {
    if(this != &right)
      setExpr(right.getExpr());

    return *this;
  }

  const Matrix& operator=(const Scalar<T_element>& right)
  {
    using namespace detail;
    setExpr(*new ScalarPiecewise<matrix, T_element>(piecewise_assign, this->getExpr(), right.getExpr()));
    return *this;
  }

  const Matrix operator+(const Matrix& right) const
  {
    using namespace detail;
    return Matrix(*new Pairwise<matrix, T_element>(pair_add, this->getExpr(), right.getExpr()));
  }

  const Matrix operator-(const Matrix& right) const
  {
    using namespace detail;
    return Matrix(*new Pairwise<matrix, T_element>(pair_sub, this->getExpr(), right.getExpr()));
  }

  const Matrix operator-() const
  {
    using namespace detail;
    return Matrix(*new Negate<matrix, T_element>(this->getExpr()));
  }
  
    
  const Matrix operator*(const Scalar<T_element>& right) const
  {
    using namespace detail;
    return Matrix(*new ScalarPiecewise<matrix, T_element>(piecewise_multiply, this->getExpr(), right.getExpr()));
  }

  const Matrix operator/(const Scalar<T_element>& right) const
  {
    using namespace detail;
    return Matrix(*new ScalarPiecewise<matrix, T_element>(piecewise_divide, this->getExpr(), right.getExpr()));
  }      

  const Matrix operator*(const Matrix& right) const
  {
    using namespace detail;
    return Matrix(*new MatrixMult<T_element>(this->getExpr(), right.getExpr()));
  }

  const Vector<T_element> operator*(const Vector<T_element>& right) const
  {
    using namespace detail;
    return Vector<T_element>(*new MatrixVectorMult<T_element>(this->getExpr(), right.getExpr()));
  }

  const Matrix transpose() const
  {
    using namespace detail;
    return Matrix(*new MatrixTranspose<T_element>(this->getExpr()));
  }

  const Vector<T_element> trans_mult(const Vector<T_element>& right) const
  {
    using namespace detail;
    return Vector<T_element>(*new TransposeMatrixVectorMult<T_element>(this->getExpr(), right.getExpr()));
  }

  const Matrix ele_mul(const Matrix& right) const
  {
    using namespace detail;
    return Matrix(*new Pairwise<matrix, T_element>(pair_mul, this->getExpr(), right.getExpr()));
  }

  const Matrix ele_div(const Matrix& right) const
  {  
    using namespace detail;
    return Matrix(*new Pairwise<matrix, T_element>(pair_div, this->getExpr(), right.getExpr()));
  }

  ScalarElement<detail::matrix, T_element> operator()(const size_type row, const size_type col) const
  {
    using namespace detail;
    const ElementIndex<matrix> index(row, col);
    return ScalarElement<matrix, T_element>(*this, index);
  }

protected:
  Matrix(detail::ExprNode<detail::matrix, T_element>& expr) : detail::Var<detail::matrix, T_element>(expr)
  {
  }

  virtual detail::ExprNode<detail::matrix, T_element>* createDefault() const
  { 
    throw NullSizeError("Invalid null size matrix usage");
  }
  
  virtual void internal_update(detail::ExprNode<detail::scalar, T_element>& previous, detail::ExprNode<detail::scalar, T_element>& next) const {}
  virtual void internal_update(detail::ExprNode<detail::vector, T_element>& previous, detail::ExprNode<detail::vector, T_element>& next) const {}
  virtual void internal_update(detail::ExprNode<detail::matrix, T_element>& previous, detail::ExprNode<detail::matrix, T_element>& next) const
  {
    if (&this->getExpr() == &previous)
      this->setExpr(next);
  }   
};

}
#endif
