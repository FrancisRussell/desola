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

#ifndef DESOLA_PAIRWISE_HPP
#define DESOLA_PAIRWISE_HPP

#include <numeric>
#include <cstddef>
#include <functional>
#include <boost/array.hpp>
#include <desola/Desola_fwd.hpp>

namespace desola
{

namespace detail
{

namespace
{
  template<typename exprType, typename T_element>
  class GetPairwiseFlops
  {
  public:
    Maybe<double> operator()(const ExprNode<exprType, T_element>& a, const ExprNode<exprType, T_element>& b) const
    {
      const boost::array<std::size_t, ExprDimensions<exprType>::dims> dimensions(a.getDims());
      const double result = std::accumulate(dimensions.begin(), dimensions.end(), 1.0, std::multiplies<double>());
      return Maybe<double>(result);
    }
  };

  template<typename T_element>
  class GetPairwiseFlops<matrix, T_element>
  {
  public:
    Maybe<double> operator()(const ExprNode<matrix, T_element>& a, const ExprNode<matrix, T_element>& b) const
    {
      return Maybe<double>(Nothing());
    }
  };
}

template<typename exprType, typename T_element>
class Pairwise : public BinOp<exprType, exprType, exprType, T_element>
{
private:
  const PairwiseOp operation;
  
  static const boost::array<std::size_t, ExprDimensions<exprType>::dims> createDims(const ExprNode<exprType, T_element>& l, const ExprNode<exprType, T_element>& r)
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

  virtual Maybe<double> getFlops() const
  {
    return GetPairwiseFlops<exprType, T_element>()(this->getLeft(), this->getRight());
  }
};

}

}
#endif
