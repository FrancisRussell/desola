#ifndef DESOLIN_EVALUATOR_HPP
#define DESOLIN_EVALUATOR_HPP

#include <set>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <desolin/Desolin_fwd.hpp>

namespace desolin_internal
{

template <typename T_element>
class Evaluator
{
public:
  virtual std::set<ExpressionNode<T_element>*> claimNodes(const std::vector<ExpressionNode<T_element>*>& sortedUnclaimed) = 0;
  virtual void generateEvaluatedNodes() = 0;
  virtual void evaluate() = 0;
  virtual ~Evaluator() {}
};

template<typename T_element>
class EvaluatorFactory
{
public:
  virtual boost::shared_ptr< Evaluator<T_element> > createEvaluator(EvaluationStrategy<T_element>& strategy) = 0;
};

}
#endif
