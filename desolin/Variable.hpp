#ifndef DESOLIN_VARIABLE_HPP
#define DESOLIN_VARIABLE_HPP

#include <desolin/Desolin_fwd.hpp>

template<typename T_element>
class Variable
{
  friend class ExpressionNode<T_element>;

protected:
  inline void update(ExprNode<scalar, T_element>& previous, ExprNode<scalar, T_element>& next) const
  {
    internal_update(previous, next);
  }
  
  inline void update(ExprNode<vector, T_element>& previous, ExprNode<vector, T_element>& next) const
  {
    internal_update(previous, next);
  }
  
  inline void update(ExprNode<matrix, T_element>& previous, ExprNode<matrix, T_element>& next) const
  {
    internal_update(previous, next);
  }
  
  virtual void internal_update(ExprNode<scalar, T_element>& previous, ExprNode<scalar, T_element>& next) const = 0;
  virtual void internal_update(ExprNode<vector, T_element>& previous, ExprNode<vector, T_element>& next) const = 0;
  virtual void internal_update(ExprNode<matrix, T_element>& previous, ExprNode<matrix, T_element>& next) const = 0;
};

template<ExprType expressionType, typename T_element>
class Var : public Variable<T_element>
{
protected:
  friend class Scalar<T_element>;
  friend class Matrix<T_element>;
  friend class Vector<T_element>;

  friend class ScalarElement<vector, T_element>;
  friend class ScalarElement<matrix, T_element>;

private:
  mutable ExprNode<expressionType, T_element>* expr;
    
protected:
  Var() : expr(NULL)
  {
  }

  Var(ExprNode<expressionType, T_element>& e) : expr(&e)
  {
    if (expr != NULL)
      expr->registerRequiredBy(*this);
  }

  virtual ~Var()
  {
    if (expr != NULL)
      expr->unregisterRequiredBy(*this);
  }
  
  inline void registerWith(ExpressionNode<T_element>& e) const
  {
    e.registerRequiredBy(*this);
  }

  inline void unregisterFrom(ExpressionNode<T_element>& e) const
  {
    e.unregisterRequiredBy(*this);
  }

  inline void setExpr(ExprNode<expressionType, T_element>& e) const
  {
    registerWith(e);
    if (expr != NULL)
    {
      unregisterFrom(*expr);
    }
    expr = &e;
  }

  inline ExprNode<expressionType, T_element>& getExpr() const
  {
    if (expr == NULL)
    {
      ExprNode<expressionType, T_element>& e = createDefault();
      expr = &e;
      registerWith(e);
    }
    return *expr;
  }
  
  virtual  ExprNode<expressionType, T_element>& createDefault() const = 0;
};

#endif
