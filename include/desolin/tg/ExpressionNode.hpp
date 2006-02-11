#ifndef DESOLIN_TG_EXPRESSION_NODE_HPP
#define DESOLIN_TG_EXPRESSION_NODE_HPP

#include <set>
#include <boost/shared_ptr.hpp>
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
};

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
};

template<typename T_element>
class TGExpressionNode
{
private:
  // Forbid copying and assignment
  TGExpressionNode(const TGExpressionNode&);
  TGExpressionNode& operator=(const TGExpressionNode&);
  
public:
  TGExpressionNode()
  {
  }
  
  virtual void accept(TGExpressionNodeVisitor<T_element>& visitor) = 0;

  virtual std::set<TGExpressionNode<T_element>*> getDependencies() const = 0;

  virtual ~TGExpressionNode()
  {
  }
};

template<TGExprType exprType, typename T_element>
class TGExprNode : public TGExpressionNode<T_element>
{
private:
  typedef typename TGInternalType<exprType, T_element>::type T_internal;
  boost::shared_ptr< T_internal > internal;
public:
  TGExprNode(T_internal* i) : internal(i)
  {
  }

  inline boost::shared_ptr< T_internal > getInternal()
  {
    return internal;
  }

  bool isParameter() const
  {
    return internal->isParameter();
  }
};

}
#endif
