#ifndef DESOLIN_VECTOR_HPP
#define DESOLIN_VECTOR_HPP

#include <desolin/Desolin_fwd.hpp>

namespace desolin
{

template<typename T_element>
class Vector : public desolin_internal::Var<desolin_internal::vector, T_element>
{
public:
  static const desolin_internal::ExprType expressionType = desolin_internal::vector;
    
  Vector()
  {
  }
  
  Vector(const int rows, const T_element initialValue) : desolin_internal::Var<desolin_internal::vector, T_element>(new desolin_internal::Literal<desolin_internal::vector, T_element>(new desolin_internal::ConventionalVector<T_element>(rows, initialValue)))
  {
  }

  Vector(const Vector& v) : desolin_internal::Var<desolin_internal::vector, T_element>(&v.getExpr())
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
    using namespace desolin_internal;
    setExpr(new ScalarPiecewise<vector, T_element>(assign, this->getExpr(), right.getExpr()));
    return *this;
  }

  const Vector operator+(const Vector& right) const
  {
    using namespace desolin_internal;
    return Vector(new Pairwise<vector, T_element>(add, this->getExpr(), right.getExpr()));
  }

  const Vector operator-(const Vector& right) const
  {
    using namespace desolin_internal;
    return Vector(new Pairwise<vector, T_element>(sub, this->getExpr(), right.getExpr()));
  }

  const Vector operator*(const Scalar<T_element>& right) const
  {
    using namespace desolin_internal;
    return Vector(new ScalarPiecewise<vector, T_element>(multiply, this->getExpr(), right.getExpr()));
  }

  const Vector operator/(const Scalar<T_element>& right) const
  {
    using namespace desolin_internal;
    return Vector(new ScalarPiecewise<vector, T_element>(divide, this->getExpr(), right.getExpr()));
  }
	 
  const Vector dot(const Vector& right) const
  {
    using namespace desolin_internal;
    return Scalar<T_element>(new VectorDot<T_element>(this->getExpr(), right.getExpr()));
  }

  const Vector cross(const Vector& right) const
  {
    using namespace desolin_internal;
    return Vector(new VectorCross<T_element>(this->getExpr(), right.getExpr()));
  }

  const Scalar<T_element> norm() const
  {
    using namespace desolin_internal;
    return Scalar<T_element>(new VectorNorm<T_element>(this->getExpr()));
  }

  ScalarElement<desolin_internal::vector, T_element> operator()(const int row) const
  {
    using namespace desolin_internal;
    const ElementIndex<vector> index(row);
    return ScalarElement<vector, T_element>(*this, index);
  }
   
protected:
  Vector(desolin_internal::ExprNode<desolin_internal::vector, T_element>* expr) : desolin_internal::Var<desolin_internal::vector, T_element>(expr)
  {
  }
  
  virtual desolin_internal::ExprNode<desolin_internal::vector, T_element>* createDefault() const
  {
    throw NullSizeError("Invalid null size vector usage");
  }

  virtual void internal_update(desolin_internal::ExprNode<desolin_internal::scalar, T_element>& previous, desolin_internal::ExprNode<desolin_internal::scalar, T_element>& next) const {}
  virtual void internal_update(desolin_internal::ExprNode<desolin_internal::vector, T_element>& previous, desolin_internal::ExprNode<desolin_internal::vector, T_element>& next) const
  {
    if (&this->getExpr() == &previous)
    {
      this->setExpr(next);
    }  
  }
  virtual void internal_update(desolin_internal::ExprNode<desolin_internal::matrix, T_element>& previous, desolin_internal::ExprNode<desolin_internal::matrix, T_element>& next) const {}    
};

}
#endif
