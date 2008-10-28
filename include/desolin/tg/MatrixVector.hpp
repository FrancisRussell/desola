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

#ifndef DESOLIN_TG_MATRIX_VECTOR_HPP
#define DESOLIN_TG_MATRIX_VECTOR_HPP

#include <desolin/tg/Desolin_tg_fwd.hpp>

namespace desolin
{

namespace detail
{

template<typename T_element>
class TGMatrixMult : public TGBinOp<tg_matrix, tg_matrix, tg_matrix, T_element>
{
public:
  bool isEqual(const TGMatrixMult& node, const std::map<const TGExpressionNode<T_element>*, const TGExpressionNode<T_element>*>& mappings) const
  {
    return TGBinOp<tg_matrix, tg_matrix, tg_matrix, T_element>::isEqual(node, mappings);
  }
  
  TGMatrixMult(TGMatrix<T_element>* internal, 
	       const TGOutputReference<tg_matrix, T_element>& left, TGOutputReference<tg_matrix, 
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
private:
  const bool transpose;

public:
  bool isEqual(const TGMatrixVectorMult& node, const std::map<const TGExpressionNode<T_element>*, const TGExpressionNode<T_element>*>& mappings) const
  {
    return TGBinOp<tg_vector, tg_matrix, tg_vector, T_element>::isEqual(node, mappings) &&
           transpose == node.transpose;
  }
  
  TGMatrixVectorMult(TGVector<T_element>* internal, 
		  const TGOutputReference<tg_matrix, T_element>& left, 
		  const TGOutputReference<tg_vector, T_element>& right, const bool _transpose) : TGBinOp<tg_vector, tg_matrix, tg_vector, T_element>(internal, left, right),
                  transpose(_transpose)
  {
  }

  inline bool isTranspose() const
  {
    return transpose;
  }

  void accept(TGExpressionNodeVisitor<T_element>& v)
  {
    v.visit(*this);
  }
};

template<typename T_element>
class TGMatrixMultiVectorMult : public TGExpressionNode<T_element>
{
public:
  typedef boost::tuple<TGOutputReference<tg_vector, T_element>, TGVector<T_element>*, bool> multiply_params;

private:
  const TGOutputReference<tg_matrix, T_element> matrix;
  const std::vector<multiply_params>& multiplies;

public:
  bool isEqual(const TGMatrixMultiVectorMult& node, const std::map<const TGExpressionNode<T_element>*, const TGExpressionNode<T_element>*>& mappings) const
  {
    //FIXME: Implement me!
    return true;
  }
  
  TGMatrixMultiVectorMult(const TGOutputReference<tg_matrix, T_element>& _matrix, 
    const std::vector<multiply_params>& _multiplies) : matrix(_matrix), multiplies(_multiplies)
  {
  }

  void accept(TGExpressionNodeVisitor<T_element>& v)
  {
    v.visit(*this);
  }

  virtual ~TGMatrixMultiVectorMult()
  {
    //TODO: Clean up internal representations
  }
};



template<typename T_element>
class TGVectorDot : public TGBinOp<tg_scalar, tg_vector, tg_vector, T_element>
{
public:
  bool isEqual(const TGVectorDot& node, const std::map<const TGExpressionNode<T_element>*, const TGExpressionNode<T_element>*>& mappings) const
  {
    return TGBinOp<tg_scalar, tg_vector, tg_vector, T_element>::isEqual(node, mappings);
  }
  
  TGVectorDot(TGScalar<T_element>* internal, 
		  const TGOutputReference<tg_vector, T_element>& left, 
		  const TGOutputReference<tg_vector, T_element>& right) : TGBinOp<tg_scalar, tg_vector, tg_vector, T_element>(internal, left, right)
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
  bool isEqual(const TGVectorCross& node, const std::map<const TGExpressionNode<T_element>*, const TGExpressionNode<T_element>*>& mappings) const
  {
    return TGBinOp<tg_vector, tg_vector, tg_vector, T_element>::isEqual(node, mappings);
  }
  
  TGVectorCross(TGVector<T_element>* internal, 
		  const TGOutputReference<tg_vector, T_element>& left, 
		  const TGOutputReference<tg_vector, T_element>& right) : TGBinOp<tg_vector, tg_vector, tg_vector, T_element>(internal, left, right)
  {
  }

  void accept(TGExpressionNodeVisitor<T_element>& v)
  {
    v.visit(*this);
  }
};

template<typename T_element>
class TGVectorTwoNorm : public TGUnOp<tg_scalar, tg_vector, T_element>
{
public:
  bool isEqual(const TGVectorTwoNorm& node, const std::map<const TGExpressionNode<T_element>*, const TGExpressionNode<T_element>*>& mappings) const
  {
    return TGUnOp<tg_scalar, tg_vector, T_element>::isEqual(node, mappings);
  }

  TGVectorTwoNorm(TGScalar<T_element>* internal, 
		  const TGOutputReference<tg_vector, T_element>& left) : TGUnOp<tg_scalar, tg_vector, T_element>(internal, left)
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
  bool isEqual(const TGMatrixTranspose& node, const std::map<const TGExpressionNode<T_element>*, const TGExpressionNode<T_element>*>& mappings) const
  {
    return TGUnOp<tg_matrix, tg_matrix, T_element>::isEqual(node, mappings);
  }
  
  TGMatrixTranspose(TGMatrix<T_element>* internal,
		    const TGOutputReference<tg_matrix, T_element>& matrix) : TGUnOp<tg_matrix, tg_matrix, T_element>(internal, matrix)
  {
  }

  void accept(TGExpressionNodeVisitor<T_element>& v)
  {
    v.visit(*this);
  }
};

}

}
#endif
