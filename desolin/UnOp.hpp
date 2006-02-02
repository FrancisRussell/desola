#ifndef DESOLIN_UNOP_HPP
#define DESOLIN_UNOP_HPP

#include <boost/array.hpp>
#include <boost/cast.hpp>
#include <desolin/Desolin_fwd.hpp>

namespace desolin_internal
{

template<ExprType resultType, ExprType exprType, typename T_element>
class UnOp : public ExprNode<resultType, T_element>
{
private:
  ExprNode<exprType, T_element>* expr;

  void updateImpl(ExpressionNode<T_element>& previous, ExpressionNode<T_element>& next)
  {
    if (expr == &previous)
    {
      this->registerDependency(&next);
      this->unregisterDependency(&previous);
      expr = boost::polymorphic_downcast< ExprNode<exprType, T_element>* >(&next);
    }
  }
  
public:
  UnOp(const boost::array<int, ExprDimensions<resultType>::dims> dims, ExprNode<exprType, T_element>& e) : ExprNode<resultType, T_element>(dims), expr(&e)
  {
    this->registerDependency(expr);
  }

  inline ExprNode<exprType, T_element>& getOperand()
  {
    return *expr;
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
