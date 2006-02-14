#ifndef DESOLIN_ELEMENTAL_HPP
#define DESOLIN_ELEMENTAL_HPP

#include <map>
#include <algorithm>
#include <boost/bind.hpp>
#include <desolin/Desolin_fwd.hpp>

namespace desolin_internal
{

template<ExprType exprType, typename T_element>
class ElementGet : public UnOp<scalar, exprType, T_element>
{
private:
  const ElementIndex<exprType> index;

public:
  ElementGet(ExprNode<exprType, T_element>& e, const ElementIndex<exprType>& i) : UnOp<scalar, exprType, T_element>(boost::array<int, 0>(), e), index(i)
  {
  }
  
  inline const ElementIndex<exprType>& getIndex() const
  { 
    return index;
  }
  
  virtual void accept(ExpressionNodeVisitor<T_element>& visitor)
  { 
    visitor.visit(*this);
  }

  virtual void internal_evaluate()
  {
    this->getOperand().evaluate();
  }

  virtual T_element getElementValue()
  {
    return this->getOperand().getElementValue(index);
  }   
};

template<ExprType exprType, typename T_element>
class ElementSet : public UnOp<exprType, exprType, T_element>
{
private:
  const std::map<ElementIndex<exprType>, ExprNode<scalar, T_element>*> assignments;

  void registerAssignmentDependency(const std::pair<ElementIndex<exprType>, ExprNode<scalar, T_element>*>& pair)
  {
    this->registerDependency(pair.second);
  }
  
public:
  ElementSet(ExprNode<exprType, T_element>& e, const std::map<ElementIndex<exprType>, ExprNode<scalar, T_element>*>& a) : UnOp<exprType, exprType, T_element>(e.getDims(), e), assignments(a)
  {
    std::for_each(assignments.begin(), assignments.end(), boost::bind(&ElementSet::registerAssignmentDependency, this, _1));
  }

  inline std::map<ElementIndex<exprType>, ExprNode<scalar, T_element>*> getAssignments() const
  {
    return assignments;
  }
  
  virtual void accept(ExpressionNodeVisitor<T_element>& visitor)
  { 
    visitor.visit(*this);
  }
};

}
#endif
