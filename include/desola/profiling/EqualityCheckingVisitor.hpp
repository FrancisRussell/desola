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

#ifndef DESOLA_PROFILING_EQUALITY_CHECKING_VISITOR_HPP
#define DESOLA_PROFILING_EQUALITY_CHECKING_VISITOR_HPP

#include "Desola_profiling_fwd.hpp"
#include <map>
#include <cassert>
#include <typeinfo>

namespace desola
{

namespace detail
{

template<typename T_element>
class PEqualityCheckingVisitor : public PExpressionNodeVisitor<T_element>
{
private:
  const std::map<const PExpressionNode<T_element>*, const PExpressionNode<T_element>*> mappings;
  bool equal;

  template<typename T_node>
  void checkMatch(const T_node& left)
  {
    assert(mappings.find(&left) != mappings.end());
    const PExpressionNode<T_element>& right = *(mappings.find(&left)->second);

    if (typeid(left) == typeid(right))
    {
      const T_node& castedRight = static_cast<const T_node&>(right);
      equal = equal && left.isEqual(castedRight, mappings);
    }
    else
    {
      equal = false;
    }
  }
  
public:
  PEqualityCheckingVisitor(const std::map<const PExpressionNode<T_element>*, const PExpressionNode<T_element>*>& m) : mappings(m), equal(true)
  {
  }

  bool isEqual() const
  {
    return equal;
  }

  virtual void visit(PElementGet<vector, T_element>& e)
  {
    checkMatch(e);
  }
  
  virtual void visit(PElementGet<matrix, T_element>& e)
  {
    checkMatch(e);
  }

  virtual void visit(PElementSet<vector, T_element>& e)
  {
    checkMatch(e);
  }
  
  virtual void visit(PElementSet<matrix, T_element>& e)
  {
    checkMatch(e);
  }

  virtual void visit(PLiteral<scalar, T_element>& e)
  {
    checkMatch(e);
  }
  
  virtual void visit(PLiteral<vector, T_element>& e)
  {
    checkMatch(e);
  }
  
  virtual void visit(PLiteral<matrix, T_element>& e)
  {
    checkMatch(e);
  }
  
  virtual void visit(PMatrixMult<T_element>& e)
  {
    checkMatch(e);
  }
  
  virtual void visit(PMatrixVectorMult<T_element>& e)
  {
    checkMatch(e);
  }
  
  virtual void visit(PTransposeMatrixVectorMult<T_element>& e)
  {
    checkMatch(e);
  }
  
  virtual void visit(PVectorDot<T_element>& e)
  {
    checkMatch(e);
  }
  
  virtual void visit(PVectorCross<T_element>& e)
  {
    checkMatch(e);
  }
  
  virtual void visit(PVectorTwoNorm<T_element>& e)
  {
    checkMatch(e);
  }
  
  virtual void visit(PMatrixTranspose<T_element>& e)
  {
    checkMatch(e);
  }

  virtual void visit(PPairwise<scalar, T_element>& e)
  {
    checkMatch(e);
  }
  
  virtual void visit(PPairwise<vector, T_element>& e)
  {
    checkMatch(e);
  }
  
  virtual void visit(PPairwise<matrix, T_element>& e)
  {
    checkMatch(e);
  }

  virtual void visit(PScalarPiecewise<scalar, T_element>& e)
  {
    checkMatch(e);
  }
  
  virtual void visit(PScalarPiecewise<vector, T_element>& e)
  {
    checkMatch(e);
  }
  
  virtual void visit(PScalarPiecewise<matrix, T_element>& e)
  {
    checkMatch(e);
  }

  virtual void visit(PNegate<scalar, T_element>& e)
  {
    checkMatch(e);
  }
  
  virtual void visit(PNegate<vector, T_element>& e)
  {
    checkMatch(e);
  }
  
  virtual void visit(PNegate<matrix, T_element>& e)
  {
    checkMatch(e);
  }

  virtual void visit(PAbsolute<T_element>& e)
  {
    checkMatch(e);
  }
  
  virtual void visit(PSquareRoot<T_element>& e)
  {
    checkMatch(e);
  }
};

}

}

#endif
