#ifndef DESOLIN_TG_ELEMENTAL_HPP
#define DESOLIN_TG_ELEMENTAL_HPP

#include <map>
#include <algorithm>
#include <boost/bind.hpp>
#include <desolin/tg/Desolin_tg_fwd.hpp>

namespace desolin_internal
{

template<TGExprType exprType, typename T_element>
class TGElementGet : public TGUnOp<tg_scalar, exprType, T_element>
{
protected:
  const TGElementIndex<exprType> index;
  
public:
  TGElementGet(typename TGInternalType<tg_scalar, T_element>::type* internal, 
		  TGExprNode<exprType, T_element>& o, 
		  const TGElementIndex<exprType> i) :  TGUnOp<tg_scalar, exprType, T_element>(internal, o), 
								     index(i)
  {
  }

  inline const TGElementIndex<exprType>& getIndex() const
  {
    return index;
  }

  virtual void accept(TGExpressionNodeVisitor<T_element>& v)
  {
    v.visit(*this);
  }
};

template<TGExprType exprType, typename T_element>
class TGElementSet : public TGUnOp<exprType, exprType, T_element>
{
private:
  const std::map<TGElementIndex<exprType>, TGExprNode<tg_scalar, T_element>*> assignments;

  void registerDependency(const std::pair<const TGElementIndex<exprType>, TGExprNode<tg_scalar, T_element>*>& pair)
  {
    this->dependencies.insert(pair.second);
  }
  
public:
  TGElementSet(typename TGInternalType<exprType, T_element>::type* internal,
	       TGExprNode<exprType, T_element>& o,
	       std::map<TGElementIndex<exprType>, TGExprNode<tg_scalar, T_element>*> a) : TGUnOp<exprType, exprType, T_element>(internal, o), assignments(a)
  {
    std::for_each(assignments.begin(), assignments.end(), boost::bind(&TGElementSet::registerDependency, this, _1));
  }

  std::map<TGElementIndex<exprType>, TGExprNode<tg_scalar, T_element>*> getAssignments() const
  {
    return assignments;
  }

  virtual void accept(TGExpressionNodeVisitor<T_element>& v)
  {
    v.visit(*this);
  }
};

}
#endif
