#ifndef DESOLIN_TG_EXPRESSION_GRAPH_HPP
#define DESOLIN_TG_EXPRESSION_GRAPH_HPP

#include <algorithm>
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
  TGExpressionGraph(const TGExpressionGraph&);
  TGExpressionGraph& operator=(const TGExpressionGraph&);
  
  std::map<TGExpressionNode<T_element>*, int> nodeNumberings;
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

public:
  TGExpressionGraph()
  {
  }
  
  inline void add(TGExpressionNode<T_element>* value)
  {
    exprVector.push_back(value);
    nodeNumberings[value] = nodeNumberings.size();
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
    taskGraphObject.print();
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

  std::size_t hashValue() const
  {
    std::vector<std::size_t> hashes(exprVector.size());
    std::transform(exprVector.begin(), exprVector.end(), 
		   std::back_insert_iterator< std::vector<std::size_t> >(hashes), 
		   boost::bind(&TGExpressionNode<T_element>::hashValue, _1, boost::cref(nodeNumberings)));

    return boost::hash_range(hashes.begin(), hashes.end());
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
