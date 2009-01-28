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

#ifndef DESOLA_PROFILING_HASHING_VISITOR_HPP
#define DESOLA_PROFILING_HASHING_VISITOR_HPP

#include "Desola_profiling_fwd.hpp"
#include <cstddef>
#include <typeinfo>
#include <string>
#include <cstring>
#include <map>
#include <vector>
#include <cassert>
#include <boost/functional/hash.hpp>

namespace desola
{

namespace detail
{

template<typename T_element>
class PHashingVisitor : public PExpressionNodeVisitor<T_element>
{
private:
  const std::map<const PExpressionNode<T_element>*, int> nodeNumberings;
  std::size_t hash;
 
  template<typename exprType>
  inline std::size_t hashExprNode(const PExprNode<exprType, T_element>& node) const
  {
    const char* nodeTypeString = typeid(node).name();
    return boost::hash_range(nodeTypeString, nodeTypeString+strlen(nodeTypeString));
  }

  template<typename resultType, typename exprType>
  std::size_t hashUnOp(const PUnOp<resultType, exprType, T_element>& unop) const
  {
    std::size_t seed = hashExprNode(unop);
    const typename std::map<const PExpressionNode<T_element>*, int>::const_iterator operand = nodeNumberings.find(&unop.getOperand());
    assert(operand != nodeNumberings.end());
    boost::hash_combine(seed, operand->second);
    return seed;
  }

  template<typename resultType, typename leftType, typename rightType>
  std::size_t hashBinOp(const PBinOp<resultType, leftType, rightType, T_element>& binop) const
  {
    std::size_t seed = hashExprNode(binop);
    const typename std::map<const PExpressionNode<T_element>*, int>::const_iterator left = nodeNumberings.find(&binop.getLeft());
    const typename std::map<const PExpressionNode<T_element>*, int>::const_iterator right = nodeNumberings.find(&binop.getRight());
    assert(left != nodeNumberings.end());
    assert(right != nodeNumberings.end());
    boost::hash_combine(seed, left->second);
    boost::hash_combine(seed, right->second);
    return seed;
  }

  template<typename exprType>
  std::size_t hashElementSet(const PElementSet<exprType, T_element>& node) const
  {
    std::size_t seed = hashUnOp(node);
    const std::vector<const PExprNode<scalar, T_element>*> assignments(node.getAssignments());
    typedef typename std::vector<const PExprNode<scalar, T_element>*>::const_iterator ConstIterator;
    
    for(ConstIterator assignment = assignments.begin(); assignment!=assignments.end(); ++assignment)
    {
      const typename std::map<const PExpressionNode<T_element>*, int>::const_iterator numbering = nodeNumberings.find(*assignment);
      assert(numbering != nodeNumberings.end());
      boost::hash_combine(seed, numbering->second);
    }
    return seed;
  }

  template<typename exprType>
  std::size_t hashPairwise(const PPairwise<exprType, T_element>& node) const
  {
    std::size_t seed = hashBinOp(node);
    boost::hash_combine(seed, node.getOperation());
    return seed;
  }

  template<typename exprType>
  std::size_t hashScalarPiecewise(const PScalarPiecewise<exprType, T_element>& node) const
  {
    std::size_t seed = hashBinOp(node);
    boost::hash_combine(seed, node.getOperation());
    return seed;
  }
  
public:

  PHashingVisitor(const std::map<const PExpressionNode<T_element>*, int>& numberings) : nodeNumberings(numberings), hash(0)
  {
  }
  
  inline std::size_t getHash() const
  {
    return hash;
  }
  
  virtual void visit(PElementGet<vector, T_element>& e)
  {
    boost::hash_combine(hash, hashUnOp(e));
  }
  
  virtual void visit(PElementGet<matrix, T_element>& e)
  {
    boost::hash_combine(hash, hashUnOp(e));
  }

  virtual void visit(PElementSet<vector, T_element>& e)
  {
    boost::hash_combine(hash, hashElementSet(e));
  }
  
  virtual void visit(PElementSet<matrix, T_element>& e)
  {
    boost::hash_combine(hash, hashElementSet(e));
  }

  virtual void visit(PLiteral<scalar, T_element>& e)
  {
    boost::hash_combine(hash, hashExprNode(e));
  }
  
  virtual void visit(PLiteral<vector, T_element>& e)
  {
    boost::hash_combine(hash, hashExprNode(e));
  }
  
  virtual void visit(PLiteral<matrix, T_element>& e)
  {
    boost::hash_combine(hash, hashExprNode(e));
  }

  virtual void visit(PMatrixMult<T_element>& e)
  {
    boost::hash_combine(hash, hashBinOp(e));
  }
  
  virtual void visit(PMatrixVectorMult<T_element>& e)
  {
    boost::hash_combine(hash, hashBinOp(e));
  }
  
  virtual void visit(PTransposeMatrixVectorMult<T_element>& e)
  {
    boost::hash_combine(hash, hashBinOp(e));
  }
  
  virtual void visit(PVectorDot<T_element>& e)
  {
    boost::hash_combine(hash, hashBinOp(e));
  }
  
  virtual void visit(PVectorCross<T_element>& e)
  {
    boost::hash_combine(hash, hashBinOp(e));
  }
  
  virtual void visit(PVectorTwoNorm<T_element>& e)
  {
    boost::hash_combine(hash, hashUnOp(e));
  }
  
  virtual void visit(PMatrixTranspose<T_element>& e)
  {
    boost::hash_combine(hash, hashUnOp(e));
  }

  virtual void visit(PPairwise<scalar, T_element>& e)
  {
    boost::hash_combine(hash, hashPairwise(e));
  }
  
  virtual void visit(PPairwise<vector, T_element>& e)
  {
    boost::hash_combine(hash, hashPairwise(e));
  }
  
  virtual void visit(PPairwise<matrix, T_element>& e) 
  {
    boost::hash_combine(hash, hashPairwise(e));
  }

  virtual void visit(PScalarPiecewise<scalar, T_element>& e)
  {
    boost::hash_combine(hash, hashScalarPiecewise(e));
  }
  
  virtual void visit(PScalarPiecewise<vector, T_element>& e) 
  {
    boost::hash_combine(hash, hashScalarPiecewise(e));
  }
  
  virtual void visit(PScalarPiecewise<matrix, T_element>& e)
  {
    boost::hash_combine(hash, hashScalarPiecewise(e));
  }

  virtual void visit(PNegate<scalar, T_element>& e)
  {
    boost::hash_combine(hash, hashUnOp(e));
  }
  
  virtual void visit(PNegate<vector, T_element>& e)
  {
    boost::hash_combine(hash, hashUnOp(e));
  }
  
  virtual void visit(PNegate<matrix, T_element>& e)
  {
    boost::hash_combine(hash, hashUnOp(e));
  }

  virtual void visit(PAbsolute<T_element>& e)
  {
    boost::hash_combine(hash, hashUnOp(e));
  }
  
  virtual void visit(PSquareRoot<T_element>& e)
  {
    boost::hash_combine(hash, hashUnOp(e));
  }
};

}

}

#endif
