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

#ifndef DESOLA_VARIABLE_HPP
#define DESOLA_VARIABLE_HPP

#include <desola/Desola_fwd.hpp>

namespace desola
{

namespace detail
{

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

  virtual ~Variable() {}
};

template<typename expressionType, typename T_element>
class Var : public Variable<T_element>
{
private:
  friend class ScalarElement<vector, T_element>;
  friend class ScalarElement<matrix, T_element>;	  
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
  
  void registerWith(ExpressionNode<T_element>& e) const
  {
    e.registerRequiredBy(*this);
  }

  void unregisterFrom(ExpressionNode<T_element>& e) const
  {
    e.unregisterRequiredBy(*this);
  }

  void setExpr(ExprNode<expressionType, T_element>& e) const
  {
    registerWith(e);
    if (expr != NULL)
      unregisterFrom(*expr);

    expr = &e;
  }

  ExprNode<expressionType, T_element>& getExpr() const
  {
    if (expr == NULL)
    {
      ExprNode<expressionType, T_element>* e = createDefault();
      expr = e;
      registerWith(*e);
    }
    return *expr;
  }
  
  virtual  ExprNode<expressionType, T_element>* createDefault() const = 0;
};

}

}
#endif
