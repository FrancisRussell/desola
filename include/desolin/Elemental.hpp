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

#ifndef DESOLIN_ELEMENTAL_HPP
#define DESOLIN_ELEMENTAL_HPP

#include <utility>
#include <cassert>
#include <map>
#include <algorithm>
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <desolin/Desolin_fwd.hpp>

namespace desolin
{

namespace detail
{

template<ExprType exprType, typename T_element>
class ElementGet : public UnOp<scalar, exprType, T_element>
{
private:
  const ElementIndex<exprType> index;

public:
  ElementGet(ExprNode<exprType, T_element>& e, const ElementIndex<exprType>& i) : UnOp<scalar, exprType, T_element>(boost::array<std::size_t, 0>(), e), index(i)
  {
  }
  
  inline const ElementIndex<exprType>& getIndex() const
  { 
    return index;
  }
  
  virtual void accept(ExpressionNodeVisitor<T_element>& visitor)
  { 
    visitor.visit(*this);
  }

  virtual void internal_evaluate()
  {
    this->getOperand().evaluate();
  }

  virtual T_element getElementValue()
  {
    return this->getOperand().getElementValue(index);
  }

  virtual double getFlops() const
  {
    return 0.0;
  }
};

template<ExprType exprType, typename T_element>
class ElementSet : public UnOp<exprType, exprType, T_element>
{
private:
  std::map<ElementIndex<exprType>, ExprNode<scalar, T_element>*> assignments;

  void registerAssignmentDependency(const std::pair<const ElementIndex<exprType>, ExprNode<scalar, T_element>*>& pair)
  {
    this->registerDependency(pair.second);
  }

  void updateAssignmentDependency(std::pair<const ElementIndex<exprType>, ExprNode<scalar, T_element>*>& pair, ExprNode<scalar, T_element>& previous, ExprNode<scalar, T_element>& next)
  {
    if (pair.second == &previous)
    {
      this->replaceDependency(&previous, &next);
      pair.second = &next;
    }
  }
  
public:
  ElementSet(ExprNode<exprType, T_element>& e, const std::map<ElementIndex<exprType>, ExprNode<scalar, T_element>*>& a) : UnOp<exprType, exprType, T_element>(e.getDims(), e), assignments(a)
  {
    std::for_each(assignments.begin(), assignments.end(), boost::bind(&ElementSet::registerAssignmentDependency, this, _1));
  }

  std::map<ElementIndex<exprType>, ExprNode<scalar, T_element>*> getAssignments()
  {
    return assignments;
  }

  std::map<ElementIndex<exprType>, const ExprNode<scalar, T_element>*> getAssignments() const
  { 
    return std::map<ElementIndex<exprType>, const ExprNode<scalar, T_element>*>(assignments.begin(), assignments.end());
  } 
  
  virtual void accept(ExpressionNodeVisitor<T_element>& visitor)
  { 
    visitor.visit(*this);
  }

  virtual void update(ExprNode<scalar, T_element>& previous, ExprNode<scalar, T_element>& next)
  {
    // Not strictly necessary as Scalars don't have elements to set.
    UnOp<exprType, exprType, T_element>::update(previous, next);
   
    for(typename std::map<ElementIndex<exprType>, ExprNode<scalar, T_element>*>::iterator i=assignments.begin(); i!=assignments.end(); ++i)
    {
      updateAssignmentDependency(*i, previous, next);
    }
  }

  virtual double getFlops() const
  {
    return 0.0;
  }
};

}

}
#endif
