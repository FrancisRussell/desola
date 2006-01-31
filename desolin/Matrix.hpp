#ifndef DESOLIN_MATRIX_HPP
#define DESOLIN_MATRIX_HPP

#include <desolin/Desolin_fwd.hpp>

template<typename T_element>
class Matrix : public Var<matrix, T_element>
{
public:
  static const ExprType expressionType = matrix;
    
  Matrix()
  {
  }
  
  Matrix(int rows, int cols)
  {
    InternalMatrix<T_element>* internal = new ConventionalMatrix<T_element>(rows, cols);
    internal->allocate();
    setExpr(*new Literal<matrix, T_element>(internal));
  }

  Matrix(const Matrix& m) : Var<matrix, T_element>(m.getExpr())
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
    setExpr(*new ScalarPiecewise<matrix, T_element>(assign, this->getExpr(), right.getExpr()));
    return *this;
  }

  const Matrix operator+(const Matrix& right) const
  {
    return Matrix(*new Pairwise<matrix, T_element>(add, this->getExpr(), right.getExpr()));
  }

  const Matrix operator-(const Matrix& right) const
  {
    return Matrix(*new Pairwise<matrix, T_element>(sub, this->getExpr(), right.getExpr()));
  }
    
  const Matrix operator*(const Scalar<T_element>& right) const
  {
    return Matrix(*new ScalarPiecewise<matrix, T_element>(multiply, this->getExpr(), right.getExpr()));
  }

  const Matrix operator/(const Scalar<T_element>& right) const
  {
    return Matrix(*new ScalarPiecewise<matrix, T_element>(divide, this->getExpr(), right.getExpr()));
  }      

  const Matrix operator*(const Matrix& right) const
  {
    return Matrix(*new MatrixMult<T_element>(this->getExpr(), right.getExpr()));
  }

  const Vector<T_element> operator*(const Vector<T_element>& right) const
  {
    return Vector<T_element>(*new MatrixVectorMult<T_element>(this->getExpr(), right.getExpr()));
  }

  const Matrix transpose() const
  {
    return Matrix(*new MatrixTranspose<T_element>(this->getExpr()));
  }

  const Vector<T_element> trans_mult(const Vector<T_element>& right) const
  {
    return transpose()*right;
  }

  ScalarElement<matrix, T_element> operator()(const int row, const int col) const
  {
    const ElementIndex<matrix> index(row, col);
    return ScalarElement<matrix, T_element>(*this, index);
  }

protected:
  Matrix(ExprNode<matrix, T_element>& expr) : Var<matrix, T_element>(expr)
  {
  }

  virtual ExprNode<matrix, T_element>& createDefault() const
  { 
    throw NullSizeError("Invalid null size matrix usage");
  }
  
  virtual void internal_update(ExprNode<scalar, T_element>& previous, ExprNode<scalar, T_element>& next) const {}
  virtual void internal_update(ExprNode<vector, T_element>& previous, ExprNode<vector, T_element>& next) const {}
  virtual void internal_update(ExprNode<matrix, T_element>& previous, ExprNode<matrix, T_element>& next) const
  {
    if (&this->getExpr() == &previous)
    {
      this->setExpr(next);
    }
  }   
};

#endif
