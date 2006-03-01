#ifndef DESOLIN_TG_HASHING_VISITOR_HPP
#define DESOLIN_TG_HASHING_VISITOR_HPP

#include <typeinfo>
#include <cassert>
#include <vector>
#include <map>
#include <algorithm>
#include <boost/functional/hash.hpp>
#include <desolin/tg/Desolin_tg_fwd.hpp>

namespace desolin_internal
{

template<typename T_element>
class TGHashingVisitor : public TGExpressionNodeVisitor<T_element>
{
private:
  const std::map<const TGExpressionNode<T_element>*, int> nodeNumberings;
  std::size_t hash;

  template<TGExprType exprType>
  inline std::size_t hashExprNode(const TGExprNode<exprType, T_element>& node) const
  {
    std::size_t seed = boost::hash<std::string>()(typeid(node).name());
    boost::hash_combine(seed, node.getInternal().hashValue());
    return seed;
  }
  
  template<TGExprType resultType, TGExprType exprType>
  std::size_t hashUnOp(const TGUnOp<resultType, exprType, T_element>& unop) const
  {
    std::size_t seed = hashExprNode(unop);
    const typename std::map<const TGExpressionNode<T_element>*, int>::const_iterator operand = nodeNumberings.find(&unop.getOperand());
    assert(operand != nodeNumberings.end());
    boost::hash_combine(seed, operand->second);
    return seed;
  }

  template<TGExprType resultType, TGExprType leftType, TGExprType rightType>
  std::size_t hashBinOp(const TGBinOp<resultType, leftType, rightType, T_element>& binop) const
  {
    std::size_t seed = hashExprNode(binop);
    const typename std::map<const TGExpressionNode<T_element>*, int>::const_iterator left = nodeNumberings.find(&binop.getLeft());
    const typename std::map<const TGExpressionNode<T_element>*, int>::const_iterator right = nodeNumberings.find(&binop.getRight());
    assert(left != nodeNumberings.end());
    assert(right != nodeNumberings.end());
    boost::hash_combine(seed, left->second);
    boost::hash_combine(seed, right->second);
    return seed;
  }

  template<TGExprType exprType>
  std::size_t hashElementGet(const TGElementGet<exprType, T_element>& node) const
  {
    std::size_t seed = hashUnOp(node);
    boost::hash_combine(seed, node.getIndex());
    return seed;
  }

  template<TGExprType exprType>
  std::size_t hashSingleElementSet(const std::pair<const TGElementIndex<exprType>, const TGExprNode<tg_scalar, T_element>*>& pair) const
  {
    std::size_t seed = boost::hash< TGElementIndex<exprType> >()(pair.first);
    const typename std::map<const TGExpressionNode<T_element>*, int>::const_iterator element = nodeNumberings.find(pair.second);
    assert(element != nodeNumberings.end());
    boost::hash_combine(seed, nodeNumberings.find(pair.second)->second);
    return seed;
  }
  
  template<TGExprType exprType>
  std::size_t hashElementSet(const TGElementSet<exprType, T_element>& node) const
  {
    std::size_t seed = hashUnOp(node);
    typedef std::map<TGElementIndex<exprType>, const TGExprNode<tg_scalar, T_element>*> T_assignmentMap;
    const T_assignmentMap assignments(node.getAssignments());

    for(typename T_assignmentMap::const_iterator i = assignments.begin(); i != assignments.end(); ++i)
    {
      boost::hash_combine(seed, hashSingleElementSet(*i));
    }
    return seed;
  }

  template<TGExprType exprType>
  std::size_t hashPairwise(const TGPairwise<exprType, T_element>& node) const
  {
    std::size_t seed = hashBinOp(node);
    boost::hash_combine(seed, node.getOperation());
    return seed;
  }

  template<TGExprType exprType>
  std::size_t hashScalarPiecewise(const TGScalarPiecewise<exprType, T_element>& node) const
  {
    std::size_t seed = hashBinOp(node);
    boost::hash_combine(seed, node.getOperation());
    return seed;
  } 
  
public:
  TGHashingVisitor(const std::map<const TGExpressionNode<T_element>*, int>& numberings) : nodeNumberings(numberings), hash(0)
  {
  }

  inline std::size_t getHash() const
  {
    return hash;
  }
  
  virtual void visit(TGElementGet<tg_vector, T_element>& e)
  {
    boost::hash_combine(hash, hashElementGet(e));
  }
  
  virtual void visit(TGElementGet<tg_matrix, T_element>& e)
  {
    boost::hash_combine(hash, hashElementGet(e));
  }

  virtual void visit(TGElementSet<tg_vector, T_element>& e)
  {
    boost::hash_combine(hash, hashElementSet(e));
  }
  
  virtual void visit(TGElementSet<tg_matrix, T_element>& e)
  {
    boost::hash_combine(hash, hashElementSet(e));
  }

  virtual void visit(TGLiteral<tg_scalar, T_element>& e)
  {
    boost::hash_combine(hash, hashExprNode(e));
  }
  
  virtual void visit(TGLiteral<tg_vector, T_element>& e)
  {
    boost::hash_combine(hash, hashExprNode(e));
  }
  
  virtual void visit(TGLiteral<tg_matrix, T_element>& e)
  { 
    boost::hash_combine(hash, hashExprNode(e));
  }

  virtual void visit(TGMatrixMult<T_element>& e)
  {
    boost::hash_combine(hash, hashBinOp(e));
  }
  
  virtual void visit(TGMatrixVectorMult<T_element>& e)
  {
    boost::hash_combine(hash, hashBinOp(e));
  }

  virtual void visit(TGTransposeMatrixVectorMult<T_element>& e)
  {
    boost::hash_combine(hash, hashBinOp(e));
  } 
  
  virtual void visit(TGVectorDot<T_element>& e)
  {
    boost::hash_combine(hash, hashBinOp(e));
  }
  
  virtual void visit(TGVectorCross<T_element>& e)
  {
    boost::hash_combine(hash, hashBinOp(e));
  }
  
  virtual void visit(TGVectorNorm<T_element>& e)
  {
    boost::hash_combine(hash, hashUnOp(e));
  }
  
  virtual void visit(TGMatrixTranspose<T_element>& e)
  {
    boost::hash_combine(hash, hashUnOp(e));
  }

  virtual void visit(TGPairwise<tg_scalar, T_element>& e)
  {
    boost::hash_combine(hash, hashPairwise(e));
  }
  
  virtual void visit(TGPairwise<tg_vector, T_element>& e)
  {
    boost::hash_combine(hash, hashPairwise(e));
  }
  
  virtual void visit(TGPairwise<tg_matrix, T_element>& e)
  {
    boost::hash_combine(hash, hashPairwise(e));
  }
  
  virtual void visit(TGScalarPiecewise<tg_scalar, T_element>& e)
  {
    boost::hash_combine(hash, hashScalarPiecewise(e));
  }
  
  virtual void visit(TGScalarPiecewise<tg_vector, T_element>& e)
  {
    boost::hash_combine(hash, hashScalarPiecewise(e)); 
  }
  
  virtual void visit(TGScalarPiecewise<tg_matrix, T_element>& e)
  {
    boost::hash_combine(hash, hashScalarPiecewise(e));
  }

  virtual void visit(TGNegate<tg_scalar, T_element>& e)
  {
    boost::hash_combine(hash, hashUnOp(e));
  }
  
  virtual void visit(TGNegate<tg_vector, T_element>& e)
  {
    boost::hash_combine(hash, hashUnOp(e));
  }
  
  virtual void visit(TGNegate<tg_matrix, T_element>& e)
  {
    boost::hash_combine(hash, hashUnOp(e));
  }

  virtual void visit(TGAbsolute<T_element>& e)
  {
    boost::hash_combine(hash, hashUnOp(e));
  }
  
  virtual void visit(TGSquareRoot<T_element>& e)
  {
    boost::hash_combine(hash, hashUnOp(e));
  }
};

}
#endif
