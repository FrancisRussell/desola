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

#include <vector>
#include <map>
#include <cstddef>
#include <boost/foreach.hpp>
#include <boost/variant.hpp>
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
  typedef typename TGExpressionNode<T_element>::internal_variant_type internal_variant_type;

private:
  TGOutputReference<tg_matrix, T_element> matrix;
  std::vector<multiply_params> multiplies;

public:
  bool isEqual(const TGMatrixMultiVectorMult& node, const std::map<const TGExpressionNode<T_element>*, const TGExpressionNode<T_element>*>& mappings) const
  {
    if (!TGExpressionNode<T_element>::isEqual(node, mappings))
      return false;

    const std::size_t numMultiplies = multiplies.size();

    if (numMultiplies != node.multiplies.size())
      return false;

    for(std::size_t index=0; index < numMultiplies; ++index)
    {
      if (!TGExpressionNode<T_element>::isEqual(boost::get<0>(multiplies[index]), boost::get<0>(node.multiplies[index]), mappings))
        return false;

      if (boost::get<2>(multiplies[index]) != boost::get<2>(node.multiplies[index]))
        return false;
    }

    return true;
  }
  
  TGMatrixMultiVectorMult(const TGOutputReference<tg_matrix, T_element>& _matrix, 
    const std::vector<multiply_params>& _multiplies) : matrix(_matrix), multiplies(_multiplies)
  {
    this->registerDependency(matrix.getExpressionNode());

    BOOST_FOREACH(const multiply_params& paramTuple, multiplies)
    {
      this->registerDependency(boost::get<0>(paramTuple).getExpressionNode());
    }
  }

  void accept(TGExpressionNodeVisitor<T_element>& v)
  {
    v.visit(*this);
  }

  inline TGOutputReference<tg_matrix, T_element> getMatrix()
  {
    return matrix;
  }

  inline const TGOutputReference<tg_matrix, T_element> getMatrix() const
  {
    return matrix;
  }

  inline TGOutputReference<tg_vector, T_element> getVector(const std::size_t index)
  {
    return boost::get<0>(multiplies[index]);
  }

  inline const TGOutputReference<tg_vector, T_element> getVector(const std::size_t index) const
  {
    return boost::get<0>(multiplies[index]);
  }

  inline bool isTranspose(const std::size_t index) const
  {
    return boost::get<2>(multiplies[index]);
  }

  std::size_t getNumVectors() const
  {
    return multiplies.size();
  }

  virtual std::size_t getNumOutputs() const
  {
    return multiplies.size();
  }

  bool isParameter(const std::size_t index) const
  {
    assert(index < multiplies.size());
    return boost::get<1>(multiplies[index])->isParameter();
  }

  internal_variant_type getInternal(const std::size_t index)
  {
    assert(index < multiplies.size());
    return internal_variant_type(boost::get<1>(multiplies[index]));
  }

  const internal_variant_type getInternal(const std::size_t index) const
  {
    assert(index < multiplies.size());
    return internal_variant_type(boost::get<1>(multiplies[index]));
  }

  void createTaskGraphVariable()
  {
    BOOST_FOREACH(multiply_params paramTuple, multiplies)
    {
      boost::get<1>(paramTuple)->createTaskGraphVariable();
    }
  }

  virtual ~TGMatrixMultiVectorMult()
  {
    BOOST_FOREACH(multiply_params paramTuple, multiplies)
    {
      delete boost::get<1>(paramTuple);
    }
  }

  virtual void alterDependencyImpl(const TGOutputReference<tg_scalar, T_element>& previous, TGOutputReference<tg_scalar, T_element>& next)
  {
  }

  virtual void alterDependencyImpl(const TGOutputReference<tg_vector, T_element>& previous, TGOutputReference<tg_vector, T_element>& next)
  {
    BOOST_FOREACH(multiply_params& paramTuple, multiplies)
    {
      ReplaceOutputReference<tg_vector, tg_vector, T_element>()(boost::get<0>(paramTuple), previous, next);
    }
  }

  virtual void alterDependencyImpl(const TGOutputReference<tg_matrix, T_element>& previous, TGOutputReference<tg_matrix, T_element>& next)
  {
    ReplaceOutputReference<tg_matrix, tg_matrix, T_element>()(matrix, previous, next);
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
