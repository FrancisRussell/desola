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

#ifndef DESOLIN_HPP
#define DESOLIN_HPP

// Uncomment next line to disable asserts
// #define NDEBUG

#include "Maybe.hpp"
#include "Cache.hpp"
#include "ConfigurationManager.hpp"
#include "StatisticsCollector.hpp"
#include "Exceptions.hpp"
#include "Traits.hpp"
#include "ExpressionNode.hpp"
#include "ExprNode.hpp"
#include "UnOp.hpp"
#include "BinOp.hpp"
#include "Elemental.hpp"
#include "Pairwise.hpp"
#include "ScalarPiecewise.hpp"
#include "MatrixVector.hpp"
#include "InternalReps.hpp"
#include "RowLengthStatistics.hpp"
#include "Literal.hpp"
#include "ExpressionNodeVisitor.hpp"
#include "ExpressionGraph.hpp"
#include "EvaluationStrategy.hpp"
#include "Evaluator.hpp"
#include "NullEvaluator.hpp"
#include "Variable.hpp"
#include "Scalar.hpp"
#include "Vector.hpp"
#include "Matrix.hpp"
#include "Printing.hpp"

#include "tg/Desolin_tg.hpp"
#include "profiling/Desolin_profiling.hpp"

#include "file-access/mtl_entry.hpp"
#include "file-access/mtl_harwell_boeing_stream.hpp"
#include "file-access/mtl_matrix_market_stream.hpp"

#endif
