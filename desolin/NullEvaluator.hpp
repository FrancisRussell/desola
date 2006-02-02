#ifndef DESOLIN_NULL_EVALUATOR_HPP
#define DESOLIN_NULL_EVALUATOR_HPP

#include <vector>
#include <set>
#include <boost/shared_ptr.hpp>
#include <desolin/Desolin_fwd.hpp>

namespace desolin_internal
{

template<typename T_element>
class NullEvaluator : private LiteralVisitor<T_element>, public Evaluator<T_element>
{
private:
  NullEvaluator(const NullEvaluator&);
  NullEvaluator& operator=(const NullEvaluator&);
  
  EvaluationStrategy<T_element>& strategy;
  std::vector<Literal<scalar, T_element>*> scalarLiterals;
  std::vector<Literal<vector, T_element>*> vectorLiterals;
  std::vector<Literal<matrix, T_element>*> matrixLiterals;
  
  template<ExprType exprType>
  void addMappings(std::vector<Literal<exprType, T_element>*>& literals)
  {
    for(typename std::vector<Literal<exprType, T_element>*>::const_iterator iterator = literals.begin(); iterator!=literals.end(); ++iterator)
    {
      strategy.addEvaluatedExprMapping(*iterator, *iterator);
    }
  }
  
public:
  NullEvaluator(EvaluationStrategy<T_element>& s) : strategy(s)
  {
  }

  virtual std::set<ExpressionNode<T_element>*> claimNodes(const std::vector<ExpressionNode<T_element>*>& sortedUnclaimed)
  {
    for(typename std::vector<ExpressionNode<T_element>*>::const_iterator iterator = sortedUnclaimed.begin(); iterator!=sortedUnclaimed.end(); ++iterator)
    {
      (*iterator)->accept(*this);
    }
    
    std::set<ExpressionNode<T_element>*> claimed;
    claimed.insert(scalarLiterals.begin(), scalarLiterals.end());
    claimed.insert(vectorLiterals.begin(), vectorLiterals.end());
    claimed.insert(matrixLiterals.begin(), matrixLiterals.end());	    
    return claimed;
  }

  virtual void generateEvaluatedNodes()
  {
    addMappings(scalarLiterals);
    addMappings(vectorLiterals);
    addMappings(matrixLiterals);
  }
  
  void visit(Literal<scalar, T_element>& l)
  {
    scalarLiterals.push_back(&l);
  }

  void visit(Literal<vector, T_element>& l)
  {
    vectorLiterals.push_back(&l);
  }

  void visit(Literal<matrix, T_element>& l)
  {
    matrixLiterals.push_back(&l);
  }

  virtual void evaluate()
  {
  }
};

template<typename T_element>
class NullEvaluatorFactory : public EvaluatorFactory<T_element>
{
  virtual boost::shared_ptr< Evaluator<T_element> > createEvaluator(EvaluationStrategy<T_element>& strategy)
  {
    return boost::shared_ptr< NullEvaluator<T_element> >(new NullEvaluator<T_element>(strategy));
  }
};

}
#endif
