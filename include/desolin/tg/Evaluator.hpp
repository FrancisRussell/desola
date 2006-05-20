#ifndef DESOLIN_TG_EVALUATOR_HPP
#define DESOLIN_TG_EVALUATOR_HPP

#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/functional/hash.hpp>
#include <vector>
#include <map>
#include <set>
#include <cassert>
#include <TaskGraph>
#include <desolin/tg/Desolin_tg_fwd.hpp>

namespace desolin_internal
{

template<typename T_element>
class TGEvaluator : public Evaluator<T_element>
{
private:
  typedef std::map<std::size_t, boost::shared_ptr< TGExpressionGraph<T_element> > > T_cachedGraphMap;
  static T_cachedGraphMap cachedGraphs;
	
  TGEvaluator(const TGEvaluator&);
  TGEvaluator& operator=(const TGEvaluator&);
	
  bool evaluated;
  EvaluationStrategy<T_element>& strategy;
  boost::shared_ptr< TGExpressionGraph<T_element> > graph;
  TGObjectGenerator<T_element> objectGenerator;
  std::vector<ExpressionNode<T_element>*> claimed;

  void generateNodes()
  {
    for(typename std::vector<ExpressionNode<T_element>*>::iterator iterator = claimed.begin(); iterator!=claimed.end(); ++iterator)
    {
      (*iterator)->accept(objectGenerator);
    }	   
  }

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
    boost::function<void ()> nodeGenerator = boost::bind(&TGEvaluator::generateNodes, this);
    graph->generateCode(nodeGenerator);
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
    const typename T_cachedGraphMap::iterator cachedGraphIterator = cachedGraphs.find(hash);

    ParameterHolder parameterHolder;
    objectGenerator.addTaskGraphMappings(parameterHolder);
	    
    if (cachedGraphIterator != cachedGraphs.end() && (*graph)==(*cachedGraphIterator->second))
    {
      graph = cachedGraphIterator->second;
    }
    else
    {
      graph->compile();
      cachedGraphs[hash] = graph;
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
std::map<std::size_t, boost::shared_ptr< TGExpressionGraph<T_element> > > TGEvaluator<T_element>::cachedGraphs;

}
#endif
