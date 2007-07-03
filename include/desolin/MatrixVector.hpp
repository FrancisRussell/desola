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

#ifndef DESOLIN_MATRIX_VECTOR_HPP
#define DESOLIN_MATRIX_VECTOR_HPP

#include <boost/array.hpp>
#include <desolin/Desolin_fwd.hpp>

namespace desolin
{

namespace detail
{

template<typename T_element>
class MatrixMult : public BinOp<matrix, matrix, matrix, T_element>
{
private:
  inline static const boost::array<int, 2> getDims(const ExprNode<matrix, T_element>& l, const ExprNode<matrix, T_element>& r)
  {
    boost::array<int, 2> dimensions = { {l.getRowCount(), r.getColCount()} };
    return dimensions;
  }
  
public:
  MatrixMult(ExprNode<matrix, T_element>& left, ExprNode<matrix, T_element>& right) : BinOp<matrix, matrix, matrix, T_element>(getDims(left, right), left, right)
  {
  }

  void accept(ExpressionNodeVisitor<T_element>& v)
  {
    v.visit(*this);
  }
};

template<typename T_element>
class MatrixVectorMult : public BinOp<vector, matrix, vector, T_element>
{
private:
  inline static const boost::array<int, 1> getDims(const ExprNode<matrix, T_element>& l, const ExprNode<vector, T_element>& r)
  {
    boost::array<int, 1> dimensions = { {l.getRowCount()} };
    return dimensions;
  }
  
public:
  MatrixVectorMult(ExprNode<matrix, T_element>& left, ExprNode<vector, T_element>& right) : BinOp<vector, matrix, vector, T_element>(getDims(left, right), left, right)
  {
  }

  void accept(ExpressionNodeVisitor<T_element>& v)
  {
    v.visit(*this);
  }
};

template<typename T_element>
class TransposeMatrixVectorMult : public BinOp<vector, matrix, vector, T_element>
{
private:
  inline static const boost::array<int, 1> getDims(const ExprNode<matrix, T_element>& l, const ExprNode<vector, T_element>& r)
  {
    boost::array<int, 1> dimensions = { {l.getColCount()} };
    return dimensions;
  }

public:
  TransposeMatrixVectorMult(ExprNode<matrix, T_element>& left, ExprNode<vector, T_element>& right) : BinOp<vector, matrix, vector, T_element>(getDims(left, right), left, right)
  {
  }

  void accept(ExpressionNodeVisitor<T_element>& v)
  {
    v.visit(*this);
  }
};


template<typename T_element>
class VectorDot : public BinOp<scalar, vector, vector, T_element>
{
private:
  inline static const boost::array<int, 0> getDims(const ExprNode<vector, T_element>& left, const ExprNode<vector, T_element>& right)
  {
    return boost::array<int, 0>();
  }
  
public:
  VectorDot(ExprNode<vector, T_element>& left, ExprNode<vector, T_element>& right) : BinOp<scalar, vector, vector, T_element>(boost::array<int, 0>(), left, right)
  {
  }

  void accept(ExpressionNodeVisitor<T_element>& v)
  {
    v.visit(*this);
  }
};

template<typename T_element>
class VectorCross : public BinOp<vector, vector, vector, T_element>
{
private:
  inline static const boost::array<int, 1>& getDims(const ExprNode<vector, T_element>& l, const ExprNode<vector, T_element>& r)
  {
    return l.getDims();
  }
  
public:
  VectorCross(ExprNode<vector, T_element>& left, ExprNode<vector, T_element>& right) : BinOp<vector, vector, vector, T_element>(getDims(left, right), left, right)
  {
  }

  void accept(ExpressionNodeVisitor<T_element>& v)
  {
    v.visit(*this);
  }
};

template<typename T_element>
class VectorNorm : public UnOp<scalar, vector, T_element>
{
public:
  VectorNorm(ExprNode<vector, T_element>& left) : UnOp<scalar, vector, T_element>(boost::array<int, 0>(), left)
  {
  }

  void accept(ExpressionNodeVisitor<T_element>& v)
  {
    v.visit(*this);
  }
};

template<typename T_element>
class MatrixTranspose : public UnOp<matrix, matrix, T_element>
{
private:
  inline static const boost::array<int, 2> getDims(const ExprNode<matrix, T_element>& m)
  {
    boost::array<int, 2> dimensions = { {m.getColCount(), m.getRowCount()} };
    return dimensions;
  }
public:
  MatrixTranspose(ExprNode<matrix, T_element>& m) : UnOp<matrix, matrix, T_element>(getDims(m), m)
  {
  }

  void accept(ExpressionNodeVisitor<T_element>& v)
  {
    v.visit(*this);
  }
};

}

}
#endif
