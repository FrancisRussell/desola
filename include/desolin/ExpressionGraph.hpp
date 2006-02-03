#ifndef DESOLIN_EXPRESSION_GRAPH_HPP
#define DESOLIN_EXPRESSION_GRAPH_HPP

#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <algorithm>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <desolin/Desolin_fwd.hpp>

namespace desolin_internal
{

template<typename T_elementType>
class ExpressionGraph
{
private:
  std::vector< ExpressionNode<T_elementType>* >  exprVector;

public:
  template<typename InputIterator>
  ExpressionGraph(const InputIterator& start, const InputIterator& end)
  {
    std::back_insert_iterator< std::vector<ExpressionNode<T_elementType>*> > out(exprVector);
    std::copy(start, end, out);
  }

  inline std::vector< ExpressionNode<T_elementType>* > getSortedNodes() const
  {
    return exprVector;
  }
  
  inline int nodeCount() const
  {
    return exprVector.size();
  }

  template<typename VisitorType>
  static void applyVisitor(ExpressionNode<T_elementType>* node, VisitorType& visitor)
  {
    node->accept(visitor);
  }
  
  template<typename VisitorType>
  inline void internalAccept(VisitorType& visitor)
  {
    std::for_each(exprVector.begin(), exprVector.end(), boost::bind(applyVisitor<VisitorType>, _1, boost::ref(visitor)));
  }
  
  void accept(ExpressionNodeVisitor<T_elementType>& visitor)
  {
    internalAccept(visitor);
  }

  void accept(LiteralVisitor<T_elementType>& visitor)
  {
    internalAccept(visitor);
  }

  void accept(ExpressionNodeTypeVisitor<T_elementType>& visitor)
  {
    internalAccept(visitor);
  }
   
  boost::shared_ptr<EvaluationStrategy<T_elementType> > createEvaluationStrategy()
  {
    return boost::shared_ptr<EvaluationStrategy<T_elementType> > (new EvaluationStrategy<T_elementType>(*this));
  }
};

}
#endif
