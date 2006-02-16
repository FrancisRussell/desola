#ifndef DESOLIN_TG_FWD_HPP
#define DESOLIN_TG_FWD_HPP

#include <desolin/Desolin_fwd.hpp>

namespace desolin_internal
{
// Enums used for template parameters
enum TGExprType
{
  tg_scalar,
  tg_vector,
  tg_matrix
};

enum TGPairwiseOp
{
  tg_pair_add,
  tg_pair_sub,
  tg_pair_mul,
  tg_pair_div
};

enum TGScalarPiecewiseOp
{
  tg_multiply,
  tg_divide,
  tg_assign
};

// Common
template<ExprType exprType, typename T_element> struct ExprTGTraits;
template<TGExprType exprType, typename T_elementType> class TGInternalType;
template<TGExprType exprType> class TGElementIndex;

// TaskGraph Evaluator Expression Tree
template<typename T_element> class TGExpressionNode;
template<TGExprType resultType, TGExprType leftType, TGExprType rightType, typename T_element> class TGBinOp;
template<TGExprType resultType, TGExprType exprType, typename T_element> class TGUnOp;
template<typename T_element> class TGExpressionNodeVisitor;
template<TGExprType exprType, typename T_element> class TGElementGet;
template<TGExprType exprType, typename T_element> class TGElementSet;
template<TGExprType exprType, typename T_element> class TGLiteral;
template<typename T_element> class TGMatrixMult;
template<typename T_element> class TGMatrixVectorMult;
template<typename T_element> class TGVectorDot;
template<typename T_element> class TGVectorCross;
template<typename T_element> class TGVectorNorm;
template<typename T_element> class TGMatrixTranspose;
template<TGExprType exprType, typename T_element> class TGPairwise;
template<TGExprType exprType, typename T_element> class TGScalarPiecewise;
template<TGExprType exprType, typename T_element> class TGNegate;
template<typename T_element> class TGAbsolute;
template<typename T_element> class TGSquareRoot;

// TaskGraph Evaluator Expression Tree Evaluation
template<typename T_element> class TGCodeGenerator;
template<typename T_element> class TGEvaluator;
template<typename T_element> class TGEvaluatorFactory;
template<typename T_element> class TGExpressionGraph;
template<typename T_element> class TGObjectGenerator;
template<ExprType exprType, typename T_element> class TGObjectGeneratorHelper;

// TaskGraph Evaluator Expression Manipulation Objects and Storage Representation
template<typename T_elementType> class TGScalar;
template<typename T_elementType> class TGVector;
template<typename T_elementType> class TGMatrix;
template<typename T_elementType> class TGScalarExpr;
template<typename T_elementType> class TGConventionalScalar;
template<typename T_elementType> class TGConventionalVector;
template<typename T_elementType> class TGConventionalMatrix;

// Creates TaskGraph Evaluator Storage Representations from Generic Representations
template<typename T_elementType> class TGScalarGen;
template<typename T_elementType> class TGVectorGen;
template<typename T_elementType> class TGMatrixGen;
}

#include <desolin/tg/NameGenerator.hpp>
#include <desolin/tg/ParameterHolder.hpp>
#include <desolin/tg/Exceptions.hpp>

#endif
