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

#ifndef DESOLIN_EXPRESSION_NODE_VISITOR_HPP
#define DESOLIN_EXPRESSION_NODE_VISITOR_HPP

#include <desolin/Desolin_fwd.hpp>

namespace desolin
{

namespace detail
{

template<typename T_element>
class ExpressionNodeVisitor
{
public:
  virtual void visit(Pairwise<scalar, T_element>& e)= 0;
  virtual void visit(Pairwise<vector, T_element>& e)= 0;
  virtual void visit(Pairwise<matrix, T_element>& e)= 0;
    
  virtual void visit(ScalarPiecewise<scalar, T_element>& e)= 0;
  virtual void visit(ScalarPiecewise<vector, T_element>& e)= 0;
  virtual void visit(ScalarPiecewise<matrix, T_element>& e)= 0;

  virtual void visit(MatrixMult<T_element>& e)= 0;
  virtual void visit(MatrixVectorMult<T_element>& e)= 0;
  virtual void visit(TransposeMatrixVectorMult<T_element>& e)=0;
  virtual void visit(VectorDot<T_element>& e)= 0;	
  virtual void visit(VectorCross<T_element>& e)= 0;
  virtual void visit(VectorNorm<T_element>& e)= 0;
  virtual void visit(MatrixTranspose<T_element>& e)= 0;

  virtual void visit(ElementGet<vector, T_element>& e)= 0;
  virtual void visit(ElementGet<matrix, T_element>& e)= 0;

  virtual void visit(ElementSet<vector, T_element>& e)= 0;
  virtual void visit(ElementSet<matrix, T_element>& e)= 0;

  virtual void visit(Literal<scalar, T_element>& e)= 0;
  virtual void visit(Literal<vector, T_element>& e)= 0;
  virtual void visit(Literal<matrix, T_element>& e)= 0;

  virtual void visit(Negate<scalar, T_element>& e)= 0;
  virtual void visit(Negate<vector, T_element>& e)= 0;
  virtual void visit(Negate<matrix, T_element>& e)= 0;

  virtual void visit(Absolute<T_element>& e)= 0;
  virtual void visit(SquareRoot<T_element>& e)= 0;
      
  virtual ~ExpressionNodeVisitor() {}
};

template<typename T_element>
class LiteralVisitor
{
public:
  virtual void visit(Literal<scalar, T_element>& e)= 0;
  virtual void visit(Literal<vector, T_element>& e)= 0;
  virtual void visit(Literal<matrix, T_element>& e)= 0;	

  virtual ~LiteralVisitor() {}
};

template<typename T_element>
class ExpressionNodeTypeVisitor
{
public:
  virtual void visit(ExprNode<scalar, T_element>& e)= 0;
  virtual void visit(ExprNode<vector, T_element>& e)= 0;
  virtual void visit(ExprNode<matrix, T_element>& e)= 0;

  virtual ~ExpressionNodeTypeVisitor() {}
};

}

}
#endif
