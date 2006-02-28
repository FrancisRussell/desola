#ifndef DESOLIN_FWD_HPP
#define DESOLIN_FWD_HPP

// Enums used for template parameters

namespace desolin_internal
{
using namespace desolin;
	
enum ExprType
{
  scalar,
  vector,
  matrix
};

enum PairwiseOp
{
  pair_add,
  pair_sub,
  pair_mul,
  pair_div
};

enum ScalarPiecewiseOp
{
  multiply,
  divide,
  assign
};

// Common
template<ExprType exprType> class ElementIndex;
template<ExprType exprType, typename T_element> struct ExprTraits;

// Expression Tree
template<typename T_element> class ExpressionGraph;
template<typename T_element> class ExpressionNode;
template<ExprType exprType, typename T_element> class ExprNode;
template<ExprType resultType, ExprType exprType, typename T_element> class UnOp;
template<ExprType resultType, ExprType leftType, ExprType rightType, typename T_element> class BinOp;
template<ExprType exprType, typename T_element> class Literal;
template<ExprType exprType, typename T_element> class ElementGet;
template<ExprType exprType, typename T_element> class ElementSet;
template<ExprType exprType, typename T_element> class Pairwise;
template<ExprType exprType, typename T_element> class ScalarPiecewise;
template<typename T_element> class MatrixMult;
template<typename T_element> class MatrixVectorMult;
template<typename T_element> class TransposeMatrixVectorMult;
template<typename T_element> class VectorDot;
template<typename T_element> class VectorCross;
template<typename T_element> class VectorNorm;
template<typename T_element> class MatrixTranspose;
template<ExprType exprType, typename T_element> class Negate;
template<typename T_element> class Absolute;
template<typename T_element> class SquareRoot;

// Expression Tree Visitors
template<typename T_element> class ExpressionNodeVisitor;
template<typename T_element> class ExpressionNodeTypeVisitor;
template<typename T_element> class LiteralVisitor;

// Storage Representations
template<typename T_element> class InternalValue;
template<typename T_element> class InternalScalar;
template<typename T_element> class InternalVector;
template<typename T_element> class InternalMatrix;
template<typename T_elementType> class ConventionalScalar;
template<typename T_elementType> class ConventionalVector;
template<typename T_elementType> class ConventionalMatrix;

//Storage Representation Visitors
template<typename T_elementType> class InternalScalarVisitor;
template<typename T_elementType> class InternalVectorVisitor;
template<typename T_elementType> class InternalMatrixVisitor;

// Expression Tree Evaluation
template<typename T_element> class EvaluationStrategy;
template<typename T_element> class LiteralReplacer;
template<typename T_element> class Evaluator;
template<typename T_element> class EvaluatorFactory;
template<typename T_element> class NullEvaluator;
template<typename T_element> class NullEvaluatorFactory;

// External Interface
template<typename T_element> class Variable;
template<ExprType expressionType, typename T_element> class Var;
}

namespace desolin
{	
// External Interface
template<typename T_element> class Vector;
template<typename T_element> class Matrix;
template<typename T_element> class Scalar;
template<desolin_internal::ExprType exprType, typename T_element> class ScalarElement;
}

#include <desolin/file-access/mtl_harwell_boeing_stream.hpp>
#include <desolin/file-access/mtl_matrix_market_stream.hpp>

#endif
