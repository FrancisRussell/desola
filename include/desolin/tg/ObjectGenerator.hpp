#ifndef DESOLIN_TG_OBJECT_GENERATOR_HPP
#define DESOLIN_TG_OBJECT_GENERATOR_HPP

#include <cassert>
#include <TaskGraph>
#include <desolin/tg/Desolin_tg_fwd.hpp>

namespace desolin_internal
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
  
  static TGScalarPiecewiseOp getTGScalarPiecewiseOp(ScalarPiecewiseOp op)
  {
    switch(op)
    {
      case multiply: return tg_multiply;
      case divide: return tg_divide;
      case assign: return tg_assign;
      default: throw TGInvalidOperationError("Unrecognised DesoLin ScalarPiecewise Operation");
    }
  }

  static TGPairwiseOp getTGPairwiseOp(PairwiseOp op)
  {
    switch(op)
    {
      case add: return tg_add;
      case sub: return tg_sub;
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
    TGExprNode<tg_vector, T_element>& v = vectorHandler.getTGExprNode(e.getLeft());
    TGExprNode<tg_scalar, T_element>& s = scalarHandler.getTGExprNode(e.getRight());
    TGElementIndex<tg_vector> index(e.getIndex());
    vectorHandler.handleNode(e, new TGElementSet<tg_vector, T_element>(internal, v, s, index));
  }

  void visit(ElementSet<matrix, T_element>& e)
  {
    TGMatrix<T_element>* internal = matrixHandler.createTGRep(e);
    TGExprNode<tg_matrix, T_element>& m = matrixHandler.getTGExprNode(e.getLeft());
    TGExprNode<tg_scalar, T_element>& s = scalarHandler.getTGExprNode(e.getRight());
    TGElementIndex<tg_matrix> index(e.getIndex());
    matrixHandler.handleNode(e, new TGElementSet<tg_matrix, T_element>(internal, m, s, index));
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

  void visit(VectorNorm<T_element>& e)
  {
    TGScalar<T_element>* internal = scalarHandler.createTGRep(e);
    TGExprNode<tg_vector, T_element>& v = vectorHandler.getTGExprNode(e.getOperand());
    scalarHandler.handleNode(e, new TGVectorNorm<T_element>(internal, v));
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
#endif
