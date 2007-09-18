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
#include <boost/ptr_container/ptr_vector.hpp>
#include <functional>
#include <cstddef>
#include <boost/function.hpp>
#include <map>
#include <TaskGraph>
#include <desolin/tg/Desolin_tg_fwd.hpp>
#include <sys/time.h>

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
  tg::tuTaskGraph taskGraphObject;
  NameGenerator generator;

  mutable bool isHashCached;
  mutable std::size_t cachedHash;
  
  void generateCode()
  {
    TGCodeGenerator<T_element> codeGenerator(*this);
    accept(codeGenerator);
  }

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
  TGExpressionGraph() : isHashCached(false)
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

  void generateCode(boost::function<void ()>& generateNodes)
  {
    tu_taskgraph(taskGraphObject)
    {
      generateNodes();
      generateCode(); 
    };
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
      taskGraphObject.applyOptimisation("fusion");
    }

    if(configurationManager.arrayContractionEnabled())
    {
      taskGraphObject.applyOptimisation("array_contraction");
    }

    taskGraphObject.compile(getTaskCompiler(), true);	

    gettimeofday(&time, NULL);
    const double duration = (time.tv_sec + time.tv_usec/1000000.0) - startTime;
    StatisticsCollector& statsCollector = StatisticsCollector::getStatisticsCollector();
    statsCollector.addCompileTime(duration);
    statsCollector.incrementCompileCount();
  }

  inline void print() const
  {
    const_cast<tg::tuTaskGraph&>(taskGraphObject).print();
  }

  void execute(const ParameterHolder& parameterHolder)
  {
    parameterHolder.setParameters(taskGraphObject);
    taskGraphObject.execute();
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
      {
        nodeNumberings[&graph.exprVector[index]] = index;
      }

      TGHashingVisitor<T_element> hasher(nodeNumberings);
      const_cast<TGExpressionGraph<T_element>&>(graph).accept(hasher);
      graph.cachedHash = hasher.getHash();
    }
    return graph.cachedHash;
  }
};

}

}
#endif
