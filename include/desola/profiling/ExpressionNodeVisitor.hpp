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

#ifndef DESOLA_PROFILING_EXPRESSION_NODE_VISITOR_HPP
#define DESOLA_PROFILING_EXPRESSION_NODE_VISITOR_HPP

#include "Desola_profiling_fwd.hpp"

namespace desola
{

namespace detail
{

template<typename T_element>
class PExpressionNodeVisitor
{
public:
  virtual void visit(PElementGet<vector, T_element>& e) = 0;
  virtual void visit(PElementGet<matrix, T_element>& e) = 0;

  virtual void visit(PElementSet<vector, T_element>& e) = 0;
  virtual void visit(PElementSet<matrix, T_element>& e) = 0;

  virtual void visit(PLiteral<scalar, T_element>& e) = 0;
  virtual void visit(PLiteral<vector, T_element>& e) = 0;
  virtual void visit(PLiteral<matrix, T_element>& e) = 0;

  virtual void visit(PMatrixMult<T_element>& e) = 0;
  virtual void visit(PMatrixVectorMult<T_element>& e) = 0;
  virtual void visit(PTransposeMatrixVectorMult<T_element>& e) = 0;
  virtual void visit(PVectorDot<T_element>& e) = 0;
  virtual void visit(PVectorCross<T_element>& e) = 0;
  virtual void visit(PVectorTwoNorm<T_element>& e) = 0;
  virtual void visit(PMatrixTranspose<T_element>& e) = 0;

  virtual void visit(PPairwise<scalar, T_element>& e) = 0;
  virtual void visit(PPairwise<vector, T_element>& e) = 0;
  virtual void visit(PPairwise<matrix, T_element>& e) = 0;

  virtual void visit(PScalarPiecewise<scalar, T_element>& e) = 0;
  virtual void visit(PScalarPiecewise<vector, T_element>& e) = 0;
  virtual void visit(PScalarPiecewise<matrix, T_element>& e) = 0;

  virtual void visit(PNegate<scalar, T_element>& e) = 0;
  virtual void visit(PNegate<vector, T_element>& e) = 0;
  virtual void visit(PNegate<matrix, T_element>& e) = 0;

  virtual void visit(PAbsolute<T_element>& e) = 0;
  virtual void visit(PSquareRoot<T_element>& e) = 0;

  virtual ~PExpressionNodeVisitor() {}
};

}

}
#endif
