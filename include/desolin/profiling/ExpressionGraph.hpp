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

#ifndef DESOLIN_PROFILING_EXPRESSION_GRAPH_HPP
#define DESOLIN_PROFILING_EXPRESSION_GRAPH_HPP

#include "Desolin_profiling_fwd.hpp"
#include <boost/ptr_container/ptr_vector.hpp>
#include <cstddef>
#include <functional>
#include <map>
#include <algorithm>
#include <cassert>

namespace desolin
{

namespace detail
{

template<typename T_element>
class PExpressionGraph
{
private:
  PExpressionGraph(const PExpressionGraph&);
  PExpressionGraph& operator=(const PExpressionGraph&);

  boost::ptr_vector< PExpressionNode<T_element> > exprVector;

  mutable bool isHashCached;
  mutable std::size_t cachedHash;

  template<typename VisitorType>
  class ApplyVisitor : public std::unary_function< void, PExpressionNode<T_element> >
  {
  private:
    VisitorType& visitor;

  public:
    ApplyVisitor(VisitorType& v) : visitor(v)
    {
    }

    inline void operator()(PExpressionNode<T_element>& node) const
    {
      node.accept(visitor);
    }
  };
  

public:
  PExpressionGraph(ExpressionGraph<T_element>& expressionGraph) : isHashCached(false)
  {
    PExpressionNodeGenerator<T_element> generator(*this);
    expressionGraph.accept(generator);
  }

  inline std::size_t nodeCount() const
  {
    return exprVector.size();
  }

  void addNode(PExpressionNode<T_element>* const node)
  {
    exprVector.push_back(node);
  }

  PExpressionNode<T_element>& nodeAt(const std::size_t index)
  {
    return exprVector[index];
  }

  void accept(PExpressionNodeVisitor<T_element>& visitor)
  {
    std::for_each(exprVector.begin(), exprVector.end(), ApplyVisitor< PExpressionNodeVisitor<T_element> >(visitor));
  }

  bool operator==(const PExpressionGraph& right) const
  {
    if(exprVector.size() != right.exprVector.size())
    {
      return false;
    }
    else
    {
      std::map<const PExpressionNode<T_element>*, const PExpressionNode<T_element>*> mappings;
      for(std::size_t index = 0; index<exprVector.size(); ++index)
      {
        mappings[&exprVector[index]] = &right.exprVector[index];
      }

      PEqualityCheckingVisitor<T_element> checker(mappings);
      const_cast<PExpressionGraph<T_element>&>(*this).accept(checker);
      return checker.isEqual();
    }
  }

  friend std::size_t hash_value(const PExpressionGraph<T_element>& graph)
  {
    if(!graph.isHashCached)
    {
      graph.isHashCached = true;

      std::map<const PExpressionNode<T_element>*, int> nodeNumberings;
      for(std::size_t index=0; index<graph.exprVector.size(); ++index)
      {
        nodeNumberings[&graph.exprVector[index]] = index;
      }

      PHashingVisitor<T_element> hasher(nodeNumberings);
      const_cast<PExpressionGraph<T_element>&>(graph).accept(hasher);
      graph.cachedHash = hasher.getHash();
    }
    return graph.cachedHash;
  }
};

}

}

#endif
