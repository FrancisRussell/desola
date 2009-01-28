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

#ifndef DESOLA_PROFILING_UNOP_HPP
#define DESOLA_PROFILING_UNOP_HPP

#include "Desola_profiling_fwd.hpp"
#include <map>
#include <cassert>

namespace desola
{

namespace detail
{

template<typename resultType, typename exprType, typename T_element>
class PUnOp : public PExprNode<resultType, T_element>
{
private:
  PExprNode<exprType, T_element>* expr;

public:
  PUnOp(PExprNode<exprType, T_element>& e) : PExprNode<resultType, T_element>(), expr(&e)
  {
  }
  
  bool isEqual(const PUnOp& node, const std::map<const PExpressionNode<T_element>*, const PExpressionNode<T_element>*>& mappings) const
  {
    assert(mappings.find(expr) != mappings.end());

    return PExprNode<resultType, T_element>::isEqual(node, mappings) &&
	   mappings.find(expr)->second == node.expr;
  }

  inline PExprNode<exprType, T_element>& getOperand()
  {
    return *expr;
  }

  inline const PExprNode<exprType, T_element>& getOperand() const
  {
    return *expr;
  }
};

template<typename exprType, typename T_element>
class PNegate : public PUnOp<exprType, exprType, T_element>
{
public:
  PNegate(PExprNode<exprType, T_element>& e) : PUnOp<exprType, exprType, T_element>(e)
  {
  }

  bool isEqual(const PNegate& node, const std::map<const PExpressionNode<T_element>*, const PExpressionNode<T_element>*>& mappings) const
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
		    
  bool isEqual(const PAbsolute& node, const std::map<const PExpressionNode<T_element>*, const PExpressionNode<T_element>*>& mappings) const
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

  bool isEqual(const PSquareRoot& node, const std::map<const PExpressionNode<T_element>*, const PExpressionNode<T_element>*>& mappings) const
  {
    return PUnOp<scalar, scalar, T_element>::isEqual(node, mappings);
  }

  virtual void accept(PExpressionNodeVisitor<T_element>& visitor)
  {
    visitor.visit(*this);
  }
};

}

}

#endif
