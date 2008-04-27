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

#ifndef DESOLIN_PROFILING_SCALAR_PIECEWISE_HPP
#define DESOLIN_PROFILING_SCALAR_PIECEWISE_HPP

#include "Desolin_profiling_fwd.hpp"
#include <map>

namespace desolin
{

namespace detail
{

template<typename exprType, typename T_element>
class PScalarPiecewise : public PBinOp<exprType, exprType, scalar, T_element>
{
private:
  const ScalarPiecewiseOp op;

public:
  PScalarPiecewise(const ScalarPiecewiseOp o, PExprNode<exprType, T_element>& l, PExprNode<scalar, T_element>& r) : PBinOp<exprType, exprType, scalar, T_element>(l, r), op(o)
  {
  }

  bool isEqual(const PScalarPiecewise& node, const std::map<const PExpressionNode<T_element>*, const PExpressionNode<T_element>*>& mappings) const
  {
    return PBinOp<exprType, exprType, scalar, T_element>::isEqual(node, mappings) &&
	   op == node.op;
  }

  const ScalarPiecewiseOp getOperation() const
  {
    return op;
  }

  virtual void accept(PExpressionNodeVisitor<T_element>& visitor)
  {
    visitor.visit(*this);
  } 
};

}

}
#endif
