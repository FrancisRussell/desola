#ifndef DESOLIN_MATRIX_HPP
#define DESOLIN_MATRIX_HPP

#include <desolin/Desolin_fwd.hpp>

namespace desolin
{

template<typename T_element>
class Matrix : public desolin_internal::Var<desolin_internal::matrix, T_element>
{
public:
  static const desolin_internal::ExprType expressionType = desolin_internal::matrix;
    
  Matrix()
  {
  }
  
  Matrix(const int rows, const int cols, const T_element initialValue) : desolin_internal::Var<desolin_internal::matrix, T_element>(new desolin_internal::Literal<desolin_internal::matrix, T_element>(new desolin_internal::ConventionalMatrix<T_element>(rows, cols, initialValue)))
  {
  }

  Matrix(const Matrix& m) : desolin_internal::Var<desolin_internal::matrix, T_element>(&m.getExpr())
  {
  }

  Matrix(harwell_boeing_stream<T_element>& stream) : desolin_internal::Var<desolin_internal::matrix, T_element>(new desolin_internal::Literal<desolin_internal::matrix, T_element>(new desolin_internal::ConventionalMatrix<T_element>(stream)))
  {
  }

  Matrix(matrix_market_stream<T_element>& stream) : desolin_internal::Var<desolin_internal::matrix, T_element>(new desolin_internal::Literal<desolin_internal::matrix, T_element>(new desolin_internal::ConventionalMatrix<T_element>(stream)))
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
    setExpr(new ScalarPiecewise<matrix, T_element>(assign, this->getExpr(), right.getExpr()));
    return *this;
  }

  const Matrix operator+(const Matrix& right) const
  {
    using namespace desolin_internal;
    return Matrix(new Pairwise<matrix, T_element>(add, this->getExpr(), right.getExpr()));
  }

  const Matrix operator-(const Matrix& right) const
  {
    using namespace desolin_internal;
    return Matrix(new Pairwise<matrix, T_element>(sub, this->getExpr(), right.getExpr()));
  }
    
  const Matrix operator*(const Scalar<T_element>& right) const
  {
    using namespace desolin_internal;
    return Matrix(new ScalarPiecewise<matrix, T_element>(multiply, this->getExpr(), right.getExpr()));
  }

  const Matrix operator/(const Scalar<T_element>& right) const
  {
    using namespace desolin_internal;
    return Matrix(new ScalarPiecewise<matrix, T_element>(divide, this->getExpr(), right.getExpr()));
  }      

  const Matrix operator*(const Matrix& right) const
  {
    using namespace desolin_internal;
    return Matrix(new MatrixMult<T_element>(this->getExpr(), right.getExpr()));
  }

  const Vector<T_element> operator*(const Vector<T_element>& right) const
  {
    using namespace desolin_internal;
    return Vector<T_element>(new MatrixVectorMult<T_element>(this->getExpr(), right.getExpr()));
  }

  const Matrix transpose() const
  {
    using namespace desolin_internal;
    return Matrix(new MatrixTranspose<T_element>(this->getExpr()));
  }

  const Vector<T_element> trans_mult(const Vector<T_element>& right) const
  {
    using namespace desolin_internal;
    return transpose()*right;
  }

  ScalarElement<desolin_internal::matrix, T_element> operator()(const int row, const int col) const
  {
    using namespace desolin_internal;
    const ElementIndex<matrix> index(row, col);
    return ScalarElement<matrix, T_element>(*this, index);
  }

protected:
  Matrix(desolin_internal::ExprNode<desolin_internal::matrix, T_element>* expr) : desolin_internal::Var<desolin_internal::matrix, T_element>(expr)
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
