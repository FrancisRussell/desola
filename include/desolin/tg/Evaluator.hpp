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

#ifndef DESOLIN_TG_EVALUATOR_HPP
#define DESOLIN_TG_EVALUATOR_HPP

#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/functional/hash.hpp>
#include <vector>
#include <map>
#include <set>
#include <cassert>
#include <TaskGraph>
#include <desolin/tg/Desolin_tg_fwd.hpp>

namespace desolin
{

namespace detail
{

template<typename T_element>
class TGCache : public Cache
{
public:
  typedef std::map<std::size_t, boost::shared_ptr< TGExpressionGraph<T_element> > > T_cachedGraphMap;
private:
  T_cachedGraphMap cache;
  
public:
  virtual void flush()
  {
    cache.clear();
  }

  T_cachedGraphMap& getCachedGraphs()
  {
    return cache;
  }
};

template<typename T_element>
class TGEvaluator : public Evaluator<T_element>
{
private:
  static TGCache<T_element> graphCache;
	
  TGEvaluator(const TGEvaluator&);
  TGEvaluator& operator=(const TGEvaluator&);
	
  bool evaluated;
  EvaluationStrategy<T_element>& strategy;
  boost::shared_ptr< TGExpressionGraph<T_element> > graph;
  TGObjectGenerator<T_element> objectGenerator;
  std::vector<ExpressionNode<T_element>*> claimed;

public:
  TGEvaluator(EvaluationStrategy<T_element>& s) : evaluated(false), strategy(s), graph(new TGExpressionGraph<T_element>()), objectGenerator(*this)
  {
  }

  //TODO: Force claimed nodes to be topologically adjacent
  virtual std::set<ExpressionNode<T_element>*> claimNodes(const std::vector< ExpressionNode<T_element>*>& nodes)
  {
    // We claim all unevaluated nodes by default
    claimed = nodes;
    return std::set<ExpressionNode<T_element>*>(claimed.begin(), claimed.end());	
  }

  virtual void generateEvaluatedNodes()
  {
    for(typename std::vector<ExpressionNode<T_element>*>::iterator iterator = claimed.begin(); iterator!=claimed.end(); ++iterator)
      (*iterator)->accept(objectGenerator);
  }

  inline EvaluationStrategy<T_element>& getStrategy()
  {
    return strategy;
  }

  inline TGExpressionGraph<T_element>& getTGExpressionGraph()
  {
    return *graph;
  }
  
  virtual void evaluate()
  {
    assert(!evaluated); 
    evaluated = true;

    const std::size_t hash = boost::hash< TGExpressionGraph<T_element> >()(*graph);
    typename TGCache<T_element>::T_cachedGraphMap& cachedGraphMap(graphCache.getCachedGraphs());
    const typename TGCache<T_element>::T_cachedGraphMap::iterator cachedGraphIterator = cachedGraphMap.find(hash);

    ParameterHolder parameterHolder;
    const ConfigurationManager& configurationManager(ConfigurationManager::getConfigurationManager());	
    objectGenerator.addTaskGraphMappings(parameterHolder);
	    
    if (configurationManager.codeCachingEnabled() && cachedGraphIterator != cachedGraphMap.end() && 
	(*graph)==(*cachedGraphIterator->second))
    {
      graph = cachedGraphIterator->second;
    }
    else
    {
      graph->performHighLevelFusion();
      graph->generateCode();
      graph->compile();
      cachedGraphMap[hash] = graph;
    }
    
    graph->execute(parameterHolder);
  }
};

template<typename T_element>
class TGEvaluatorFactory : public EvaluatorFactory<T_element>
{
public:	
  virtual boost::shared_ptr< Evaluator<T_element> > createEvaluator(EvaluationStrategy<T_element>& strategy)
  {
    return boost::shared_ptr< Evaluator<T_element> >(new TGEvaluator<T_element>(strategy));
  }
};

template<typename T_element>
TGCache<T_element> TGEvaluator<T_element>::graphCache;

}

}
#endif
