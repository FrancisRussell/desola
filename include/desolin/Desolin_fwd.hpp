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

#ifndef DESOLIN_FWD_HPP
#define DESOLIN_FWD_HPP

namespace desolin
{

namespace detail
{

enum EvaluationDirective
{
  EVALUATE,
  NO_EVALUATE
};

// Types used for template parameters

// Expression Types
struct scalar {};
struct vector {};
struct matrix {};

enum PairwiseOp
{
  pair_add,
  pair_sub,
  pair_mul,
  pair_div
};

enum ScalarPiecewiseOp
{
  piecewise_multiply,
  piecewise_divide,
  piecewise_assign
};

// Common
template<typename exprType> class ElementIndex;
template<typename exprType, typename T_element> struct ExprTraits;

// Expression Tree
template<typename T_element> class ExpressionGraph;
template<typename T_element> class ExpressionNode;
template<typename exprType, typename T_element> class ExprNode;
template<typename resultType, typename exprType, typename T_element> class UnOp;
template<typename resultType, typename leftType, typename rightType, typename T_element> class BinOp;
template<typename exprType, typename T_element> class Literal;
template<typename exprType, typename T_element> class ElementGet;
template<typename exprType, typename T_element> class ElementSet;
template<typename exprType, typename T_element> class Pairwise;
template<typename exprType, typename T_element> class ScalarPiecewise;
template<typename T_element> class MatrixMult;
template<typename T_element> class MatrixVectorMult;
template<typename T_element> class TransposeMatrixVectorMult;
template<typename T_element> class VectorDot;
template<typename T_element> class VectorCross;
template<typename T_element> class VectorTwoNorm;
template<typename T_element> class MatrixTranspose;
template<typename exprType, typename T_element> class Negate;
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
template<typename T_elementType> class CRSMatrix;

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
template<typename expressionType, typename T_element> class Var;

// Caching
class Cache;

// Matrix IO
template <class T> struct entry1;
template <class T> struct entry2;
}

// Configuration and Statistics
class ConfigurationManager;
class StatisticsCollector;

// Matrix IO
template <class T> class harwell_boeing_stream;
template <class T> class matrix_market_stream;

// External Interface
template<typename T_element> class Vector;
template<typename T_element> class Matrix;
template<typename T_element> class Scalar;
template<typename exprType, typename T_element> class ScalarElement;


//Exceptions
class DesolinLogicError;
class DesolinRuntimeError;
class NullSizeError;
}

#endif
