#ifndef DESOLIN_PROFILING_EXPRESSION_NODE_VISITOR_HPP
#define DESOLIN_PROFILING_EXPRESSION_NODE_VISITOR_HPP

#include "Desolin_profiling_fwd.hpp"

namespace desolin_internal
{

template<typename T_element>
class PExpressionNodeVisitor
{
public:
  virtual void visit(PElementGet<vector, T_element>& e) = 0;
  virtual void visit(PElementGet<matrix, T_element>& e) = 0;

  virtual void visit(PElementSet<vector, T_element>& e) = 0;
  virtual void visit(PElementSet<matrix, T_element>& e) = 0;

  virtual void visit(PLiteral<scalar, T_element>& e) = 0;
  virtual void visit(PLiteral<vector, T_element>& e) = 0;
  virtual void visit(PLiteral<matrix, T_element>& e) = 0;

  virtual void visit(PMatrixMult<T_element>& e) = 0;
  virtual void visit(PMatrixVectorMult<T_element>& e) = 0;
  virtual void visit(PTransposeMatrixVectorMult<T_element>& e) = 0;
  virtual void visit(PVectorDot<T_element>& e) = 0;
  virtual void visit(PVectorCross<T_element>& e) = 0;
  virtual void visit(PVectorNorm<T_element>& e) = 0;
  virtual void visit(PMatrixTranspose<T_element>& e) = 0;

  virtual void visit(PPairwise<scalar, T_element>& e) = 0;
  virtual void visit(PPairwise<vector, T_element>& e) = 0;
  virtual void visit(PPairwise<matrix, T_element>& e) = 0;

  virtual void visit(PScalarPiecewise<scalar, T_element>& e) = 0;
  virtual void visit(PScalarPiecewise<vector, T_element>& e) = 0;
  virtual void visit(PScalarPiecewise<matrix, T_element>& e) = 0;

  virtual void visit(PNegate<scalar, T_element>& e) = 0;
  virtual void visit(PNegate<vector, T_element>& e) = 0;
  virtual void visit(PNegate<matrix, T_element>& e) = 0;

  virtual void visit(PAbsolute<T_element>& e) = 0;
  virtual void visit(PSquareRoot<T_element>& e) = 0;

  virtual ~PExpressionNodeVisitor() {}
};

}

#endif
