#ifndef DESOLIN_TG_EXPRESSION_GRAPH_HPP
#define DESOLIN_TG_EXPRESSION_GRAPH_HPP

#include <algorithm>
#include <boost/shared_ptr.hpp>
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
  TGExpressionGraph(const TGExpressionGraph&);
  TGExpressionGraph& operator=(const TGExpressionGraph&);
  
  std::vector< TGExpressionNode<T_element>* >  exprVector;
  tg::tuTaskGraph taskGraphObject;
  NameGenerator generator;

  void generateCode()
  {
    TGCodeGenerator<T_element> codeGenerator(*this);
    accept(codeGenerator);
  }

  static void applyVisitor(TGExpressionNode<T_element>* node, TGExpressionNodeVisitor<T_element>& visitor)
  {
    node->accept(visitor);
  }

  static void addDependencies(TGExpressionNode<T_element>* node, std::set<TGExpressionNode<T_element>*>& deps)
  {
    const std::set<TGExpressionNode<T_element>*> dependencies(node->getDependencies());
    deps.insert(dependencies.begin(), dependencies.end());
  }

  std::set<TGExpressionNode<T_element>*> getLeaves()
  {
    std::set<TGExpressionNode<T_element>*> all(exprVector.begin(), exprVector.end());
    
    std::set<TGExpressionNode<T_element>*> dependencies;
    std::for_each(all.begin(), all.end(), boost::bind(addDependencies, _1, boost::ref(dependencies)));
    
    std::set<TGExpressionNode<T_element>*> leaves;
    std::insert_iterator< std::set<TGExpressionNode<T_element>*> > leafInserter(leaves, leaves.begin());
    std::set_difference(all.begin(), all.end(), dependencies.begin(), dependencies.end(), leafInserter);
    return leaves;
  }
  
public:
  TGExpressionGraph()
  {
  }
  
  void add(TGExpressionNode<T_element>* value)
  {
    exprVector.push_back(value);
  }

  NameGenerator& getNameGenerator()
  {
    return generator;
  }

  const int size() const
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

  void print() const
  {
    taskGraphObject.print();
  }

  void execute(const ParameterHolder& parameterHolder)
  {
    taskGraphObject.applyOptimisation("fusion");
    taskGraphObject.compile(tg::GCC, true);
    parameterHolder.setParameters(taskGraphObject);
    taskGraphObject.execute();
  }

  void accept(TGExpressionNodeVisitor<T_element>& visitor)
  {
    std::for_each(exprVector.begin(), exprVector.end(), boost::bind(applyVisitor, _1, boost::ref(visitor)));
  }

  ~TGExpressionGraph()
  {
    for(typename std::vector<TGExpressionNode<T_element>*>::iterator i = exprVector.begin(); i!=exprVector.end(); ++i)
    {
      delete *i;
    }
  }
};

}
#endif
