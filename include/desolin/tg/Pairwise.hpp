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

#ifndef DESOLIN_TG_PAIRWISE_HPP
#define DESOLIN_TG_PAIRWISE_HPP

#include <desolin/tg/Desolin_tg_fwd.hpp>
#include <map>

namespace desolin
{

namespace detail
{

template<TGExprType exprType, typename T_element>
class TGPairwise : public TGBinOp<exprType, exprType, exprType, T_element>
{
private:
  const TGPairwiseOp op;

public:  
  TGPairwise(typename TGInternalType<exprType, T_element>::type* internal, 
		  const TGPairwiseOp o,
		  TGExprNode<exprType, T_element>& l, 
		  TGExprNode<exprType, T_element>& r) : TGBinOp<exprType, exprType, exprType, T_element>(internal, l, r), op(o)
  {
  }


  virtual void accept(TGExpressionNodeVisitor<T_element>& v)
  {
    v.visit(*this);
  }

  inline bool isEqual(const TGPairwise& node, const std::map<const TGExpressionNode<T_element>*, const TGExpressionNode<T_element>*>& mappings) const
  {
    return  TGBinOp<exprType, exprType, exprType, T_element>::isEqual(node, mappings) &&
	    op == node.op;
  } 

  const TGPairwiseOp getOperation() const
  {
    return op;
  }
};

}

}
#endif
