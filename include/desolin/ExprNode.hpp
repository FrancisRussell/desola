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
#include <numeric>
#include <boost/array.hpp>
#include <boost/functional/hash.hpp>
#include <boost/static_assert.hpp>
#include <desolin/Desolin_fwd.hpp>

namespace desolin
{

namespace detail
{

template<ExprType exprType, typename T_elementType>
class ExprNode : public ExpressionNode<T_elementType>
{
  // Should never be instantiated
  BOOST_STATIC_ASSERT(sizeof(T_elementType)==0);
};

template<typename T_elementType>
class ExprNode<scalar, T_elementType> : public ExpressionNode<T_elementType>
{
protected:
  const boost::array<int, 0> dimensions;
	
public:
  ExprNode() : dimensions(boost::array<int, 0>())
  {
  }

  ExprNode(const boost::array<int, 0>& dims) : dimensions(dims)
  {
  }

  const boost::array<int, 0>& getDims() const
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
  const boost::array<int, 1> dimensions;
  
public:
  ExprNode(const boost::array<int, 1>& dims) : dimensions(dims)
  {
  }
  
  const boost::array<int, 1>& getDims() const
  {
    return dimensions;
  }
  
  const int getRowCount() const
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
  const boost::array<int, 2> dimensions;
	
public:
  ExprNode(const boost::array<int, 2>& dims) : dimensions(dims)
  {
  }

  const boost::array<int, 2>& getDims() const
  {
    return dimensions;
  }
  
  const int getRowCount() const
  {
    return dimensions[0];
  }

  const int getColCount() const
  {
    return dimensions[1];
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

template<ExprType exprType>
class ElementIndex
{
  // Should never be instantiated
  BOOST_STATIC_ASSERT(exprType!=exprType);
};

template<>
class ElementIndex<vector>
{
private:
  const int row;

public:
  ElementIndex(const int r) : row(r)
  {
  }
  
  inline int getRow() const
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
  return boost::hash<int>()(index.getRow());
}

template<>
class ElementIndex<matrix>
{
private:
  const int row;
  const int col;

public:
  ElementIndex(const int r, const int c) : row(r), col(c)
  {
  }

  inline const int getRow() const
  {
    return row;
  }

  inline const int getCol() const
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
  std::size_t seed = boost::hash<int>()(index.getRow());
  boost::hash_combine(seed, index.getCol());
  return seed;
}

template<ExprType exprType>
struct ExprDimensions
{
};

template<>
struct ExprDimensions<scalar>
{
  static const int dims = 0;
};

template<>
struct ExprDimensions<vector>
{
  static const int dims = 1;
};

template<>
struct ExprDimensions<matrix>
{
  static const int dims = 2;
};

}

}
#endif
