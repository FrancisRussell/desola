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

#ifndef DESOLIN_PROFILING_ELEMENTAL_HPP
#define DESOLIN_PROFILING_ELEMENTAL_HPP

#include "Desolin_profiling_fwd.hpp"
#include <map>
#include <vector>

namespace desolin
{

namespace detail
{

template<ExprType exprType, typename T_element>
class PElementGet : public PUnOp<scalar, exprType, T_element>
{
public:
  inline bool isEqual(const PElementGet& node, const std::map<const PExpressionNode<T_element>*, const PExpressionNode<T_element>*>& mappings) const
  {
    return PUnOp<scalar, exprType, T_element>::isEqual(node, mappings);
  }

  PElementGet(PExprNode<exprType, T_element>& operand) : PUnOp<scalar, exprType, T_element>(operand)
  {
  }

  virtual void accept(PExpressionNodeVisitor<T_element>& visitor)
  {
    visitor.visit(*this);
  }  
};

template<ExprType exprType, typename T_element>
class PElementSet : public PUnOp<exprType, exprType, T_element>
{
private:
  const std::vector<PExprNode<scalar, T_element>*> assignments;

public:
  PElementSet(PExprNode<exprType, T_element>& e, const std::vector<PExprNode<scalar, T_element>*>& a) : PUnOp<exprType, exprType, T_element>(e), assignments(a)
  {
  }
  
  bool isEqual(const PElementSet& node, const std::map<const PExpressionNode<T_element>*, const PExpressionNode<T_element>*>& mappings) const
  {
    if (!PUnOp<exprType, exprType, T_element>::isEqual(node, mappings))
      return false;

    if (assignments.size() != node.assignments.size())
      return false;

    //TODO: Fix to use STL algorithm?
    for(typename std::vector<PExprNode<scalar, T_element>*>::size_type index = 0; index<assignments.size(); ++index)
    {
      assert(mappings.find(assignments[index]) != mappings.end());
      if (mappings.find(assignments[index])->second != node.assignments[index])
        return false;
    }
    
    return true;
  }

  virtual void accept(PExpressionNodeVisitor<T_element>& visitor)
  {
    visitor.visit(*this);
  }

  std::vector<PExprNode<scalar, T_element>*> getAssignments()
  {
    return assignments;
  }

  std::vector<const PExprNode<scalar, T_element>*> getAssignments() const
  {
    return std::vector<const PExprNode<scalar, T_element>*>(assignments.begin(), assignments.end());
  }
};

}

}

#endif
