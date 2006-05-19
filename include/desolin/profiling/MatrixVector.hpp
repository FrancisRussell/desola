#ifndef DESOLIN_PROFILING_MATRIX_VECTOR_HPP
#define DESOLIN_PROFILING_MATRIX_VECTOR_HPP

#include "Desolin_profiling_fwd.hpp"
#include <map>

namespace desolin_internal
{

template<typename T_element>
class PMatrixMult : public PBinOp<matrix, matrix, matrix, T_element>
{
public:
  inline bool isEqual(const PMatrixMult& node, const std::map<const PExpressionNode<T_element>*, const PExpressionNode<T_element>*>& mappings) const
  {
    return PBinOp<matrix, matrix, matrix, T_element>::isEqual(node, mappings);
  }

  PMatrixMult(PExprNode<matrix, T_element>& left, PExprNode<matrix, T_element>& right) : PBinOp<matrix, matrix, matrix, T_element>(left, right)
  {
  }

  virtual void accept(PExpressionNodeVisitor<T_element>& visitor)
  {
    visitor.visit(*this);
  }
};

template<typename T_element>
class PMatrixVectorMult : public PBinOp<vector, matrix, vector, T_element>
{
public:
  inline bool isEqual(const PMatrixVectorMult& node, const std::map<const PExpressionNode<T_element>*, const PExpressionNode<T_element>*>& mappings) const
  {
    return PBinOp<vector, matrix, vector, T_element>::isEqual(node, mappings);
  }

  PMatrixVectorMult(PExprNode<matrix, T_element>& left, PExprNode<vector, T_element>& right) : PBinOp<vector, matrix, vector, T_element>(left, right)
  {
  }

  virtual void accept(PExpressionNodeVisitor<T_element>& visitor)
  {
    visitor.visit(*this);
  }
};

template<typename T_element>
class PTransposeMatrixVectorMult : public PBinOp<vector, matrix, vector, T_element>
{
public:
  inline bool isEqual(const PTransposeMatrixVectorMult& node, const std::map<const PExpressionNode<T_element>*, const PExpressionNode<T_element>*>& mappings) const
  {
    return PBinOp<vector, matrix, vector, T_element>::isEqual(node, mappings);
  }

  PTransposeMatrixVectorMult(PExprNode<matrix, T_element>& left, PExprNode<vector, T_element>& right) : PBinOp<vector, matrix, vector, T_element>(left, right)
  {
  }

  virtual void accept(PExpressionNodeVisitor<T_element>& visitor)
  {
    visitor.visit(*this);
  }
};

template<typename T_element>
class PVectorDot : public PBinOp<scalar, vector, vector, T_element>
{
public:
  inline bool isEqual(const PVectorDot& node, const std::map<const PExpressionNode<T_element>*, const PExpressionNode<T_element>*>& mappings) const
  {
    return PBinOp<scalar, vector, vector, T_element>::isEqual(node, mappings);
  }

  PVectorDot(PExprNode<vector, T_element>& left, PExprNode<vector, T_element>& right) : PBinOp<scalar, vector, vector, T_element>(left, right)
  {
  }

  virtual void accept(PExpressionNodeVisitor<T_element>& visitor)
  {
    visitor.visit(*this);
  }
};

template<typename T_element>
class PVectorCross : public PBinOp<vector, vector, vector, T_element>
{
public:
  inline bool isEqual(const PVectorCross& node, const std::map<const PExpressionNode<T_element>*, const PExpressionNode<T_element>*>& mappings) const
  {
    return PBinOp<vector, vector, vector, T_element>::isEqual(node, mappings);
  }

  PVectorCross(PExprNode<vector, T_element>& left, PExprNode<vector, T_element>& right) : PBinOp<vector, vector, vector, T_element>(left, right)
  {
  }

  virtual void accept(PExpressionNodeVisitor<T_element>& visitor)
  {
    visitor.visit(*this);
  }
};

template<typename T_element>
class PVectorNorm : public PUnOp<scalar, vector, T_element>
{
public:
  inline bool isEqual(const PVectorNorm& node, const std::map<const PExpressionNode<T_element>*, const PExpressionNode<T_element>*>& mappings) const
  {     
    return PUnOp<scalar, vector, T_element>::isEqual(node, mappings);
  }

  PVectorNorm(PExprNode<vector, T_element>& operand) : PUnOp<scalar, vector, T_element>(operand)
  {
  }

  virtual void accept(PExpressionNodeVisitor<T_element>& visitor)
  {
    visitor.visit(*this);
  }
};

template<typename T_element>
class PMatrixTranspose : public PUnOp<matrix, matrix, T_element>
{
public:
  inline bool isEqual(const PMatrixTranspose& node, const std::map<const PExpressionNode<T_element>*, const PExpressionNode<T_element>*>& mappings) const
  {
    return PUnOp<matrix, matrix, T_element>::isEqual(node, mappings);
  }

  PMatrixTranspose(PExprNode<matrix, T_element>& operand) : PUnOp<matrix, matrix, T_element>(operand)
  {
  }

  virtual void accept(PExpressionNodeVisitor<T_element>& visitor)
  {
    visitor.visit(*this);
  }
};

}

#endif