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

namespace desolin_internal
{

template<TGExprType exprType>
class TGElementIndex
{
};

template<>
class TGElementIndex<tg_vector>
{
private:
  const int row;

public:
  TGElementIndex(const int r) : row(r)
  {
  }

  TGElementIndex(const ElementIndex<vector>& index) : row(index.getRow())
  {
  }

  const inline int getRow() const
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
  const int row;
  const int col;
  
public:
  TGElementIndex(const int r, const int c) : row(r), col(c)
  {
  }

  TGElementIndex(const ElementIndex<matrix>& index) : row(index.getRow()), col(index.getCol())
  {
  }

  const inline int getRow() const
  {
    return row;
  }

  const inline int getCol() const
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
  std::size_t seed = boost::hash<int>()(index.getRow());
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
#endif
