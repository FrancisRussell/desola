#ifndef DESOLIN_PROFILING_UNOP_HPP
#define DESOLIN_PROFILING_UNOP_HPP

#include "Desolin_profiling_fwd.hpp"
#include <map>
#include <cassert>

namespace desolin_internal
{

template<ExprType resultType, ExprType exprType, typename T_element>
class PUnOp : public PExprNode<resultType, T_element>
{
private:
  PExprNode<exprType, T_element>* expr;

public:
  PUnOp(PExprNode<exprType, T_element>& e) : PExprNode<resultType, T_element>(), expr(&e)
  {
  }
  
  inline bool isEqual(const PUnOp& node, const std::map<const PExpressionNode<T_element>*, const PExpressionNode<T_element>*>& mappings) const
  {
    assert(mappings.find(expr) != mappings.end());

    return PExprNode<resultType, T_element>::isEqual(node, mappings) &&
	   mappings.find(expr)->second == node.expr;
  }

  inline PExprNode<resultType, T_element>& getOperand()
  {
    return *expr;
  }

  inline const PExprNode<resultType, T_element>& getOperand() const
  {
    return *expr;
  }
};

template<ExprType exprType, typename T_element>
class PNegate : public PUnOp<exprType, exprType, T_element>
{
public:
  PNegate(PExprNode<exprType, T_element>& e) : PUnOp<exprType, exprType, T_element>(e)
  {
  }

  inline bool isEqual(const PNegate& node, const std::map<const PExpressionNode<T_element>*, const PExpressionNode<T_element>*>& mappings) const
  {
    return PUnOp<exprType, exprType, T_element>::isEqual(node, mappings);
  }
  
  virtual void accept(PExpressionNodeVisitor<T_element>& visitor)
  {
    visitor.visit(*this);
  }
};

template<typename T_element>
class PAbsolute : public PUnOp<scalar, scalar, T_element>
{
public:
  PAbsolute(PExprNode<scalar, T_element>& e) : PUnOp<scalar, scalar, T_element>(e)
  {
  }
		    
  inline bool isEqual(const PAbsolute& node, const std::map<const PExpressionNode<T_element>*, const PExpressionNode<T_element>*>& mappings) const
  {
    return PUnOp<scalar, scalar, T_element>::isEqual(node, mappings);
  }

  virtual void accept(PExpressionNodeVisitor<T_element>& visitor)
  {
    visitor.visit(*this);
  }
};

template<typename T_element>
class PSquareRoot : public PUnOp<scalar, scalar, T_element>
{
public:
  PSquareRoot(PExprNode<scalar, T_element>& e) : PUnOp<scalar, scalar, T_element>(e)
  {
  }

  inline bool isEqual(const PSquareRoot& node, const std::map<const PExpressionNode<T_element>*, const PExpressionNode<T_element>*>& mappings) const
  {
    return PUnOp<scalar, scalar, T_element>::isEqual(node, mappings);
  }

  virtual void accept(PExpressionNodeVisitor<T_element>& visitor)
  {
    visitor.visit(*this);
  }
};

}

#endif
