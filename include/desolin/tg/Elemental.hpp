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

#ifndef DESOLIN_TG_ELEMENTAL_HPP
#define DESOLIN_TG_ELEMENTAL_HPP

#include <utility>
#include <map>
#include <algorithm>
#include <vector>
#include <cassert>
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <desolin/tg/Desolin_tg_fwd.hpp>

namespace desolin
{

namespace detail
{

template<TGExprType exprType, typename T_element>
class TGElementGet : public TGUnOp<tg_scalar, exprType, T_element>
{
private:
  const TGElementIndex<exprType> index;

public:
  bool isEqual(const TGElementGet& node, const std::map<const TGExpressionNode<T_element>*, const TGExpressionNode<T_element>*>& mappings) const
  {
    return TGUnOp<tg_scalar, exprType, T_element>::isEqual(node, mappings) &&
    index == node.index;
  }
  
  TGElementGet(typename TGInternalType<tg_scalar, T_element>::type* internal, 
		  TGExprNode<exprType, T_element>& o, 
		  const TGElementIndex<exprType> i) :  TGUnOp<tg_scalar, exprType, T_element>(internal, o), 
								     index(i)
  {
  }

  inline const TGElementIndex<exprType>& getIndex() const
  {
    return index;
  }

  virtual void accept(TGExpressionNodeVisitor<T_element>& v)
  {
    v.visit(*this);
  }
};

template<TGExprType exprType, typename T_element>
class TGElementSet : public TGUnOp<exprType, exprType, T_element>
{
private:
  const std::map<TGElementIndex<exprType>, TGExprNode<tg_scalar, T_element>*> assignments;

  void registerDependency(const std::pair<const TGElementIndex<exprType>, TGExprNode<tg_scalar, T_element>*>& pair)
  {
    this->dependencies.insert(pair.second);
  }

public:
  bool isEqual(const TGElementSet& node, const std::map<const TGExpressionNode<T_element>*, const TGExpressionNode<T_element>*>& mappings) const
  {
    if (!TGUnOp<exprType, exprType, T_element>::isEqual(node, mappings))
      return false;
    
    typedef typename std::map<TGElementIndex<exprType>, TGExprNode<tg_scalar, T_element>*>::const_iterator ConstIterator;

    for(ConstIterator i = assignments.begin(); i != assignments.end(); ++i)
    {
      ConstIterator assignment = node.assignments.find(i->first);
      if (assignment == node.assignments.end())
      {
        return false;
      }
      else
      {
        assert(mappings.find(i->second) != mappings.end());
        if (mappings.find(i->second)->second != assignment->second)
          return false;
      }
    }
    return true;
  } 
  
public:
  TGElementSet(typename TGInternalType<exprType, T_element>::type* internal,
	       TGExprNode<exprType, T_element>& o,
	       const std::map<TGElementIndex<exprType>, TGExprNode<tg_scalar, T_element>*>& a) : TGUnOp<exprType, exprType, T_element>(internal, o), assignments(a)
  {
    std::for_each(assignments.begin(), assignments.end(), boost::bind(&TGElementSet::registerDependency, this, _1));
  }

  std::map<TGElementIndex<exprType>, TGExprNode<tg_scalar, T_element>*> getAssignments() 
  {
    return assignments;
  }

  std::map<TGElementIndex<exprType>, const TGExprNode<tg_scalar, T_element>*> getAssignments() const
  {
    return std::map<TGElementIndex<exprType>, const TGExprNode<tg_scalar, T_element>*>(assignments.begin(), assignments.end());
  }

  virtual void accept(TGExpressionNodeVisitor<T_element>& v)
  {
    v.visit(*this);
  }
};

}

}
#endif
