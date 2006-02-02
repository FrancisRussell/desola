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

template<TGExprType resultType, TGExprType leftType, TGExprType rightType, typename T_element>
class TGBinOp : public TGExprNode<resultType, T_element>
{
private:
  TGExprNode<leftType, T_element>* left;
  TGExprNode<rightType, T_element>* right;
public:
  TGBinOp(typename TGInternalType<resultType, T_element>::type* internal, TGExprNode<leftType, T_element>& l, TGExprNode<rightType, T_element>& r) : TGExprNode<resultType, T_element>(internal), left(&l), right(&r)
  {
  }

  inline TGExprNode<leftType, T_element>& getLeft()
  {
    return *left;
  }

  inline TGExprNode<rightType, T_element>& getRight()
  {
    return *right;
  }

  virtual std::set<TGExpressionNode<T_element>*> getDependencies() const
  {
    std::set<TGExpressionNode<T_element>*> dependencies;
    dependencies.insert(left);
    dependencies.insert(right);
    return dependencies;
  }
};

template<TGExprType resultType, TGExprType exprType, typename T_element>
class TGUnOp : public TGExprNode<resultType, T_element>
{
private:
  TGExprNode<exprType, T_element>* expr;
public:
  TGUnOp(typename TGInternalType<resultType, T_element>::type* internal, TGExprNode<exprType, T_element>& e) : TGExprNode<resultType, T_element>(internal), expr(&e)
  {
  }

  inline TGExprNode<exprType, T_element>& getOperand()
  {
    return *expr;
  }

  virtual std::set<TGExpressionNode<T_element>*> getDependencies() const
  {
    std::set<TGExpressionNode<T_element>*> dependencies;
    dependencies.insert(expr);
    return dependencies;
  }
};

}
#endif
