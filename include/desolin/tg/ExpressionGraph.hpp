#ifndef DESOLIN_TG_EXPRESSION_GRAPH_HPP
#define DESOLIN_TG_EXPRESSION_GRAPH_HPP

#include <iostream>
#include <algorithm>
#include <boost/shared_ptr.hpp>
#include <boost/functional/hash.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <boost/function.hpp>
#include <vector>
#include <map>
#include <TaskGraph>
#include <desolin/tg/Desolin_tg_fwd.hpp>

namespace desolin_internal
{

template<typename T_element>
class TGExpressionGraph
{
private:
  static std::map<std::size_t, boost::shared_ptr< TGExpressionGraph > > cachedGraphs;

  TGExpressionGraph(const TGExpressionGraph&);
  TGExpressionGraph& operator=(const TGExpressionGraph&);
  
  std::vector< TGExpressionNode<T_element>* >  exprVector;
  tg::tuTaskGraph taskGraphObject;
  NameGenerator generator;

  mutable bool isHashCached;
  mutable std::size_t cachedHash;
  
  void generateCode()
  {
    TGCodeGenerator<T_element> codeGenerator(*this);
    accept(codeGenerator);
  }

  static void applyVisitor(TGExpressionNode<T_element>* node, TGExpressionNodeVisitor<T_element>& visitor)
  {
    node->accept(visitor);
  }

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

  inline void print() const
  {
    const_cast<tg::tuTaskGraph&>(taskGraphObject).print();
  }

  void execute(const ParameterHolder& parameterHolder)
  {
    taskGraphObject.applyOptimisation("fusion");
    taskGraphObject.compile(tg::GCC, false);
    parameterHolder.setParameters(taskGraphObject);
    taskGraphObject.execute();
  }

  void accept(TGExpressionNodeVisitor<T_element>& visitor)
  {
    std::for_each(exprVector.begin(), exprVector.end(), boost::bind(applyVisitor, _1, boost::ref(visitor)));
  }

  bool operator==(const TGExpressionGraph& right) const
  {
    if (exprVector.size() != right.exprVector.size())
    {
      return false;
    }
    else
    {
      std::map<TGExpressionNode<T_element>*, TGExpressionNode<T_element>*> mappings;

      for(int index = 0; index<exprVector.size(); ++index)
      {
        mappings[exprVector[index]] = right.exprVector[index];
      }

      for(int index = 0; index<exprVector.size(); ++index)
      {
        if (!exprVector[index]->matches(*right.exprVector[index], mappings))
          return false;
      }
      return true;
    }
  }

  friend std::size_t hash_value(const TGExpressionGraph<T_element>& graph)
  {
    if (!graph.isHashCached)
    {
      std::map<const TGExpressionNode<T_element>*, int> nodeNumberings;
      for(std::size_t index=0; index<graph.exprVector.size(); ++index)
      {
        nodeNumberings[graph.exprVector[index]] = index;
      }

      TGHashingVisitor<T_element> hasher(nodeNumberings);
      const_cast<TGExpressionGraph<T_element>&>(graph).accept(hasher);
      graph.cachedHash = hasher.getHash();
    }
    return graph.cachedHash;
  }

  ~TGExpressionGraph()
  {
    for(typename std::vector<TGExpressionNode<T_element>*>::iterator i = exprVector.begin(); i!=exprVector.end(); ++i)
    {
      delete *i;
    }
  }
};

template<typename T_element>
std::map<std::size_t, boost::shared_ptr< TGExpressionGraph<T_element> > > TGExpressionGraph<T_element>::cachedGraphs;

}
#endif
