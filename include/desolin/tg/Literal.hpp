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

#ifndef DESOLIN_TG_LITERAL_HPP
#define DESOLIN_TG_LITERAL_HPP

#include <boost/shared_ptr.hpp>
#include <desolin/tg/Desolin_tg_fwd.hpp>

namespace desolin
{

namespace detail
{

template<typename exprType, typename T_element>
class TGLiteral : public TGExprNode<exprType, T_element>
{
public:
  bool isEqual(const TGLiteral& node, const std::map<const TGExpressionNode<T_element>*, const TGExpressionNode<T_element>*>& mappings) const
  {
    return TGExprNode<exprType, T_element>::isEqual(node, mappings);
  }
  
  TGLiteral(typename TGInternalType<exprType, T_element>::type* internal) : 
	  TGExprNode<exprType, T_element>(internal)
  {
  }

  void accept(TGExpressionNodeVisitor<T_element>& v)
  {
    v.visit(*this);
  }

  virtual void replaceDependency(const TGOutputReference<tg_scalar, T_element>& prev, TGOutputReference<tg_scalar, T_element>& next) {}
  virtual void replaceDependency(const TGOutputReference<tg_vector, T_element>& prev, TGOutputReference<tg_vector, T_element>& next) {}
  virtual void replaceDependency(const TGOutputReference<tg_matrix, T_element>& prev, TGOutputReference<tg_matrix, T_element>& next) {}
};

}

}
#endif
