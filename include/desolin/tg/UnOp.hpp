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

template<TGExprType resultType, TGExprType exprType, typename T_element>
class TGUnOp : public TGExprNode<resultType, T_element>
{
private:
  TGExprNode<exprType, T_element>* expr;

public:
  inline bool isEqual(const TGUnOp& node, const std::map<const TGExpressionNode<T_element>*, const TGExpressionNode<T_element>*>& mappings) const
  {
    assert(mappings.find(expr) != mappings.end());

    return TGExprNode<resultType, T_element>::isEqual(node, mappings) && 
           mappings.find(expr)->second == node.expr;
  }
    
  TGUnOp(typename TGInternalType<resultType, T_element>::type* internal, TGExprNode<exprType, T_element>& e) : TGExprNode<resultType, T_element>(internal), expr(&e)
  {
    this->dependencies.insert(expr);
  }

  inline TGExprNode<exprType, T_element>& getOperand()
  {
    return *expr;
  }

  inline const TGExprNode<exprType, T_element>& getOperand() const
  {
    return *expr;
  }
};

template<TGExprType exprType, typename T_element>
class TGNegate : public TGUnOp<exprType, exprType, T_element>
{
public:
  TGNegate(typename TGInternalType<exprType, T_element>::type* internal, TGExprNode<exprType, T_element>& e) : TGUnOp<exprType, exprType, T_element>(internal, e)
  {
  }

  inline bool isEqual(const TGNegate& node, const std::map<const TGExpressionNode<T_element>*, const TGExpressionNode<T_element>*>& mappings) const
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
  TGAbsolute(typename TGInternalType<tg_scalar, T_element>::type* internal, TGExprNode<tg_scalar, T_element>& e) : TGUnOp<tg_scalar, tg_scalar, T_element>(internal, e)
  {
  }

  inline bool isEqual(const TGAbsolute& node, const std::map<const TGExpressionNode<T_element>*, const TGExpressionNode<T_element>*>& mappings) const
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
  TGSquareRoot(typename TGInternalType<tg_scalar, T_element>::type* internal, TGExprNode<tg_scalar, T_element>& e) : TGUnOp<tg_scalar, tg_scalar, T_element>(internal, e)
  {
  }

  inline bool isEqual(const TGSquareRoot& node, const std::map<const TGExpressionNode<T_element>*, const TGExpressionNode<T_element>*>& mappings) const
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
