#ifndef DESOLIN_VECTOR_HPP
#define DESOLIN_VECTOR_HPP

#include <desolin/Desolin_fwd.hpp>

template<typename T_element>
class Vector : public Var<vector, T_element>
{
public:
  static const ExprType expressionType = vector;
    
  Vector()
  {
  }
  
  Vector(const int rows, const T_element initialValue)
  {
    InternalVector<T_element>* internal = new ConventionalVector<T_element>(rows, initialValue);
    setExpr(*new Literal<vector, T_element>(internal));
  }

  Vector(const Vector& v) : Var<vector, T_element>(v.getExpr())
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

  const int numRows() const
  {
    return this->getExpr().getRowCount();
  }

  const Vector& operator=(const Scalar<T_element>& right)
  {
    setExpr(*new ScalarPiecewise<vector, T_element>(assign, this->getExpr(), right.getExpr()));
    return *this;
  }

  const Vector operator+(const Vector& right) const
  {
    return Vector(*new Pairwise<vector, T_element>(add, this->getExpr(), right.getExpr()));
  }

  const Vector operator-(const Vector& right) const
  {
    return Vector(*new Pairwise<vector, T_element>(sub, this->getExpr(), right.getExpr()));
  }

  const Vector operator*(const Scalar<T_element>& right) const
  {
    return Vector(*new ScalarPiecewise<vector, T_element>(multiply, this->getExpr(), right.getExpr()));
  }

  const Vector operator/(const Scalar<T_element>& right) const
  {
    return Vector(*new ScalarPiecewise<vector, T_element>(divide, this->getExpr(), right.getExpr()));
  }
	 
  const Vector dot(const Vector& right) const
  {
    return Scalar<T_element>(*new VectorDot<T_element>(this->getExpr(), right.getExpr()));
  }

  const Vector cross(const Vector& right) const
  {
    return Vector(*new VectorCross<T_element>(this->getExpr(), right.getExpr()));
  }

  const Scalar<T_element> norm() const
  {
    return Scalar<T_element>(*new VectorNorm<T_element>(this->getExpr()));
  }

  ScalarElement<vector, T_element> operator()(const int row) const
  {
    const ElementIndex<vector> index(row);
    return ScalarElement<vector, T_element>(*this, index);
  }
   
protected:
  Vector(ExprNode<vector, T_element>& expr) : Var<vector, T_element>(expr)
  {
  }
  
  virtual ExprNode<vector, T_element>& createDefault() const
  {
    throw NullSizeError("Invalid null size vector usage");
  }

  virtual void internal_update(ExprNode<scalar, T_element>& previous, ExprNode<scalar, T_element>& next) const {}
  virtual void internal_update(ExprNode<vector, T_element>& previous, ExprNode<vector, T_element>& next) const
  {
    if (&this->getExpr() == &previous)
    {
      this->setExpr(next);
    }  
  }
  virtual void internal_update(ExprNode<matrix, T_element>& previous, ExprNode<matrix, T_element>& next) const {}    
};

#endif
