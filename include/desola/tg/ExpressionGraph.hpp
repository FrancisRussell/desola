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

#ifndef DESOLA_TG_EXPRESSION_GRAPH_HPP
#define DESOLA_TG_EXPRESSION_GRAPH_HPP

#include <vector>
#include <algorithm>
#include <functional>
#include <cstddef>
#include <cassert>
#include <map>
#include <set>
#include <iterator>
#include <utility>
#include <TaskGraph>
#include <desola/tg/Desola_tg_fwd.hpp>
#include <boost/foreach.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <sys/time.h>

namespace desola
{

namespace detail
{

template<typename T_element>
class TGExpressionGraph
{
private:
  TGExpressionGraph(const TGExpressionGraph&);
  TGExpressionGraph& operator=(const TGExpressionGraph&);
  
  std::vector<TGExpressionNode<T_element>*> exprVector;
  boost::scoped_ptr<tg::tuTaskGraph> taskGraphObject;
  NameGenerator generator;

  mutable bool isHashCached;
  mutable std::size_t cachedHash;
  
  template<typename VisitorType>
  class ApplyVisitor : public std::unary_function< void, TGExpressionNode<T_element> >
  {
  private:
    VisitorType& visitor;

  public:
    ApplyVisitor(VisitorType& v) : visitor(v)
    {
    }

    inline void operator()(TGExpressionNode<T_element>* const node) const
    {
      node->accept(visitor);
    }
  };

  std::vector<TGExpressionNode<T_element>*> getTopologicalSort() const
  {
    const std::vector<TGExpressionNode<T_element>*> leaves(getLeaves(exprVector));
    std::vector<TGExpressionNode<T_element>*> nodes;
    std::set<TGExpressionNode<T_element>*> visited; 
    nodes.reserve(exprVector.size());

    typedef std::back_insert_iterator< std::vector<TGExpressionNode<T_element>*> > OutputIterator;
    OutputIterator out(nodes);
    std::for_each(leaves.begin(), leaves.end(), boost::bind(getTopologicalSortHelper<OutputIterator>, _1, boost::ref(visited), boost::ref(out)));
    return nodes;
  }

  template<typename OutputIterator>
  static void getTopologicalSortHelper(TGExpressionNode<T_element>* const node, std::set<TGExpressionNode<T_element>*>& visited, OutputIterator& out)
  {
    if (visited.insert(node).second)
    {
      std::for_each(node->beginDependencies(), node->endDependencies(), 
        boost::bind(getTopologicalSortHelper<OutputIterator>, _1, boost::ref(visited), boost::ref(out)));
      *out++ = node;
    }
  }

  static std::vector<TGExpressionNode<T_element>*> getLeaves(const std::vector<TGExpressionNode<T_element>*>& nodes)
  {
    std::vector<TGExpressionNode<T_element>*> leaves;

    BOOST_FOREACH(TGExpressionNode<T_element>* node, nodes)
    {
      if (node->numReverseDependencies() == 0)
        leaves.push_back(node);
    }

    return leaves;
  }

  template<typename exprType>
  void replaceDependencyImpl(const TGOutputReference<exprType, T_element>& previous, TGOutputReference<exprType, T_element>& next)
  {
    TGExpressionNode<T_element>* const node = previous.getExpressionNode();
    
    BOOST_FOREACH(TGExpressionNode<T_element>* node, std::make_pair(node->beginReverseDependencies(), node->endReverseDependencies()))
    {
      node->alterDependency(previous, next);
    }
  }

public:
  TGExpressionGraph() : taskGraphObject(NULL), isHashCached(false)
  {
  }

  inline void add(TGExpressionNode<T_element>* const value)
  {
    assert(!isHashCached);
    exprVector.push_back(value);
  }

  inline NameGenerator& getNameGenerator()
  {
    return generator;
  }

  const inline int size() const
  {
    return exprVector.size();
  }

  void performHighLevelFusion()
  {
    HighLevelFuser<T_element> fuser(*this);
    fuser.execute();
  }

  void generateCode()
  {
    // FIXME: We only create the TaskGraph object here because if we don't do any code generation, the TaskGraph segfaults 
    // on destruction.
    assert(taskGraphObject.get() == NULL);
    boost::scoped_ptr<tg::tuTaskGraph> newTaskGraph(new tg::tuTaskGraph());
    taskGraphObject.swap(newTaskGraph);

    tu_taskgraph(*taskGraphObject)
    {
      for(typename std::vector<TGExpressionNode<T_element>*>::iterator exprVectorIter(exprVector.begin()); exprVectorIter!=exprVector.end(); ++exprVectorIter)
        (*exprVectorIter)->createTaskGraphVariable();

      TGCodeGenerator<T_element> codeGenerator(*this);
      accept(codeGenerator);
    }
  }

  tg::Compilers getTaskCompiler() const
  {
    const ConfigurationManager& configurationManager(ConfigurationManager::getConfigurationManager());
    
    if (configurationManager.usingGCC())
    {
      return tg::GCC;
    }
    else if (configurationManager.usingICC())
    {
      return tg::ICC;
    }
    else
    {
      assert(false);
      return tg::GCC;
    }
  }

  void compile()
  {
    timeval time;
    gettimeofday(&time, NULL);
    const double startTime = time.tv_sec + time.tv_usec/1000000.0;
    
    const ConfigurationManager& configurationManager(ConfigurationManager::getConfigurationManager());

    if (configurationManager.loopFusionEnabled())
    {
      taskGraphObject->applyOptimisation("raise_initial_assignments");
      taskGraphObject->applyOptimisation("fusion");
    }

    if(configurationManager.arrayContractionEnabled())
    {
      taskGraphObject->applyOptimisation("array_contraction");
    }

    taskGraphObject->applyOptimisation("malloc_large_arrays");

    taskGraphObject->compile(getTaskCompiler(), true);	

    gettimeofday(&time, NULL);
    const double duration = (time.tv_sec + time.tv_usec/1000000.0) - startTime;
    StatisticsCollector& statsCollector = StatisticsCollector::getStatisticsCollector();
    statsCollector.addCompileTime(duration);
    statsCollector.incrementCompileCount();
  }

  inline void print() const
  {
    const_cast<tg::tuTaskGraph&>(*taskGraphObject).print();
  }

  void execute(const ParameterHolder& parameterHolder)
  {
    parameterHolder.setParameters(*taskGraphObject);
    taskGraphObject->execute();
  }

  void accept(TGExpressionNodeVisitor<T_element>& visitor)
  {
    std::for_each(exprVector.begin(), exprVector.end(), ApplyVisitor< TGExpressionNodeVisitor<T_element> >(visitor));
  }

  bool operator==(const TGExpressionGraph& right) const
  {
    if (exprVector.size() != right.exprVector.size())
    {
      return false;
    }
    else
    {
      std::map<const TGExpressionNode<T_element>*, const TGExpressionNode<T_element>*> mappings;
      for(std::size_t index = 0; index<exprVector.size(); ++index)
      {
        mappings[exprVector[index]] = right.exprVector[index];
      }

      TGEqualityCheckingVisitor<T_element> checker(mappings);
      const_cast<TGExpressionGraph<T_element>&>(*this).accept(checker);
      return checker.isEqual();
    }
  }

  friend std::size_t hash_value(const TGExpressionGraph<T_element>& graph)
  {
    if (!graph.isHashCached)
    {
      graph.isHashCached = true;
      std::map<const TGExpressionNode<T_element>*, int> nodeNumberings;

      for(std::size_t index=0; index<graph.exprVector.size(); ++index)
        nodeNumberings[graph.exprVector[index]] = index;

      TGHashingVisitor<T_element> hasher(nodeNumberings);
      const_cast<TGExpressionGraph<T_element>&>(graph).accept(hasher);
      graph.cachedHash = hasher.getHash();
    }
    return graph.cachedHash;
  }

  void replaceDependency(const TGOutputReference<tg_scalar, T_element>& previous, TGOutputReference<tg_scalar, T_element>& next)
  {
    replaceDependencyImpl(previous, next);
  }

  void replaceDependency(const TGOutputReference<tg_vector, T_element>& previous, TGOutputReference<tg_vector, T_element>& next)
  {
    replaceDependencyImpl(previous, next);
  }

  void replaceDependency(const TGOutputReference<tg_matrix, T_element>& previous, TGOutputReference<tg_matrix, T_element>& next)
  {
    replaceDependencyImpl(previous, next);
  }

  void removeNodes(const std::set<TGExpressionNode<T_element>*>& nodes)
  {
    std::vector<TGExpressionNode<T_element>*> newExprVector;
    newExprVector.reserve(exprVector.size());

    BOOST_FOREACH(TGExpressionNode<T_element>* node, exprVector)
    {
      if (nodes.find(node) == nodes.end())
      {
        newExprVector.push_back(node);
      }
      else
      {
        delete node;
      }
    }

    exprVector.swap(newExprVector);
  }

  void sort()
  {
    std::vector<TGExpressionNode<T_element>*> newExprVector(getTopologicalSort());
    exprVector.swap(newExprVector);
  }

  ~TGExpressionGraph()
  {
    // We delete in reverse order because the nodes throw an assertion failure if they're deleted
    // while something still depends on them.
    BOOST_FOREACH(TGExpressionNode<T_element>* node, std::make_pair(exprVector.rbegin(), exprVector.rend()))
    {
      delete node;
    }
  }
};

}

}
#endif
