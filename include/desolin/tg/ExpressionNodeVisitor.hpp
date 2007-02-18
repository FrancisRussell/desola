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

#ifndef DESOLIN_TG_EXPRESSION_NODE_VISITOR_HPP
#define DESOLIN_TG_EXPRESSION_NODE_VISITOR_HPP

#include <desolin/tg/Desolin_tg_fwd.hpp>

namespace desolin
{

namespace detail
{

template<typename T_element>
class TGExpressionNodeVisitor
{
public:
  virtual void visit(TGElementGet<tg_vector, T_element>& e) = 0;
  virtual void visit(TGElementGet<tg_matrix, T_element>& e) = 0;

  virtual void visit(TGElementSet<tg_vector, T_element>& e) = 0;
  virtual void visit(TGElementSet<tg_matrix, T_element>& e) = 0;

  virtual void visit(TGLiteral<tg_scalar, T_element>& e)=0;
  virtual void visit(TGLiteral<tg_vector, T_element>& e)=0;
  virtual void visit(TGLiteral<tg_matrix, T_element>& e)=0;

  virtual void visit(TGMatrixMult<T_element>& e)=0;
  virtual void visit(TGMatrixVectorMult<T_element>& e)=0;
  virtual void visit(TGTransposeMatrixVectorMult<T_element>& e)=0;
  virtual void visit(TGVectorDot<T_element>& e)=0;
  virtual void visit(TGVectorCross<T_element>& e)=0;
  virtual void visit(TGVectorNorm<T_element>& e)=0;
  virtual void visit(TGMatrixTranspose<T_element>& e)=0;

  virtual void visit(TGPairwise<tg_scalar, T_element>& e) = 0;
  virtual void visit(TGPairwise<tg_vector, T_element>& e) = 0;
  virtual void visit(TGPairwise<tg_matrix, T_element>& e) = 0;

  virtual void visit(TGScalarPiecewise<tg_scalar, T_element>& e)=0;
  virtual void visit(TGScalarPiecewise<tg_vector, T_element>& e)=0;
  virtual void visit(TGScalarPiecewise<tg_matrix, T_element>& e)=0;

  virtual void visit(TGNegate<tg_scalar, T_element>& e)=0;
  virtual void visit(TGNegate<tg_vector, T_element>& e)=0;
  virtual void visit(TGNegate<tg_matrix, T_element>& e)=0;

  virtual void visit(TGAbsolute<T_element>& e)=0;
  virtual void visit(TGSquareRoot<T_element>& e)=0;
 
  virtual ~TGExpressionNodeVisitor() {}
};

}

}
#endif
