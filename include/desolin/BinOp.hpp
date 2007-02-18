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

#ifndef DESOLIN_BINOP_HPP
#define DESOLIN_BINOP_HPP

#include <desolin/Desolin_fwd.hpp>
#include <boost/array.hpp>
#include <boost/cast.hpp>
#include <cassert>

namespace desolin
{

namespace detail
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
      this->replaceDependency(&previous, &next);
      left = boost::polymorphic_downcast< ExprNode<leftType, T_element>* >(&next);
    }
    if(right == &previous)
    {
      this->replaceDependency(&previous, &next);
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

  inline const ExprNode<leftType, T_element>& getLeft() const
  {
    return *left;
  }
  
  inline ExprNode<rightType, T_element>& getRight()
  {
    return *right;
  }

  inline const ExprNode<rightType, T_element>& getRight() const
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

}
#endif
