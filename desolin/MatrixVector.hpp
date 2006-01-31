#ifndef DESOLIN_MATRIX_VECTOR_HPP
#define DESOLIN_MATRIX_VECTOR_HPP

#include <boost/array.hpp>
#include <desolin/Desolin_fwd.hpp>

template<typename T_element>
class MatrixMult : public BinOp<matrix, matrix, matrix, T_element>
{
private:
  inline static const boost::array<int, 2> getDims(const ExprNode<matrix, T_element>& l, const ExprNode<matrix, T_element>& r)
  {
    boost::array<int, 2> dimensions = {l.getRowCount(), r.getColCount()};
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
    boost::array<int, 1> dimensions = {l.getRowCount()};
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
  inline static const boost::array<int, 2> getDims(const ExprNode<matrix, T_element>& matrix)
  {
    boost::array<int, 2> dimensions = {matrix.getColCount(), matrix.getRowCount()};
    return dimensions;
  }
public:
  MatrixTranspose(ExprNode<matrix, T_element>& matrix) : UnOp<matrix, matrix, T_element>(getDims(matrix), matrix)
  {
  }

  void accept(ExpressionNodeVisitor<T_element>& v)
  {
    v.visit(*this);
  }
};

#endif
