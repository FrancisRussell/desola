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

#ifndef DESOLIN_PROFILING_PROFILER_HPP
#define DESOLIN_PROFILING_PROFILER_HPP

#include "Desolin_profiling_fwd.hpp"
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/bind/apply.hpp>
#include <memory>
#include <map>
#include <cstddef>

namespace desolin
{

namespace detail
{

template<typename T_element>
class Profiler : public Cache
{
private:
  Profiler(const Profiler&);
  Profiler& operator=(const Profiler&);
	  
  static Profiler<T_element> profiler;
  typedef std::map<std::size_t, boost::shared_ptr< PExpressionGraph<T_element> > > T_cachedProfileMap;
  T_cachedProfileMap cachedProfiles;

  Profiler()
  {
  }

  void addMonitors(ExpressionGraph<T_element>& graph, boost::shared_ptr< PExpressionGraph<T_element> > profilingGraph)
  {
    assert(graph.nodeCount() == profilingGraph->nodeCount());

    for(std::size_t index=0; index<graph.nodeCount(); ++index)
    {
      PExpressionNodeRef<T_element> ref(profilingGraph, profilingGraph->nodeAt(index));
      graph.nodeAt(index).notifyOfUse(ref);
    }
  }

  void setEvaluationDirectives(ExpressionGraph<T_element>& graph, boost::shared_ptr< PExpressionGraph<T_element> > profilingGraph)
  {
    assert(graph.nodeCount() == profilingGraph->nodeCount());

    for(std::size_t index=0; index<graph.nodeCount(); ++index)
    {
      const EvaluationDirective directive(profilingGraph->nodeAt(index).getSuggestedEvaluationDirective());
      graph.nodeAt(index).setEvaluationDirective(directive);
    }
  }
      
  static bool hasNoEvaluateSet(const ExpressionNode<T_element>* const node)
  {
    return node->getEvaluationDirective() == NO_EVALUATE;
  }

  std::vector<ExpressionNode<T_element>*> getRequiredNodes(ExpressionGraph<T_element>& graph)
  {
    const std::vector<ExpressionNode<T_element>*> allNodes(graph.sortedNodesBegin(), graph.sortedNodesEnd());
    std::vector<ExpressionNode<T_element>*> required;
    std::back_insert_iterator< std::vector<ExpressionNode<T_element>*> > inserter(required);

    std::remove_copy_if(allNodes.begin(), allNodes.end(), inserter, boost::bind(hasNoEvaluateSet, _1));
    return ExpressionNode<T_element>::getTopologicallySortedNodes(required);
  }
  
public:
  static Profiler& getProfiler()
  {
    return profiler;
  }
  
  std::auto_ptr< ExpressionGraph<T_element> > getAnnotatedExpressionGraph(ExpressionGraph<T_element>& graph, ExpressionNode<T_element>& current)
  {
    boost::shared_ptr< PExpressionGraph<T_element> > profilingGraph(new PExpressionGraph<T_element>(graph));
    const std::size_t hash = boost::hash< PExpressionGraph<T_element> >()(*profilingGraph);

    const typename T_cachedProfileMap::iterator cachedProfileIterator(cachedProfiles.find(hash)); 

    if (cachedProfileIterator != cachedProfiles.end() && (*profilingGraph)==(*cachedProfileIterator->second))
    {
      profilingGraph = cachedProfileIterator->second;
    }
    else
    {
      cachedProfiles[hash] = profilingGraph;
    }

    addMonitors(graph, profilingGraph);
    setEvaluationDirectives(graph, profilingGraph);

    // Make sure the node being evaluated always has flag set for it to be evaluated
    current.setEvaluationDirective(EVALUATE);

    const std::vector<ExpressionNode<T_element>*> requiredNodes(getRequiredNodes(graph));
    return std::auto_ptr< ExpressionGraph<T_element> >(new ExpressionGraph<T_element>(requiredNodes.begin(), requiredNodes.end()));
  }

  virtual void flush()
  {
    cachedProfiles.clear();
  }

  virtual ~Profiler()
  {
  }
};

template<typename T_element> Profiler<T_element> Profiler<T_element>::profiler;

}

}
#endif
