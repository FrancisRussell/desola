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

#ifndef DESOLA_BINOP_HPP
#define DESOLA_BINOP_HPP

#include <desola/Desola_fwd.hpp>
#include <boost/array.hpp>
#include <cassert>
#include <cstddef>

namespace desola
{

namespace detail
{

template<typename resultType, typename leftType, typename rightType, typename T_element>
class BinOp : public ExprNode<resultType, T_element>
{
private:
  ExprNode<leftType, T_element>* left;
  ExprNode<rightType, T_element>* right;

  template<typename T_replacement>
  void updateImpl(ExprNode<T_replacement, T_element>& previous, ExprNode<T_replacement, T_element>& next)
  {
    assert(left != NULL);
    assert(right != NULL);

    const bool leftReplaced = ReplaceExprNode<leftType, T_replacement, T_element>()(left, &previous, &next);
    if (leftReplaced)
      this->replaceDependency(&previous, &next);

    const bool rightReplaced = ReplaceExprNode<rightType, T_replacement, T_element>()(right, &previous, &next);
    if(rightReplaced)
      this->replaceDependency(&previous, &next);
  }

public:
  BinOp(const boost::array<std::size_t, ExprDimensions<resultType>::dims> dims, ExprNode<leftType, T_element>& l, ExprNode<rightType, T_element>& r) : ExprNode<resultType, T_element>(dims), left(&l), right(&r)
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
