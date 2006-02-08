#ifndef DESOLIN_HPP
#define DESOLIN_HPP

// Uncomment next line to disable asserts
// #define NDEBUG

// Boost STL compliant array with partial specialisation for size 0
// See http://www.josuttis.com/cppcode
#include <desolin/boost_array.hpp>

#include <desolin/Exceptions.hpp>
#include <desolin/Traits.hpp>
#include <desolin/ExpressionNode.hpp>
#include <desolin/ExprNode.hpp>
#include <desolin/UnOp.hpp>
#include <desolin/BinOp.hpp>
#include <desolin/Elemental.hpp>
#include <desolin/Pairwise.hpp>
#include <desolin/ScalarPiecewise.hpp>
#include <desolin/MatrixVector.hpp>
#include <desolin/InternalReps.hpp>
#include <desolin/Literal.hpp>
#include <desolin/ExpressionNodeVisitor.hpp>
#include <desolin/ExpressionGraph.hpp>
#include <desolin/EvaluationStrategy.hpp>
#include <desolin/Evaluator.hpp>
#include <desolin/NullEvaluator.hpp>
#include <desolin/Variable.hpp>
#include <desolin/Scalar.hpp>
#include <desolin/Vector.hpp>
#include <desolin/Matrix.hpp>
#include <desolin/Printing.hpp>

#include <desolin/tg/Desolin_tg.hpp>

#include <desolin/harwell-boeing/mtl_harwell_boeing_stream.hpp>

#endif
