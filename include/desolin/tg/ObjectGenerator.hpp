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

#ifndef DESOLIN_TG_OBJECT_GENERATOR_HPP
#define DESOLIN_TG_OBJECT_GENERATOR_HPP

#include <map>
#include <cassert>
#include <TaskGraph>
#include <desolin/tg/Desolin_tg_fwd.hpp>

namespace desolin
{

namespace detail
{

template<typename T_element>
class TGObjectGenerator : public ExpressionNodeVisitor<T_element>
{
private:
  TGObjectGenerator(const TGObjectGenerator&);
  TGObjectGenerator& operator=(const TGObjectGenerator&);

  TGEvaluator<T_element>& evaluator;
  TGObjectGeneratorHelper<scalar, T_element> scalarHandler;
  TGObjectGeneratorHelper<vector, T_element> vectorHandler;
  TGObjectGeneratorHelper<matrix, T_element> matrixHandler;
  
  static TGScalarPiecewiseOp getTGScalarPiecewiseOp(const ScalarPiecewiseOp op)
  {
    switch(op)
    {
      case multiply: return tg_multiply;
      case divide: return tg_divide;
      case assign: return tg_assign;
      default: throw TGInvalidOperationError("Unrecognised DesoLin ScalarPiecewise Operation");
    }
  }

  static TGPairwiseOp getTGPairwiseOp(const PairwiseOp op)
  {
    switch(op)
    {
      case pair_add: return tg_pair_add;
      case pair_sub: return tg_pair_sub;
      case pair_mul: return tg_pair_mul;
      case pair_div: return tg_pair_div;
      default: throw TGInvalidOperationError("Unrecognised DesoLin Pairwise Operation");
    }
  }

public:
  TGObjectGenerator(TGEvaluator<T_element>& e) : evaluator(e), scalarHandler(evaluator), vectorHandler(evaluator), matrixHandler(evaluator)
  {
  }

  void addTaskGraphMappings(ParameterHolder& parameterHolder)
  {
    scalarHandler.addTaskGraphMappings(parameterHolder);
    vectorHandler.addTaskGraphMappings(parameterHolder);
    matrixHandler.addTaskGraphMappings(parameterHolder);
  }

  void visit(ElementGet<vector, T_element>& e)
  {
    TGScalar<T_element>* internal = scalarHandler.createTGRep(e);
    TGExprNode<tg_vector, T_element>& v = vectorHandler.getTGExprNode(e.getOperand());
    TGElementIndex<tg_vector> index(e.getIndex());
    scalarHandler.handleNode(e, new TGElementGet<tg_vector, T_element>(internal, v, index));
  }

  void visit(ElementGet<matrix, T_element>& e)
  {
    TGScalar<T_element>* internal = scalarHandler.createTGRep(e);
    TGExprNode<tg_matrix, T_element>& m = matrixHandler.getTGExprNode(e.getOperand());
    TGElementIndex<tg_matrix> index(e.getIndex());
    scalarHandler.handleNode(e, new TGElementGet<tg_matrix, T_element>(internal, m, index));
  }

  void visit(ElementSet<vector, T_element>& e)
  {
    TGVector<T_element>* internal = vectorHandler.createTGRep(e);
    TGExprNode<tg_vector, T_element>& v = vectorHandler.getTGExprNode(e.getOperand());
    
    const std::map<ElementIndex<vector>, ExprNode<scalar, T_element>*> assignments(e.getAssignments());
    std::map<TGElementIndex<tg_vector>, TGExprNode<tg_scalar, T_element>*> tgAssignments;
    
    typedef typename std::map<ElementIndex<vector>, ExprNode<scalar, T_element>*>::const_iterator Iterator;
    for(Iterator i = assignments.begin(); i != assignments.end(); ++i)
    {
      const TGElementIndex<tg_vector> index(i->first);
      TGExprNode<tg_scalar, T_element>& s = scalarHandler.getTGExprNode(*(i->second));
      tgAssignments[index] = &s;
    }
    vectorHandler.handleNode(e, new TGElementSet<tg_vector, T_element>(internal, v, tgAssignments));
  }

  void visit(ElementSet<matrix, T_element>& e)
  {
    TGMatrix<T_element>* internal = matrixHandler.createTGRep(e);
    TGExprNode<tg_matrix, T_element>& v = matrixHandler.getTGExprNode(e.getOperand());
		        
    const std::map<ElementIndex<matrix>, ExprNode<scalar, T_element>*> assignments(e.getAssignments());
    std::map<TGElementIndex<tg_matrix>, TGExprNode<tg_scalar, T_element>*> tgAssignments;
		        
    typedef typename std::map<ElementIndex<matrix>, ExprNode<scalar, T_element>*>::const_iterator Iterator;
    for(Iterator i = assignments.begin(); i != assignments.end(); ++i)
    {
      const TGElementIndex<tg_matrix> index(i->first);
      TGExprNode<tg_scalar, T_element>& s = scalarHandler.getTGExprNode(*(i->second));
      tgAssignments[index] = &s;
    }
    matrixHandler.handleNode(e, new TGElementSet<tg_matrix, T_element>(internal, v, tgAssignments));
  }
    
  void visit(Literal<scalar, T_element>& e)
  {
    assert(false);
  }

  void visit(Literal<vector, T_element>& e)
  {
    assert(false);
  }

  void visit(Literal<matrix, T_element>& e)
  {
    assert(false);
  }

  void visit(MatrixMult<T_element>& e)
  {
    TGMatrix<T_element>* internal = matrixHandler.createTGRep(e);
    TGExprNode<tg_matrix, T_element>& left = matrixHandler.getTGExprNode(e.getLeft());
    TGExprNode<tg_matrix, T_element>& right = matrixHandler.getTGExprNode(e.getRight());
    matrixHandler.handleNode(e, new TGMatrixMult<T_element>(internal, left, right));
  }
  
  void visit(MatrixVectorMult<T_element>& e)
  { 
    TGVector<T_element>* internal = vectorHandler.createTGRep(e);
    TGExprNode<tg_matrix, T_element>& left = matrixHandler.getTGExprNode(e.getLeft());
    TGExprNode<tg_vector, T_element>& right = vectorHandler.getTGExprNode(e.getRight());
    vectorHandler.handleNode(e, new TGMatrixVectorMult<T_element>(internal, left, right));
  }

  void visit(TransposeMatrixVectorMult<T_element>& e)
  {
    TGVector<T_element>* internal = vectorHandler.createTGRep(e);
    TGExprNode<tg_matrix, T_element>& left = matrixHandler.getTGExprNode(e.getLeft());
    TGExprNode<tg_vector, T_element>& right = vectorHandler.getTGExprNode(e.getRight());
    vectorHandler.handleNode(e, new TGTransposeMatrixVectorMult<T_element>(internal, left, right));		
  }
  
  void visit(VectorDot<T_element>& e)
  { 
    TGScalar<T_element>* internal = scalarHandler.createTGRep(e);
    TGExprNode<tg_vector, T_element>& left = vectorHandler.getTGExprNode(e.getLeft());
    TGExprNode<tg_vector, T_element>& right = vectorHandler.getTGExprNode(e.getRight());
    scalarHandler.handleNode(e, new TGVectorDot<T_element>(internal, left, right));
  }
  
  void visit(VectorCross<T_element>& e)
  {
    TGVector<T_element>* internal = vectorHandler.createTGRep(e);
    TGExprNode<tg_vector, T_element>& left = vectorHandler.getTGExprNode(e.getLeft());
    TGExprNode<tg_vector, T_element>& right = vectorHandler.getTGExprNode(e.getRight());
    vectorHandler.handleNode(e, new TGVectorCross<T_element>(internal, left, right));		
  }

  void visit(VectorTwoNorm<T_element>& e)
  {
    TGScalar<T_element>* internal = scalarHandler.createTGRep(e);
    TGExprNode<tg_vector, T_element>& v = vectorHandler.getTGExprNode(e.getOperand());
    scalarHandler.handleNode(e, new TGVectorTwoNorm<T_element>(internal, v));
  }
  
  void visit(MatrixTranspose<T_element>& e)
  {
    TGMatrix<T_element>* internal = matrixHandler.createTGRep(e);
    TGExprNode<tg_matrix, T_element>& m = matrixHandler.getTGExprNode(e.getOperand());
    matrixHandler.handleNode(e, new TGMatrixTranspose<T_element>(internal, m));
  }

  void visit(Pairwise<scalar, T_element>& e)
  {
    TGPairwiseOp op = getTGPairwiseOp(e.getOperation()); 
    TGScalar<T_element>* internal = scalarHandler.createTGRep(e);
    TGExprNode<tg_scalar, T_element>& left = scalarHandler.getTGExprNode(e.getLeft());
    TGExprNode<tg_scalar, T_element>& right = scalarHandler.getTGExprNode(e.getRight());
    scalarHandler.handleNode(e, new TGPairwise<tg_scalar, T_element>(internal, op, left, right));
  }

  void visit(Pairwise<vector, T_element>& e)
  {
    TGPairwiseOp op = getTGPairwiseOp(e.getOperation());
    TGVector<T_element>* internal = vectorHandler.createTGRep(e);
    TGExprNode<tg_vector, T_element>& left = vectorHandler.getTGExprNode(e.getLeft());
    TGExprNode<tg_vector, T_element>& right = vectorHandler.getTGExprNode(e.getRight());
    vectorHandler.handleNode(e, new TGPairwise<tg_vector, T_element>(internal, op, left, right));
  }

  void visit(Pairwise<matrix, T_element>& e)
  {
    TGPairwiseOp op = getTGPairwiseOp(e.getOperation());
    TGMatrix<T_element>* internal = matrixHandler.createTGRep(e);
    TGExprNode<tg_matrix, T_element>& left = matrixHandler.getTGExprNode(e.getLeft());
    TGExprNode<tg_matrix, T_element>& right = matrixHandler.getTGExprNode(e.getRight());
    matrixHandler.handleNode(e, new TGPairwise<tg_matrix, T_element>(internal, op, left, right));
  }

  void visit(ScalarPiecewise<scalar, T_element>& e)
  {
    TGScalarPiecewiseOp op = getTGScalarPiecewiseOp(e.getOperation());
    TGScalar<T_element>* internal = scalarHandler.createTGRep(e);
    TGExprNode<tg_scalar, T_element>& left = scalarHandler.getTGExprNode(e.getLeft());
    TGExprNode<tg_scalar, T_element>& right = scalarHandler.getTGExprNode(e.getRight());
    scalarHandler.handleNode(e, new TGScalarPiecewise<tg_scalar, T_element>(internal, op, left, right));
  }

  void visit(ScalarPiecewise<vector, T_element>& e)
  {
    TGScalarPiecewiseOp op = getTGScalarPiecewiseOp(e.getOperation());
    TGVector<T_element>* internal = vectorHandler.createTGRep(e);
    TGExprNode<tg_vector, T_element>& left = vectorHandler.getTGExprNode(e.getLeft());
    TGExprNode<tg_scalar, T_element>& right = scalarHandler.getTGExprNode(e.getRight());
    vectorHandler.handleNode(e, new TGScalarPiecewise<tg_vector, T_element>(internal, op, left, right));
  }

  void visit(ScalarPiecewise<matrix, T_element>& e)
  {
    TGScalarPiecewiseOp op = getTGScalarPiecewiseOp(e.getOperation());
    TGMatrix<T_element>* internal = matrixHandler.createTGRep(e);
    TGExprNode<tg_matrix, T_element>& left = matrixHandler.getTGExprNode(e.getLeft());
    TGExprNode<tg_scalar, T_element>& right = scalarHandler.getTGExprNode(e.getRight());
    matrixHandler.handleNode(e, new TGScalarPiecewise<tg_matrix, T_element>(internal, op, left, right));
  }

  void visit(Negate<scalar, T_element>& e)
  {
    TGScalar<T_element>* internal = scalarHandler.createTGRep(e); 
    TGExprNode<tg_scalar, T_element>& operand = scalarHandler.getTGExprNode(e.getOperand());
    scalarHandler.handleNode(e, new TGNegate<tg_scalar, T_element>(internal, operand));
  }

  void visit(Negate<vector, T_element>& e)
  {
    TGVector<T_element>* internal = vectorHandler.createTGRep(e);
    TGExprNode<tg_vector, T_element>& operand = vectorHandler.getTGExprNode(e.getOperand());
    vectorHandler.handleNode(e, new TGNegate<tg_vector, T_element>(internal, operand));
  }

  void visit(Negate<matrix, T_element>& e)
  {
    TGMatrix<T_element>* internal = matrixHandler.createTGRep(e);
    TGExprNode<tg_matrix, T_element>& operand = matrixHandler.getTGExprNode(e.getOperand());
    matrixHandler.handleNode(e, new TGNegate<tg_matrix, T_element>(internal, operand));
  }

  void visit(Absolute<T_element>& e)
  {
    TGScalar<T_element>* internal = scalarHandler.createTGRep(e);
    TGExprNode<tg_scalar, T_element>& operand = scalarHandler.getTGExprNode(e.getOperand());
    scalarHandler.handleNode(e, new TGAbsolute<T_element>(internal, operand));
  }

  void visit(SquareRoot<T_element>& e)
  {
    TGScalar<T_element>* internal = scalarHandler.createTGRep(e);
    TGExprNode<tg_scalar, T_element>& operand = scalarHandler.getTGExprNode(e.getOperand());
    scalarHandler.handleNode(e, new TGSquareRoot<T_element>(internal, operand));
  }
};

}

}
#endif
