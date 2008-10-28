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

#ifndef DESOLIN_TG_FWD_HPP
#define DESOLIN_TG_FWD_HPP

namespace desolin
{

namespace detail
{

// Types used for template parameters

struct tg_scalar {};
struct tg_vector {};
struct tg_matrix {};

enum TGPairwiseOp
{
  tg_pair_add,
  tg_pair_sub,
  tg_pair_mul,
  tg_pair_div
};

enum TGScalarPiecewiseOp
{
  tg_piecewise_multiply,
  tg_piecewise_divide,
  tg_piecewise_assign
};

// Common
class NameGenerator;
class ParameterHolder;
class TGInvalidOperationError;

template<typename exprType, typename T_element> struct ExprTGTraits;
template<typename exprType, typename T_elementType> class TGInternalType;
template<typename exprType> class TGElementIndex;

// TaskGraph Evaluator Expression Tree
template<typename tgExprType, typename T_element> class TGOutputReference;
template<typename T_element> class TGExpressionNode;
template<typename exprType, typename T_element> class TGExprNode;
template<typename resultType, typename leftType, typename rightType, typename T_element> class TGBinOp;
template<typename resultType, typename exprType, typename T_element> class TGUnOp;
template<typename T_element> class TGExpressionNodeVisitor;
template<typename T_element> class TGHashingVisitor;
template<typename T_element> class TGEqualityCheckingVisitor;
template<typename exprType, typename T_element> class TGElementGet;
template<typename exprType, typename T_element> class TGElementSet;
template<typename exprType, typename T_element> class TGLiteral;
template<typename T_element> class TGMatrixMult;
template<typename T_element> class TGMatrixVectorMult;
template<typename T_element> class TGVectorDot;
template<typename T_element> class TGVectorCross;
template<typename T_element> class TGVectorTwoNorm;
template<typename T_element> class TGMatrixTranspose;
template<typename exprType, typename T_element> class TGPairwise;
template<typename exprType, typename T_element> class TGScalarPiecewise;
template<typename exprType, typename T_element> class TGNegate;
template<typename T_element> class TGAbsolute;
template<typename T_element> class TGSquareRoot;

// TaskGraph Evaluator Expression Tree Evaluation
template<typename T_element> class TGCodeGenerator;
template<typename T_element> class TGEvaluator;
template<typename T_element> class TGEvaluatorFactory;
template<typename T_element> class TGExpressionGraph;
template<typename T_element> class TGObjectGenerator;
template<typename exprType, typename T_element> class TGObjectGeneratorHelper;

// TaskGraph Evaluator Expression Manipulation Objects and Storage Representation
template<typename T_elementType> class TGScalar;
template<typename T_elementType> class TGVector;
template<typename T_elementType> class TGMatrix;
template<typename T_elementType> class TGScalarExpr;
template<typename T_elementType> class TGConventionalScalar;
template<typename T_elementType> class TGConventionalVector;
template<typename T_elementType> class TGConventionalMatrix;
template<typename T_elementType> class TGCRSMatrix;

// Creates TaskGraph Evaluator Storage Representations from Generic Representations
template<typename T_elementType> class TGScalarGen;
template<typename T_elementType> class TGVectorGen;
template<typename T_elementType> class TGMatrixGen;

// Wrappers for TaskGraph scalars and arrays to allow delayed instantiation
template<typename T> class ScalarVariableWrapper;
template<typename T, unsigned dimensions> class TaskArrayWrapper;
}

}

#endif
