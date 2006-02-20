#ifndef DESOLIN_TG_ELEMENTAL_HPP
#define DESOLIN_TG_ELEMENTAL_HPP

#include <map>
#include <algorithm>
#include <vector>
#include <cassert>
#include <boost/functional/hash.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>
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

  inline virtual std::size_t hashValue(const std::map<TGExpressionNode<T_element>*, int>& nodeNumberings) const
  {
    std::size_t seed = TGUnOp<tg_scalar, exprType, T_element>::hashValue(nodeNumberings);
    boost::hash_combine(seed, index);
    return seed;
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

  static std::size_t calculateHash(const std::map<TGExpressionNode<T_element>*, int>& nodeNumberings, 
		                   const std::pair<const TGElementIndex<exprType>, TGExprNode<tg_scalar, T_element>*>& pair)
  {
    std::size_t seed = boost::hash< TGElementIndex<exprType> >()(pair.first);
    assert(nodeNumberings.find(pair.second) != nodeNumberings.end());
    boost::hash_combine(seed, nodeNumberings.find(pair.second)->second);
    return seed;
  }
  
  inline virtual std::size_t hashValue(const std::map<TGExpressionNode<T_element>*, int>& nodeNumberings) const
  {
    std::size_t seed = TGUnOp<exprType, exprType, T_element>::hashValue(nodeNumberings);
    std::vector<std::size_t> hashes(assignments.size());
    std::transform(assignments.begin(), assignments.end(), 
		   std::back_insert_iterator< std::vector<std::size_t> >(hashes), 
		   boost::bind(&TGElementSet<exprType, T_element>::calculateHash, boost::cref(nodeNumberings), _1));
    boost::hash_combine(seed, boost::hash_range(hashes.begin(), hashes.end()));
    return seed;
  }  
};

}
#endif
