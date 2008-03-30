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

#ifndef DESOLIN_PAIRWISE_HPP
#define DESOLIN_PAIRWISE_HPP

#include <numeric>
#include <functional>
#include <boost/array.hpp>
#include <desolin/Desolin_fwd.hpp>

namespace desolin
{

namespace detail
{

template<ExprType exprType, typename T_element>
class Pairwise : public BinOp<exprType, exprType, exprType, T_element>
{
private:
  const PairwiseOp operation;
  
  static const boost::array<int, ExprDimensions<exprType>::dims> createDims(const ExprNode<exprType, T_element>& l, const ExprNode<exprType, T_element>& r)
  {
    return l.getDims();
  }
public:  
  Pairwise(const PairwiseOp op, ExprNode<exprType, T_element>& l, ExprNode<exprType, T_element>& r) : BinOp<exprType, exprType, exprType, T_element>(createDims(l, r), l, r), operation(op)
  {
  }

  PairwiseOp getOperation() const
  {
    return operation;
  }

  void accept(ExpressionNodeVisitor<T_element>& v)
  {
    v.visit(*this);
  }

  virtual double getFlops() const
  {
    return std::accumulate(this->dimensions.begin(), this->dimensions.end(), 1.0, std::multiplies<double>());
  }
};

}

}
#endif
