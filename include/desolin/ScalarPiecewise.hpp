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

#ifndef DESOLIN_SCALAR_PIECEWISE
#define DESOLIN_SCALAR_PIECEWISE

#include <desolin/Desolin_fwd.hpp>

namespace desolin
{

namespace detail
{

template<ExprType exprType, typename T_element>
class ScalarPiecewise : public BinOp<exprType, exprType, scalar, T_element>
{
private:
  const ScalarPiecewiseOp operation;

public:  
  ScalarPiecewise(const ScalarPiecewiseOp op, ExprNode<exprType, T_element>& o, ExprNode<scalar, T_element>& s) : BinOp<exprType, exprType, scalar, T_element>(o.getDims(), o, s), operation(op)
  {
  }

  ScalarPiecewiseOp getOperation() const
  {
    return operation;
  }

  void accept(ExpressionNodeVisitor<T_element>& v)
  {
    v.visit(*this);
  }
};

}

}
#endif
