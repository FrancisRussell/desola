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

#ifndef DESOLA_SCALAR_PIECEWISE
#define DESOLA_SCALAR_PIECEWISE

#include <boost/array.hpp>
#include <desola/Desola_fwd.hpp>

namespace desola
{

namespace detail
{

namespace
{
  template<typename exprType, typename T_element>
  class GetScalarPiecewiseSize
  {
  public:
    Maybe<double> operator()(const ExprNode<exprType, T_element>& e) const
    {
      const boost::array<std::size_t, ExprDimensions<exprType>::dims> dimensions(e.getDims());
      return std::accumulate(dimensions.begin(), dimensions.end(), 1.0, std::multiplies<double>());
    }
  };

  template<typename T_element>
  class GetScalarPiecewiseSize<matrix, T_element>
  {
  public:
    Maybe<double> getSize(const ExprNode<matrix, T_element>& e)
    {
      return Maybe<double>(e.nnz());
    }
  };
}

template<typename exprType, typename T_element>
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

  virtual Maybe<double> getFlops() const
  {
    return GetScalarPiecewiseSize<exprType, T_element>()(this->getLeft());
  }
};

}

}
#endif
