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
#include <boost/foreach.hpp>
#include <desolin/tg/Desolin_tg_fwd.hpp>

namespace desolin
{

namespace detail
{

template<typename exprType, typename T_element>
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
		  const TGOutputReference<exprType, T_element>& o, 
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

template<typename exprType, typename T_element>
class TGElementSet : public TGExprNode<exprType, T_element>
{
public:
  typedef std::map<TGElementIndex<exprType>, TGOutputReference<tg_scalar, T_element> > AssignmentMap;

private:
  TGOutputReference<exprType, T_element> expr;
  AssignmentMap assignments;

  static void registerAssignmentValue(TGElementSet* const e, const std::pair<const TGElementIndex<exprType>, TGOutputReference<tg_scalar, T_element> >& pair)
  {
    e->registerDependency(pair.second.getExpressionNode());
  }

public:
  bool isEqual(const TGElementSet& node, const std::map<const TGExpressionNode<T_element>*, const TGExpressionNode<T_element>*>& mappings) const
  {
    if (!TGExprNode<exprType, T_element>::isEqual(node, mappings))
      return false;
    
    if (!TGExpressionNode<T_element>::isEqual(expr, node.expr, mappings))
      return false;

    if (assignments.size() != node.assignments.size())
      return false;
    
    typedef typename std::map<TGElementIndex<exprType>, TGOutputReference<tg_scalar, T_element> >::const_iterator ConstIterator;

    for(ConstIterator i = assignments.begin(); i != assignments.end(); ++i)
    {
      ConstIterator assignment = node.assignments.find(i->first);
      if (assignment == node.assignments.end())
      {
        return false;
      }
      else
      {
        if (!TGExpressionNode<T_element>::isEqual(i->second, assignment->second, mappings))
          return false;
      }
    }
    return true;
  } 
  
  TGElementSet(typename TGInternalType<exprType, T_element>::type* internal,
	       const TGOutputReference<exprType, T_element>& o,
	       const AssignmentMap& a) : TGExprNode<exprType, T_element>(internal), expr(o), assignments(a)
  {
    this->registerDependency(expr.getExpressionNode());
    std::for_each(assignments.begin(), assignments.end(), boost::bind(&TGElementSet::registerAssignmentValue, this, _1));
  }

  inline TGOutputReference<exprType, T_element> getOperand()
  {
    return expr;
  }

  inline const TGOutputReference<exprType, T_element> getOperand() const
  {
    return expr;
  }

  std::map<TGElementIndex<exprType>, TGOutputReference<tg_scalar, T_element> > getAssignments() 
  {
    return assignments;
  }

  std::map<TGElementIndex<exprType>, const TGOutputReference<tg_scalar, T_element> > getAssignments() const
  {
    return std::map<TGElementIndex<exprType>, const TGOutputReference<tg_scalar, T_element> >(assignments.begin(), assignments.end());
  }

  virtual void accept(TGExpressionNodeVisitor<T_element>& v)
  {
    v.visit(*this);
  }

  virtual void alterDependencyImpl(const TGOutputReference<tg_scalar, T_element>& previous, TGOutputReference<tg_scalar, T_element>& next)
  {
    ReplaceOutputReference<exprType, tg_scalar, T_element>()(expr, previous, next);

    BOOST_FOREACH(typename AssignmentMap::value_type& assignment, assignments)
    {
      ReplaceOutputReference<tg_scalar, tg_scalar, T_element>()(assignment.second, previous, next);
    }
  }

  virtual void alterDependencyImpl(const TGOutputReference<tg_vector, T_element>& previous, TGOutputReference<tg_vector, T_element>& next)
  {
    ReplaceOutputReference<exprType, tg_vector, T_element>()(expr, previous, next);
  }

  virtual void alterDependencyImpl(const TGOutputReference<tg_matrix, T_element>& previous, TGOutputReference<tg_matrix, T_element>& next)
  {
    ReplaceOutputReference<exprType, tg_matrix, T_element>()(expr, previous, next);
  }
};

}

}
#endif
