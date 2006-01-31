#ifndef DESOLIN_TG_EXPRESSION_NODE_VISITOR_HPP
#define DESOLIN_TG_EXPRESSION_NODE_VISITOR_HPP

#include <desolin/tg/Desolin_tg_fwd.hpp>

template<typename T_element>
class TGExpressionNodeVisitor
{
public:
  virtual void visit(TGElementGet<tg_vector, T_element>& e) = 0;
  virtual void visit(TGElementGet<tg_matrix, T_element>& e) = 0;

  virtual void visit(TGElementSet<tg_vector, T_element>& e) = 0;
  virtual void visit(TGElementSet<tg_matrix, T_element>& e) = 0;

  virtual void visit(TGLiteral<tg_scalar, T_element>& e)=0;
  virtual void visit(TGLiteral<tg_vector, T_element>& e)=0;
  virtual void visit(TGLiteral<tg_matrix, T_element>& e)=0;

 virtual void visit(TGMatrixMult<T_element>& e)=0;
 virtual void visit(TGMatrixVectorMult<T_element>& e)=0;
 virtual void visit(TGVectorDot<T_element>& e)=0;
 virtual void visit(TGVectorCross<T_element>& e)=0;
 virtual void visit(TGVectorNorm<T_element>& e)=0;
 virtual void visit(TGMatrixTranspose<T_element>& e)=0;

 virtual void visit(TGPairwise<tg_scalar, T_element>& e) = 0;
 virtual void visit(TGPairwise<tg_vector, T_element>& e) = 0;
 virtual void visit(TGPairwise<tg_matrix, T_element>& e) = 0;

 virtual void visit(TGScalarPiecewise<tg_scalar, T_element>& e)=0;
 virtual void visit(TGScalarPiecewise<tg_vector, T_element>& e)=0;
 virtual void visit(TGScalarPiecewise<tg_matrix, T_element>& e)=0;
 
 virtual ~TGExpressionNodeVisitor() {}
};

#endif
