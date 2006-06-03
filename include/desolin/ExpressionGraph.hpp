#ifndef DESOLIN_EXPRESSION_GRAPH_HPP
#define DESOLIN_EXPRESSION_GRAPH_HPP

#include <algorithm>
#include <vector>
#include <functional>
#include <cstddef>
#include <boost/shared_ptr.hpp>
#include <desolin/Desolin_fwd.hpp>

namespace desolin_internal
{

template<typename T_element>
class ExpressionGraph
{
private:
  std::vector< ExpressionNode<T_element>* >  exprVector;

  template<typename VisitorType>
  class ApplyVisitor : public std::unary_function<void, ExpressionNode<T_element>*>
  {
  private:
    VisitorType& visitor;

  public:
    ApplyVisitor(VisitorType& v) : visitor(v)
    {
    }
    
    inline void operator()(ExpressionNode<T_element>* const node) const
    {
      node->accept(visitor);
    }
  };
			
  template<typename VisitorType>
  inline void internalAccept(VisitorType& visitor)
  {
    std::for_each(exprVector.begin(), exprVector.end(), ApplyVisitor<VisitorType>(visitor));
  }

  static void setDefaultAnnotation(ExpressionNode<T_element>* const node)
  {
    if (node->getExternalRequiredBy().empty())
    {
      node->setEvaluationDirective(NO_EVALUATE);
    }
    else
    {
      node->setEvaluationDirective(EVALUATE);
    }
  }
  
public:
  template<typename InputIterator>
  ExpressionGraph(const InputIterator& start, const InputIterator& end) : exprVector(start, end)
  {
  }

  inline std::vector< ExpressionNode<T_element>* > getSortedNodes() const
  {
    return exprVector;
  }
  
  inline int nodeCount() const
  {
    return exprVector.size();
  }

  ExpressionNode<T_element>& nodeAt(const std::size_t index)
  {
    return *exprVector[index];
  }

  void accept(ExpressionNodeVisitor<T_element>& visitor)
  {
    internalAccept(visitor);
  }

  void accept(LiteralVisitor<T_element>& visitor)
  {
    internalAccept(visitor);
  }

  void accept(ExpressionNodeTypeVisitor<T_element>& visitor)
  {
    internalAccept(visitor);
  }

  // Replicates original behavious before profiling system. Only saves result
  // of nodes which have an external handle pointing to them.
  void setDefaultAnnotations()
  {
    std::for_each(exprVector.begin(), exprVector.end(), setDefaultAnnotation);
  }
   
  boost::shared_ptr<EvaluationStrategy<T_element> > createEvaluationStrategy()
  {
    return boost::shared_ptr<EvaluationStrategy<T_element> >(new EvaluationStrategy<T_element>(*this));
  }
};

}
#endif
