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

#ifndef DESOLIN_PROFILING_LITERAL_HPP
#define DESOLIN_PROFILING_LITERAL_HPP

#include "Desolin_profiling_fwd.hpp"
#include <map>

namespace desolin
{

namespace detail
{

template<ExprType exprType, typename T_element>
class PLiteral : public PExprNode<exprType, T_element>
{
public:
  bool isEqual(const PLiteral& node, const std::map<const PExpressionNode<T_element>*, const PExpressionNode<T_element>*>& mappings) const
  {
    return PExprNode<exprType, T_element>::isEqual(node, mappings);
  }

  PLiteral() : PExprNode<exprType, T_element>()
  {
  }

  virtual void accept(PExpressionNodeVisitor<T_element>& visitor)
  {
    visitor.visit(*this);
  }
};

}

}

#endif
