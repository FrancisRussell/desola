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

#ifndef DESOLIN_EXPRNODE_HPP
#define DESOLIN_EXPRNODE_HPP

#include <set>
#include <vector>
#include <map>
#include <cassert>
#include <cstddef>
#include <numeric>
#include <boost/array.hpp>
#include <boost/functional/hash.hpp>
#include <boost/static_assert.hpp>
#include <desolin/Desolin_fwd.hpp>

namespace desolin
{

namespace detail
{

template<typename exprType, typename T_elementType>
class ExprNode : public ExpressionNode<T_elementType>
{
  // Should never be instantiated
  BOOST_STATIC_ASSERT(sizeof(T_elementType)==0);
};

template<typename T_elementType>
class ExprNode<scalar, T_elementType> : public ExpressionNode<T_elementType>
{
protected:
  const boost::array<std::size_t, 0> dimensions;
	
public:
  ExprNode() : dimensions(boost::array<std::size_t, 0>())
  {
  }

  ExprNode(const boost::array<std::size_t, 0>& dims) : dimensions(dims)
  {
  }

  const boost::array<std::size_t, 0>& getDims() const
  {
    return dimensions;
  }

  virtual void accept(ExpressionNodeTypeVisitor<T_elementType>& visitor)
  {
    visitor.visit(*this);
  }

  void replace(ExprNode& replacement)
  {
    this->internalReplace(*this, replacement);
  }

  virtual T_elementType getElementValue()
  {
    assert(false);
    return T_elementType();
  }
};

template<typename T_elementType>
class ExprNode<vector, T_elementType> : public ExpressionNode<T_elementType>
{
protected:
  const boost::array<std::size_t, 1> dimensions;
  
public:
  ExprNode(const boost::array<std::size_t, 1>& dims) : dimensions(dims)
  {
  }
  
  const boost::array<std::size_t, 1>& getDims() const
  {
    return dimensions;
  }
  
  const std::size_t getRowCount() const
  {
    return dimensions[0];
  }

  virtual void accept(ExpressionNodeTypeVisitor<T_elementType>& visitor)
  {
    visitor.visit(*this);
  }

  void replace(ExprNode& replacement)
  {
    this->internalReplace(*this, replacement);
  }

  virtual T_elementType getElementValue(const ElementIndex<vector>& index)
  {
    assert(false);
    return T_elementType();
  }			   
};

template<typename T_elementType>
class ExprNode<matrix, T_elementType> : public ExpressionNode<T_elementType>
{
protected:
  const boost::array<std::size_t, 2> dimensions;
	
public:
  ExprNode(const boost::array<std::size_t, 2>& dims) : dimensions(dims)
  {
  }

  const boost::array<std::size_t, 2>& getDims() const
  {
    return dimensions;
  }
  
  const std::size_t getRowCount() const
  {
    return dimensions[0];
  }

  const std::size_t getColCount() const
  {
    return dimensions[1];
  }

  virtual Maybe<std::size_t> nnz() const
  {
    return Nothing();
  }

  virtual void accept(ExpressionNodeTypeVisitor<T_elementType>& visitor)
  {
    visitor.visit(*this);
  }

  void replace(ExprNode& replacement)
  {
    this->internalReplace(*this, replacement);
  }

  virtual T_elementType getElementValue(const ElementIndex<matrix>& index)
  {
    assert(false);
    return T_elementType();
  }
};

template<typename exprType>
class ElementIndex
{
  // Should never be instantiated
  BOOST_STATIC_ASSERT(sizeof(exprType) == 0);
};

template<>
class ElementIndex<vector>
{
private:
  const std::size_t row;

public:
  ElementIndex(const std::size_t r) : row(r)
  {
  }
  
  inline std::size_t getRow() const
  {
    return row;
  }

  inline bool operator==(const ElementIndex& i) const
  {
    return row == i.row;
  }

  inline bool operator<(const ElementIndex& i) const
  {
    return row < i.row;
  }
};

std::size_t hash_value(const ElementIndex<vector>& index)
{
  return boost::hash<std::size_t>()(index.getRow());
}

template<>
class ElementIndex<matrix>
{
private:
  const std::size_t row;
  const std::size_t col;

public:
  ElementIndex(const std::size_t r, const size_t c) : row(r), col(c)
  {
  }

  inline const std::size_t getRow() const
  {
    return row;
  }

  inline const std::size_t getCol() const
  {
    return col;
  }

  inline bool operator==(const ElementIndex& i) const
  {
    return row == i.row && col == i.col;
  }

  inline bool operator<(const ElementIndex& i) const
  {
    if(row != i.row)
    {
      return row < i.row;
    }
    else
    {
      return col < i.col;
    }
  }
};

std::size_t hash_value(const ElementIndex<matrix>& index)
{
  std::size_t seed = boost::hash<std::size_t>()(index.getRow());
  boost::hash_combine(seed, index.getCol());
  return seed;
}

template<typename exprType>
struct ExprDimensions
{
};

template<>
struct ExprDimensions<scalar>
{
  static const std::size_t dims = 0;
};

template<>
struct ExprDimensions<vector>
{
  static const std::size_t dims = 1;
};

template<>
struct ExprDimensions<matrix>
{
  static const std::size_t dims = 2;
};

}

}
#endif
