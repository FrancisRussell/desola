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

namespace desolin_internal
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

  void compile()
  {
    taskGraphObject.applyOptimisation("fusion");
    taskGraphObject.applyOptimisation("array_contraction");
    taskGraphObject.compile(tg::GCC, true);	
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
#endif
