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

#ifndef DESOLIN_TG_EXPRESSION_NODE_HPP
#define DESOLIN_TG_EXPRESSION_NODE_HPP

#include <set>
#include <map>
#include <string>
#include <cassert>
#include <cstddef>
#include <boost/scoped_ptr.hpp>
#include <boost/functional/hash.hpp>
#include <desolin/tg/Desolin_tg_fwd.hpp>

namespace desolin
{

namespace detail
{

template<TGExprType exprType>
class TGElementIndex
{
};

template<>
class TGElementIndex<tg_vector>
{
private:
  const std::size_t row;

public:
  TGElementIndex(const std::size_t r) : row(r)
  {
  }

  TGElementIndex(const ElementIndex<vector>& index) : row(index.getRow())
  {
  }

  const inline std::size_t getRow() const
  {
    return row;
  }

  inline bool operator==(const TGElementIndex& i) const
  {
    return row == i.row;
  }

  inline bool operator<(const TGElementIndex& i) const
  {
    return row < i.row;
  }
};

std::size_t hash_value(const TGElementIndex<tg_vector>& index)
{
  return boost::hash<int>()(index.getRow());
}

template<>
class TGElementIndex<tg_matrix>
{
private:
  const std::size_t row;
  const std::size_t col;
  
public:
  TGElementIndex(const std::size_t r, const std::size_t c) : row(r), col(c)
  {
  }

  TGElementIndex(const ElementIndex<matrix>& index) : row(index.getRow()), col(index.getCol())
  {
  }

  const inline std::size_t getRow() const
  {
    return row;
  }

  const inline std::size_t getCol() const
  {
    return col;
  }

  inline bool operator==(const TGElementIndex& i) const
  {
    return row == i.row && col == i.col;
  }

  inline bool operator<(const TGElementIndex& i) const
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

std::size_t hash_value(const TGElementIndex<tg_matrix>& index)
{
  std::size_t seed = boost::hash<std::size_t>()(index.getRow());
  boost::hash_combine(seed, index.getCol());
  return seed;
}


template<typename T_element>
class TGExpressionNode
{
private:
  // Forbid copying and assignment
  TGExpressionNode(const TGExpressionNode&);
  TGExpressionNode& operator=(const TGExpressionNode&);

protected:
  std::set<TGExpressionNode<T_element>*> dependencies;

public:
  TGExpressionNode()
  {
  }
  
  virtual void accept(TGExpressionNodeVisitor<T_element>& visitor) = 0;
  virtual void createTaskGraphVariable() = 0;

  std::set<TGExpressionNode<T_element>*> getDependencies() const
  {
    return dependencies;
  }

  virtual ~TGExpressionNode()
  {
  }
};

template<TGExprType exprType, typename T_element>
class TGExprNode : public TGExpressionNode<T_element>
{
private:
  typedef typename TGInternalType<exprType, T_element>::type T_internal;
  boost::scoped_ptr< T_internal > internal;

public:
  inline bool isEqual(const TGExprNode& node, const std::map<const TGExpressionNode<T_element>*, const TGExpressionNode<T_element>*>& mappings) const
  {
    return internal->matches(*node.internal);
  }
  
  TGExprNode(T_internal* i) : internal(i)
  {
  }

  void createTaskGraphVariable()
  {
    internal->createTaskGraphVariable();
  }

  inline T_internal& getInternal()
  {
    return *internal;
  }

  inline const T_internal& getInternal() const
  {
    return *internal;
  }

  inline bool isParameter() const
  {
    return internal->isParameter();
  }
};

}

}
#endif
