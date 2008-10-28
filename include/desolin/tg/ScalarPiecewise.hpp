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

#ifndef DESOLIN_TG_SCALAR_PIECEWISE_HPP
#define DESOLIN_TG_SCALAR_PIECEWISE_HPP

#include <desolin/tg/Desolin_tg_fwd.hpp>

namespace desolin
{

namespace detail
{

template<typename exprType, typename T_element>
class TGScalarPiecewise : public TGBinOp<exprType, exprType, tg_scalar, T_element>
{
private:
  const TGScalarPiecewiseOp op;

public:  
  TGScalarPiecewise(typename TGInternalType<exprType, T_element>::type* internal,
		  const TGScalarPiecewiseOp o,
		  const TGOutputReference<exprType, T_element>& operand,
		  const TGOutputReference<tg_scalar, T_element>& s) : TGBinOp<exprType, exprType, tg_scalar, T_element>(internal, operand, s), op(o)
  {
  }

  bool isEqual(const TGScalarPiecewise& node, const std::map<const TGExpressionNode<T_element>*, const TGExpressionNode<T_element>*>& mappings) const
  {
    return  TGBinOp<exprType, exprType, tg_scalar, T_element>::isEqual(node, mappings) &&
	    op == node.op;
  }

  const TGScalarPiecewiseOp getOperation() const
  {
    return op;
  }

  void accept(TGExpressionNodeVisitor<T_element>& v)
  {
    v.visit(*this);
  }
};

}

}
#endif
