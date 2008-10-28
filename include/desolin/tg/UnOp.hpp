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

#ifndef DESOLIN_TG_UNOP_HPP
#define DESOLIN_TG_UNOP_HPP

#include <set>
#include <map>
#include <cassert>
#include <desolin/tg/Desolin_tg_fwd.hpp>

namespace desolin
{

namespace detail
{

template<typename resultType, typename exprType, typename T_element>
class TGUnOp : public TGExprNode<resultType, T_element>
{
private:
  TGOutputReference<exprType, T_element> expr;

public:
  bool isEqual(const TGUnOp& node, const std::map<const TGExpressionNode<T_element>*, const TGExpressionNode<T_element>*>& mappings) const
  {
    return TGExprNode<resultType, T_element>::isEqual(node, mappings) &&
           TGExpressionNode<T_element>::isEqual(expr, node.expr, mappings);
  }
    
  TGUnOp(typename TGInternalType<resultType, T_element>::type* internal, TGOutputReference<exprType, T_element>& e) : TGExprNode<resultType, T_element>(internal), expr(e)
  {
    this->dependencies.insert(expr.getExpressionNode());
  }

  inline TGOutputReference<exprType, T_element> getOperand()
  {
    return expr;
  }

  inline const TGOutputReference<exprType, T_element> getOperand() const
  {
    return expr;
  }
};

template<typename exprType, typename T_element>
class TGNegate : public TGUnOp<exprType, exprType, T_element>
{
public:
  TGNegate(typename TGInternalType<exprType, T_element>::type* internal, TGOutputReference<exprType, T_element>& e) : TGUnOp<exprType, exprType, T_element>(internal, e)
  {
  }

  bool isEqual(const TGNegate& node, const std::map<const TGExpressionNode<T_element>*, const TGExpressionNode<T_element>*>& mappings) const
  {
    return TGUnOp<exprType, exprType, T_element>::isEqual(node, mappings);
  }
  
  virtual void accept(TGExpressionNodeVisitor<T_element>& v)
  {
    v.visit(*this);
  }
};

template<typename T_element>
class TGAbsolute : public TGUnOp<tg_scalar, tg_scalar, T_element>
{
public:
  TGAbsolute(typename TGInternalType<tg_scalar, T_element>::type* internal, TGOutputReference<tg_scalar, T_element>& e) : TGUnOp<tg_scalar, tg_scalar, T_element>(internal, e)
  {
  }

  bool isEqual(const TGAbsolute& node, const std::map<const TGExpressionNode<T_element>*, const TGExpressionNode<T_element>*>& mappings) const
  {
    return TGUnOp<tg_scalar, tg_scalar, T_element>::isEqual(node, mappings);
  }
  
  virtual void accept(TGExpressionNodeVisitor<T_element>& v)
  {
    v.visit(*this);
  }
};

template<typename T_element>
class TGSquareRoot : public TGUnOp<tg_scalar, tg_scalar, T_element>
{
public:
  TGSquareRoot(typename TGInternalType<tg_scalar, T_element>::type* internal, TGOutputReference<tg_scalar, T_element>& e) : TGUnOp<tg_scalar, tg_scalar, T_element>(internal, e)
  {
  }

  bool isEqual(const TGSquareRoot& node, const std::map<const TGExpressionNode<T_element>*, const TGExpressionNode<T_element>*>& mappings) const
  {
    return TGUnOp<tg_scalar, tg_scalar, T_element>::isEqual(node, mappings);
  } 

  virtual void accept(TGExpressionNodeVisitor<T_element>& v)
  {
    v.visit(*this);
  }
};

}

}
#endif
