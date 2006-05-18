#ifndef DESOLIN_HPP
#define DESOLIN_HPP

// Uncomment next line to disable asserts
// #define NDEBUG

// Boost STL compliant array with partial specialisation for size 0
// See http://www.josuttis.com/cppcode
#include "boost_array.hpp"

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

#include "file-access/mtl_harwell_boeing_stream.hpp"
#include "file-access/mtl_matrix_market_stream.hpp"

#endif
