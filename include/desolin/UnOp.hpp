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

#ifndef DESOLIN_UNOP_HPP
#define DESOLIN_UNOP_HPP

#include <cmath>
#include <cassert>
#include <boost/array.hpp>
#include <boost/cast.hpp>
#include <desolin/Desolin_fwd.hpp>

namespace desolin
{

namespace detail
{

template<typename resultType, typename exprType, typename T_element>
class UnOp : public ExprNode<resultType, T_element>
{
private:
  ExprNode<exprType, T_element>* expr;

  void updateImpl(ExpressionNode<T_element>& previous, ExpressionNode<T_element>& next)
  {
    assert(expr != NULL);
    
    if (expr == &previous)
    {
      this->replaceDependency(&previous, &next);
      expr = boost::polymorphic_downcast< ExprNode<exprType, T_element>* >(&next);
    }
  }
  
public:
  UnOp(const boost::array<std::size_t, ExprDimensions<resultType>::dims> dims, ExprNode<exprType, T_element>& e) : ExprNode<resultType, T_element>(dims), expr(&e)
  {
    this->registerDependency(expr);
  }

  inline ExprNode<exprType, T_element>& getOperand()
  {
    return *expr;
  }

  inline const ExprNode<exprType, T_element>& getOperand() const
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

namespace
{
  template<typename exprType, typename T_element>
  class GetNegateSize
  {
  public:
    Maybe<double> operator()(const ExprNode<exprType, T_element>& e) const
    {
      const boost::array<std::size_t, ExprDimensions<exprType>::dims> dimensions(e.getDims());
      return std::accumulate(dimensions.begin(), dimensions.end(), 1.0, std::multiplies<double>());
    }
  };

  template<typename T_element>
  class GetNegateSize<matrix, T_element>
  {
  public:
    Maybe<double> operator()(const ExprNode<matrix, T_element>& e)
    {
      return Maybe<double>(e.nnz());
    }
  };
}

template<typename exprType, typename T_element>
class Negate : public UnOp<exprType, exprType, T_element>
{
public:
  Negate(ExprNode<exprType, T_element>& e) : UnOp<exprType, exprType, T_element>(e.getDims(), e)
  {
  }

  virtual void accept(ExpressionNodeVisitor<T_element>& visitor)
  {
    visitor.visit(*this);
  }

  virtual Maybe<double> getFlops() const
  {
    return GetNegateSize<exprType, T_element>()(this->getOperand());
  }
};

template<typename T_element>
class Absolute : public UnOp<scalar, scalar, T_element>
{
public:
  Absolute(ExprNode<scalar, T_element>& e) : UnOp<scalar, scalar, T_element>(e.getDims(), e)
  {
  }

  virtual void accept(ExpressionNodeVisitor<T_element>& visitor)
  {
    visitor.visit(*this);
  }

  virtual void internal_evaluate()
  {
    this->getOperand().evaluate();
  }

  virtual T_element getElementValue()
  {
    return std::abs(this->getOperand().getElementValue());
  }

  virtual Maybe<double> getFlops() const
  {
    return 1.0;
  }
};

template<typename T_element>
class SquareRoot : public UnOp<scalar, scalar, T_element>
{
public:
  SquareRoot(ExprNode<scalar, T_element>& e) : UnOp<scalar, scalar, T_element>(e.getDims(), e)
  {
  }

  virtual void accept(ExpressionNodeVisitor<T_element>& visitor)
  { 
    visitor.visit(*this);
  }
		      
  virtual void internal_evaluate()
  { 
    this->getOperand().evaluate();
  }
		        
  virtual T_element getElementValue()
  { 
    return std::sqrt(this->getOperand().getElementValue());
  }

  virtual Maybe<double> getFlops() const
  {
    return 1.0;
  }
};

}

}
#endif
