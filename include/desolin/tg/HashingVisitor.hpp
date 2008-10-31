/****************************************************************************/
/* Copyright 2005-2006, Francis Russell                                     */
/*                                                                          */
/* Licensed under the Apache License, Version 2.0 (the License);            */
/* you may not use this file except in compliance with the License.         */
/* You may obtain a copy of the License at                                  */
/*                                                                          */
/*     http://www.apache.org/licenses/LICENSE-2.0                           */
/*                                                                          */
/* Unless required by applicable law or agreed to in writing, software      */
/* distributed under the License is distributed on an AS IS BASIS,          */
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. */
/* See the License for the specific language governing permissions and      */
/* limitations under the License.                                           */
/*                                                                          */
/****************************************************************************/

#ifndef DESOLIN_TG_HASHING_VISITOR_HPP
#define DESOLIN_TG_HASHING_VISITOR_HPP

#include <utility>
#include <typeinfo>
#include <cassert>
#include <cstddef>
#include <cstring>
#include <string>
#include <map>
#include <boost/functional/hash.hpp>
#include <desolin/tg/Desolin_tg_fwd.hpp>

namespace desolin
{

namespace detail
{

template<typename T_element>
class TGHashingVisitor : public TGExpressionNodeVisitor<T_element>
{
private:
  const std::map<const TGExpressionNode<T_element>*, int> nodeNumberings;
  std::size_t hash;

  template<typename exprType>
  inline std::size_t hashOutputReference(const TGOutputReference<exprType, T_element>& ref) const
  {
    const typename std::map<const TGExpressionNode<T_element>*, int>::const_iterator nodeNumbering 
      = nodeNumberings.find(ref.getExpressionNode());
    assert(nodeNumbering != nodeNumberings.end());

    std::size_t seed = boost::hash<int>()(nodeNumbering->second);
    boost::hash_combine(seed, ref.getIndex());

    return seed;
  }

  template<typename exprType>
  inline std::size_t hashExprNode(const TGExprNode<exprType, T_element>& node) const
  {
    const char* nodeTypeString = typeid(node).name();
    std::size_t seed = boost::hash_range(nodeTypeString, nodeTypeString+strlen(nodeTypeString));
    boost::hash_combine(seed, node.getInternal().hashValue());
    return seed;
  }
  
  template<typename resultType, typename exprType>
  std::size_t hashUnOp(const TGUnOp<resultType, exprType, T_element>& unop) const
  {
    std::size_t seed = hashExprNode(unop);
    boost::hash_combine(seed, hashOutputReference(unop.getOperand()));
    return seed;
  }

  template<typename resultType, typename leftType, typename rightType>
  std::size_t hashBinOp(const TGBinOp<resultType, leftType, rightType, T_element>& binop) const
  {
    std::size_t seed = hashExprNode(binop);
    boost::hash_combine(seed, hashOutputReference(binop.getLeft()));
    boost::hash_combine(seed, hashOutputReference(binop.getRight()));
    return seed;
  }

  template<typename exprType>
  std::size_t hashElementGet(const TGElementGet<exprType, T_element>& node) const
  {
    std::size_t seed = hashUnOp(node);
    boost::hash_combine(seed, node.getIndex());
    return seed;
  }

  template<typename exprType>
  std::size_t hashSingleElementSet(const std::pair<const TGElementIndex<exprType>, const TGOutputReference<tg_scalar, T_element> >& pair) const
  {
    std::size_t seed = boost::hash< TGElementIndex<exprType> >()(pair.first);
    boost::hash_combine(seed, hashOutputReference(pair.second));
    return seed;
  }
  
  template<typename exprType>
  std::size_t hashElementSet(const TGElementSet<exprType, T_element>& node) const
  {
    std::size_t seed = hashUnOp(node);
    typedef std::map<TGElementIndex<exprType>, const TGOutputReference<tg_scalar, T_element> > T_assignmentMap;
    const T_assignmentMap assignments(node.getAssignments());

    for(typename T_assignmentMap::const_iterator i = assignments.begin(); i != assignments.end(); ++i)
    {
      boost::hash_combine(seed, hashSingleElementSet(*i));
    }
    return seed;
  }

  template<typename exprType>
  std::size_t hashPairwise(const TGPairwise<exprType, T_element>& node) const
  {
    std::size_t seed = hashBinOp(node);
    boost::hash_combine(seed, node.getOperation());
    return seed;
  }

  template<typename exprType>
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
    boost::hash_combine(hash, e.isTranspose());
  }

  virtual void visit(TGMatrixMultiVectorMult<T_element>& e)
  {
    //FIXME: Implement me!
  }

  virtual void visit(TGVectorDot<T_element>& e)
  {
    boost::hash_combine(hash, hashBinOp(e));
  }
  
  virtual void visit(TGVectorCross<T_element>& e)
  {
    boost::hash_combine(hash, hashBinOp(e));
  }
  
  virtual void visit(TGVectorTwoNorm<T_element>& e)
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

}
#endif
