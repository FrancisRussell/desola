#ifndef DESOLIN_TG_MATRIX_VECTOR_HPP
#define DESOLIN_TG_MATRIX_VECTOR_HPP

#include <desolin/tg/Desolin_tg_fwd.hpp>

namespace desolin_internal
{

template<typename T_element>
class TGMatrixMult : public TGBinOp<tg_matrix, tg_matrix, tg_matrix, T_element>
{
public:
  TGMatrixMult(TGMatrix<T_element>* internal, 
		  TGExprNode<tg_matrix, T_element>& left, TGExprNode<tg_matrix, 
		  T_element>& right) : TGBinOp<tg_matrix, tg_matrix, tg_matrix, T_element>(internal, left, right)
  {
  }

  void accept(TGExpressionNodeVisitor<T_element>& v)
  {
    v.visit(*this);
  }
};

template<typename T_element>
class TGMatrixVectorMult : public TGBinOp<tg_vector, tg_matrix, tg_vector, T_element>
{
public:
  TGMatrixVectorMult(TGVector<T_element>* internal, 
		  TGExprNode<tg_matrix, T_element>& left, 
		  TGExprNode<tg_vector, T_element>& right) : TGBinOp<tg_vector, tg_matrix, tg_vector, T_element>(internal, left, right)
  {
  }

  void accept(TGExpressionNodeVisitor<T_element>& v)
  {
    v.visit(*this);
  }
};

template<typename T_element>
class TGVectorDot : public TGBinOp<tg_scalar, tg_vector, tg_vector, T_element>
{
public:
  TGVectorDot(TGScalar<T_element>* internal, 
		  TGExprNode<tg_vector, T_element>& left, 
		  TGExprNode<tg_vector, T_element>& right) : TGBinOp<tg_scalar, tg_vector, tg_vector, T_element>(internal, left, right)
  {
  }

  void accept(TGExpressionNodeVisitor<T_element>& v)
  {
    v.visit(*this);
  }
};

template<typename T_element>
class TGVectorCross : public TGBinOp<tg_vector, tg_vector, tg_vector, T_element>
{
public:
  TGVectorCross(TGVector<T_element>* internal, 
		  TGExprNode<tg_vector, T_element>& left, 
		  TGExprNode<tg_vector, T_element>& right) : TGBinOp<tg_vector, tg_vector, tg_vector, T_element>(internal, left, right)
  {
  }

  void accept(TGExpressionNodeVisitor<T_element>& v)
  {
    v.visit(*this);
  }
};

template<typename T_element>
class TGVectorNorm : public TGUnOp<tg_scalar, tg_vector, T_element>
{
public:
  TGVectorNorm(TGScalar<T_element>* internal, 
		  TGExprNode<tg_vector, T_element>& left) : TGUnOp<tg_scalar, tg_vector, T_element>(internal, left)
  {
  }

  void accept(TGExpressionNodeVisitor<T_element>& v)
  {
    v.visit(*this);
  }
};

template<typename T_element>
class TGMatrixTranspose : public TGUnOp<tg_matrix, tg_matrix, T_element>
{
public:
  TGMatrixTranspose(TGMatrix<T_element>* internal,
		  TGExprNode<tg_matrix, T_element>& matrix) : TGUnOp<tg_matrix, tg_matrix, T_element>(internal, matrix)
  {
  }

  void accept(TGExpressionNodeVisitor<T_element>& v)
  {
    v.visit(*this);
  }
};

}
#endif
