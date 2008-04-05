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

#ifndef DESOLIN_PROFILNG_BINOP_HPP
#define DESOLIN_PROFILNG_BINOP_HPP

#include "Desolin_profiling_fwd.hpp"
#include <map>
#include <cassert>

namespace desolin
{

namespace detail
{

template<ExprType resultType, ExprType leftType, ExprType rightType, typename T_element>
class PBinOp : public PExprNode<resultType, T_element>
{
private:
  PExprNode<leftType, T_element>* left;
  PExprNode<rightType, T_element>* right;

public:
  bool isEqual(const PBinOp& node, const std::map<const PExpressionNode<T_element>*, const PExpressionNode<T_element>*>& mappings) const
  {
    assert(mappings.find(left) != mappings.end());
    assert(mappings.find(right) != mappings.end());

    return PExprNode<resultType, T_element>::isEqual(node, mappings) &&
	   mappings.find(left)->second == node.left &&
	   mappings.find(right)->second == node.right;
  }

  PBinOp(PExprNode<leftType, T_element>& l, PExprNode<rightType, T_element>& r) : PExprNode<resultType, T_element>(), left(&l), right(&r)
  {
  }

  inline PExprNode<leftType, T_element>& getLeft()
  {
    return *left;
  }

  inline const PExprNode<leftType, T_element>& getLeft() const
  {
    return *left;
  }

  inline PExprNode<rightType, T_element>& getRight()
  {
    return *right;
  }

  inline const PExprNode<rightType, T_element>& getRight() const
  {
    return *right;
  }  
};

}

}

#endif

