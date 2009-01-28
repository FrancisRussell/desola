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

#ifndef DESOLA_EXPRESSION_GRAPH_HPP
#define DESOLA_EXPRESSION_GRAPH_HPP

#include <algorithm>
#include <vector>
#include <functional>
#include <cstddef>
#include <numeric>
#include <boost/shared_ptr.hpp>
#include <desola/Desola_fwd.hpp>

namespace desola
{

namespace detail
{

template<typename T_element>
class ExpressionGraph
{
private:
  std::vector<ExpressionNode<T_element>*>  exprVector;

  template<typename VisitorType>
  class ApplyVisitor : public std::unary_function<void, ExpressionNode<T_element>*>
  {
  private:
    VisitorType& visitor;

  public:
    ApplyVisitor(VisitorType& v) : visitor(v)
    {
    }
    
    inline void operator()(ExpressionNode<T_element>* const node) const
    {
      node->accept(visitor);
    }
  };
			
  template<typename VisitorType>
  inline void internalAccept(VisitorType& visitor)
  {
    std::for_each(exprVector.begin(), exprVector.end(), ApplyVisitor<VisitorType>(visitor));
  }

  static void setDefaultAnnotation(ExpressionNode<T_element>* const node)
  {
    if (node->getExternalRequiredBy().empty())
    {
      node->setEvaluationDirective(NO_EVALUATE);
    }
    else
    {
      node->setEvaluationDirective(EVALUATE);
    }
  }

  static Maybe<double> accumulateFlops(const Maybe<double>& flops, const ExpressionNode<T_element>* const node)
  {
    return flops + node->getFlops();
  }
  
public:
  typedef typename std::vector<ExpressionNode<T_element>*>::const_iterator const_node_iterator;

  template<typename InputIterator>
  ExpressionGraph(const InputIterator& start, const InputIterator& end) : exprVector(start, end)
  {
  }

  const_node_iterator sortedNodesBegin() const
  {
    return exprVector.begin();
  }

  const_node_iterator sortedNodesEnd() const
  {
    return exprVector.end();
  }

  inline std::size_t nodeCount() const
  {
    return exprVector.size();
  }

  ExpressionNode<T_element>& nodeAt(const std::size_t index)
  {
    return *exprVector[index];
  }

  void accept(ExpressionNodeVisitor<T_element>& visitor)
  {
    internalAccept(visitor);
  }

  void accept(LiteralVisitor<T_element>& visitor)
  {
    internalAccept(visitor);
  }

  void accept(ExpressionNodeTypeVisitor<T_element>& visitor)
  {
    internalAccept(visitor);
  }

  // Replicates original behavious before profiling system. Only saves result
  // of nodes which have an external handle pointing to them.
  void setDefaultAnnotations()
  {
    std::for_each(exprVector.begin(), exprVector.end(), setDefaultAnnotation);
  }

  Maybe<double> getFlops() const
  {
    return std::accumulate(exprVector.begin(), exprVector.end(), Maybe<double>(0.0), accumulateFlops);
  }
   
  boost::shared_ptr<EvaluationStrategy<T_element> > createEvaluationStrategy()
  {
    return boost::shared_ptr<EvaluationStrategy<T_element> >(new EvaluationStrategy<T_element>(*this));
  }
};

}

}
#endif
