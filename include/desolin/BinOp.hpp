#ifndef DESOLIN_BINOP_HPP
#define DESOLIN_BINOP_HPP

#include <desolin/Desolin_fwd.hpp>
#include <boost/array.hpp>
#include <boost/cast.hpp>
#include <cassert>

namespace desolin_internal
{

template<ExprType resultType, ExprType leftType, ExprType rightType, typename T_element>
class BinOp : public ExprNode<resultType, T_element>
{
private:
  ExprNode<leftType, T_element>* left;
  ExprNode<rightType, T_element>* right;

  void updateImpl(ExpressionNode<T_element>& previous, ExpressionNode<T_element>& next)
  {
    assert(left != NULL);
    assert(right != NULL);

    if (left == &previous)
    {
      this->registerDependency(&next);
      this->unregisterDependency(&previous);
      left = boost::polymorphic_downcast< ExprNode<leftType, T_element>* >(&next);
    }
    if(right == &previous)
    {
      this->registerDependency(&next);
      this->unregisterDependency(&previous);
      right = boost::polymorphic_downcast< ExprNode<rightType, T_element>* >(&next);
    }
  }

public:
  BinOp(const boost::array<int, ExprDimensions<resultType>::dims> dims, ExprNode<leftType, T_element>& l, ExprNode<rightType, T_element>& r) : ExprNode<resultType, T_element>(dims), left(&l), right(&r)
  {
    this->registerDependency(left);
    this->registerDependency(right);
  }

  inline ExprNode<leftType, T_element>& getLeft()
  {
    return *left;
  }

  inline ExprNode<rightType, T_element>& getRight()
  {
    return *right;
  }

  virtual void update(ExprNode<scalar, T_element>& previous, ExprNode<scalar, T_element>& next)
  {
    updateImpl(previous, next);
  }
  
  virtual void update(ExprNode<vector, T_element>& previous, ExprNode<vector, T_element>& next)
  {
    updateImpl(previous, next);	
  }
  
  virtual void update(ExprNode<matrix, T_element>& previous, ExprNode<matrix, T_element>& next)
  {
    updateImpl(previous, next);
  }
};

}
#endif
