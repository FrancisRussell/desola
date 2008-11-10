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

#ifndef DESOLIN_TG_BINOP_HPP
#define DESOLIN_TG_BINOP_HPP

#include <set>
#include <map>
#include <cassert>
#include <desolin/tg/Desolin_tg_fwd.hpp>

namespace desolin
{

namespace detail
{

template<typename resultType, typename leftType, typename rightType, typename T_element>
class TGBinOp : public TGExprNode<resultType, T_element>
{
private:
  TGOutputReference<leftType, T_element> left;
  TGOutputReference<rightType, T_element> right;

public:
  bool isEqual(const TGBinOp& node, const std::map<const TGExpressionNode<T_element>*, const TGExpressionNode<T_element>*>& mappings) const
  {
    return TGExprNode<resultType, T_element>::isEqual(node, mappings) && 
           TGExpressionNode<T_element>::isEqual(left, node.left, mappings) &&
           TGExpressionNode<T_element>::isEqual(right, node.right, mappings);
  }
  
  TGBinOp(typename TGInternalType<resultType, T_element>::type* internal, const TGOutputReference<leftType,T_element>& l, 
    const TGOutputReference<rightType, T_element>& r) : TGExprNode<resultType, T_element>(internal), left(l), right(r)
  {
  }

  inline TGOutputReference<leftType, T_element> getLeft()
  {
    return left;
  }

  inline const TGOutputReference<leftType, T_element> getLeft() const
  {
    return left;
  }

  inline TGOutputReference<rightType, T_element> getRight()
  {
    return right;
  }

  inline const TGOutputReference<rightType, T_element> getRight() const
  {
    return right;
  }

  virtual std::set<TGExpressionNode<T_element>*> getDependencies() const
  {
    TGExpressionNode<T_element>* exprPtrs[]  = {left.getExpressionNode(), right.getExpressionNode()};
    return std::set<TGExpressionNode<T_element>*>(exprPtrs, exprPtrs+2);
  }

  virtual void replaceDependency(const TGOutputReference<tg_scalar, T_element>& previous, TGOutputReference<tg_scalar, T_element>& next)
  {
    ReplaceOutputReference<leftType, tg_scalar, T_element>()(left, previous, next);
    ReplaceOutputReference<rightType, tg_scalar, T_element>()(right, previous, next);
  }

  virtual void replaceDependency(const TGOutputReference<tg_vector, T_element>& previous, TGOutputReference<tg_vector, T_element>& next)
  {
    ReplaceOutputReference<leftType, tg_vector, T_element>()(left, previous, next);
    ReplaceOutputReference<rightType, tg_vector, T_element>()(right, previous, next);
  }

  virtual void replaceDependency(const TGOutputReference<tg_matrix, T_element>& previous, TGOutputReference<tg_matrix, T_element>& next)
  {
    ReplaceOutputReference<leftType, tg_matrix, T_element>()(left, previous, next);
    ReplaceOutputReference<rightType, tg_matrix, T_element>()(right, previous, next);
  }
};

}

}
#endif
