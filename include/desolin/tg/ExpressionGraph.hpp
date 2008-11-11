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

#ifndef DESOLIN_TG_EXPRESSION_GRAPH_HPP
#define DESOLIN_TG_EXPRESSION_GRAPH_HPP

#include <algorithm>
#include <functional>
#include <cstddef>
#include <cassert>
#include <map>
#include <memory>
#include <sys/time.h>
#include <TaskGraph>
#include <desolin/tg/Desolin_tg_fwd.hpp>
#include "HighLevelFuser.hpp"
#include <boost/foreach.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/scoped_ptr.hpp>

namespace desolin
{

namespace detail
{

template<typename T_element>
class TGExpressionGraph
{
private:
  TGExpressionGraph(const TGExpressionGraph&);
  TGExpressionGraph& operator=(const TGExpressionGraph&);
  
  boost::ptr_vector< TGExpressionNode<T_element> >  exprVector;
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

    inline void operator()(TGExpressionNode<T_element>& node) const
    {
      node.accept(visitor);
    }
  };

public:
  TGExpressionGraph() : taskGraphObject(NULL), isHashCached(false)
  {
  }

  inline void add(TGExpressionNode<T_element>* value)
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
      for(typename boost::ptr_vector< TGExpressionNode<T_element> >::iterator exprVectorIter(exprVector.begin()); exprVectorIter!=exprVector.end(); ++exprVectorIter)
        exprVectorIter->createTaskGraphVariable();

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
        mappings[&exprVector[index]] = &right.exprVector[index];
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
        nodeNumberings[&graph.exprVector[index]] = index;

      TGHashingVisitor<T_element> hasher(nodeNumberings);
      const_cast<TGExpressionGraph<T_element>&>(graph).accept(hasher);
      graph.cachedHash = hasher.getHash();
    }
    return graph.cachedHash;
  }

  void replaceDependency(const TGOutputReference<tg_scalar, T_element>& previous, TGOutputReference<tg_scalar, T_element>& next)
  {
    BOOST_FOREACH(TGExpressionNode<T_element>& node, exprVector)
    {
      node.replaceDependency(previous, next);
    }
  }

  void replaceDependency(const TGOutputReference<tg_vector, T_element>& previous, TGOutputReference<tg_vector, T_element>& next)
  {
    BOOST_FOREACH(TGExpressionNode<T_element>& node, exprVector)
    {
      node.replaceDependency(previous, next);
    }
  }

  void replaceDependency(const TGOutputReference<tg_matrix, T_element>& previous, TGOutputReference<tg_matrix, T_element>& next)
  {
    BOOST_FOREACH(TGExpressionNode<T_element>& node, exprVector)
    {
      node.replaceDependency(previous, next);
    }
  }

  void replaceNodes(const std::set<TGExpressionNode<T_element>*>& oldNodes, TGExpressionNode<T_element>* newNode)
  {
    boost::ptr_vector< TGExpressionNode<T_element> > newExprVector;
    bool firstMatch = true;
  
    while(!exprVector.empty())
    {
      typename boost::ptr_vector< TGExpressionNode<T_element> >::auto_type node = exprVector.pop_back();

      if (oldNodes.find(node.get()) == oldNodes.end())
      {
        newExprVector.push_back(node.release());
      }
      else
      {
        // We replace the final matching node in the list with the new node.
        // This should keep the topological sort valid without having to completely
        // re-sort the DAG.
        if (firstMatch)
          newExprVector.push_back(newNode);

        firstMatch = false;
      }
    }

    // There doesn't seem to be an efficient way to reverse a ptr_vector
    while(!newExprVector.empty())
      exprVector.push_back(newExprVector.pop_back().release());
  }
};

}

}
#endif
