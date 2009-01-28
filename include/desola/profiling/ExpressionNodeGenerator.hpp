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

#ifndef DESOLA_PROFILING_EXPRESSION_NODE_GENERATOR_HPP
#define DESOLA_PROFILING_EXPRESSION_NODE_GENERATOR_HPP

#include "Desola_profiling_fwd.hpp"
#include <boost/ptr_container/ptr_vector.hpp>
#include <map>
#include <vector>
#include <cassert>
#include <utility>
#include <algorithm>

namespace desola
{

namespace detail
{

template<typename T_element>
class PExpressionNodeGenerator : public ExpressionNodeVisitor<T_element>
{
private:
  std::map<ExprNode<scalar, T_element>*, PExprNode<scalar, T_element>*> scalarMap;
  std::map<ExprNode<vector, T_element>*, PExprNode<vector, T_element>*> vectorMap;
  std::map<ExprNode<matrix, T_element>*, PExprNode<matrix, T_element>*> matrixMap;
      
  PExpressionGraph<T_element>& graph;
  
  void handleNode(ExprNode<scalar, T_element>& exprNode, PExprNode<scalar, T_element>* const node)
  {
    scalarMap[&exprNode] = node;
    graph.addNode(node);
  }

  void handleNode(ExprNode<vector, T_element>& exprNode, PExprNode<vector, T_element>* const node)
  {
    vectorMap[&exprNode] = node;
    graph.addNode(node);
  }

  void handleNode(ExprNode<matrix, T_element>& exprNode, PExprNode<matrix, T_element>* const node)
  {
    matrixMap[&exprNode] = node;
    graph.addNode(node);
  }

  PExprNode<scalar, T_element>& getScalar(ExprNode<scalar, T_element>& expr) const
  {
    assert(scalarMap.find(&expr) != scalarMap.end());
    return *(scalarMap.find(&expr)->second);
  }

  PExprNode<vector, T_element>& getVector(ExprNode<vector, T_element>& expr) const
  {
    assert(vectorMap.find(&expr) != vectorMap.end());
    return *(vectorMap.find(&expr)->second);	
  }

  PExprNode<matrix, T_element>& getMatrix(ExprNode<matrix, T_element>& expr) const
  {
    assert(matrixMap.find(&expr) != matrixMap.end());
    return *(matrixMap.find(&expr)->second);	
  }

  template<typename exprType>
  std::vector<PExprNode<scalar, T_element>*> getAssignments(ElementSet<exprType, T_element>& expr) const 
  {
    const std::map<ElementIndex<exprType>, ExprNode<scalar, T_element>*> exprAssignments(expr.getAssignments());
    std::vector<PExprNode<scalar, T_element>*> pAssignments;

    typedef typename std::map<ElementIndex<exprType>, ExprNode<scalar, T_element>*>::const_iterator ConstIterator;
    for(ConstIterator exprIterator  = exprAssignments.begin(); exprIterator != exprAssignments.end(); ++exprIterator)
    {
      pAssignments.push_back(&getScalar(*(exprIterator->second)));
    }
    return pAssignments;
  }
  
public:
  PExpressionNodeGenerator(PExpressionGraph<T_element>& g) : graph(g)
  {
  }
  
  virtual void visit(Pairwise<scalar, T_element>& e)
  {
    handleNode(e, new PPairwise<scalar, T_element>(e.getOperation(), getScalar(e.getLeft()), getScalar(e.getRight())));
  }
  
  virtual void visit(Pairwise<vector, T_element>& e)
  {
    handleNode(e, new PPairwise<vector, T_element>(e.getOperation(), getVector(e.getLeft()), getVector(e.getRight())));
  }
  
  virtual void visit(Pairwise<matrix, T_element>& e)
  {
    handleNode(e, new PPairwise<matrix, T_element>(e.getOperation(), getMatrix(e.getLeft()), getMatrix(e.getRight())));
  }

  virtual void visit(ScalarPiecewise<scalar, T_element>& e)
  {
    handleNode(e, new PScalarPiecewise<scalar, T_element>(e.getOperation(), getScalar(e.getLeft()), getScalar(e.getRight())));
  }
  
  virtual void visit(ScalarPiecewise<vector, T_element>& e)
  {
    handleNode(e, new PScalarPiecewise<vector, T_element>(e.getOperation(), getVector(e.getLeft()), getScalar(e.getRight())));
  }
  
  virtual void visit(ScalarPiecewise<matrix, T_element>& e)
  {
    handleNode(e, new PScalarPiecewise<matrix, T_element>(e.getOperation(), getMatrix(e.getLeft()), getScalar(e.getRight())));
  }

  virtual void visit(MatrixMult<T_element>& e)
  {
    handleNode(e, new PMatrixMult<T_element>(getMatrix(e.getLeft()), getMatrix(e.getRight())));
  }
  
  virtual void visit(MatrixVectorMult<T_element>& e)
  {
    handleNode(e, new PMatrixVectorMult<T_element>(getMatrix(e.getLeft()), getVector(e.getRight())));
  }
  
  virtual void visit(TransposeMatrixVectorMult<T_element>& e)
  {
    handleNode(e, new PTransposeMatrixVectorMult<T_element>(getMatrix(e.getLeft()), getVector(e.getRight())));
  }
  
  virtual void visit(VectorDot<T_element>& e)
  {
    handleNode(e, new PVectorDot<T_element>(getVector(e.getLeft()), getVector(e.getRight())));
  }
  
  virtual void visit(VectorCross<T_element>& e)
  {
    handleNode(e, new PVectorCross<T_element>(getVector(e.getLeft()), getVector(e.getRight())));
  }
  
  virtual void visit(VectorTwoNorm<T_element>& e)
  {
    handleNode(e, new PVectorTwoNorm<T_element>(getVector(e.getOperand())));
  }
  
  virtual void visit(MatrixTranspose<T_element>& e)
  {
    handleNode(e, new PMatrixTranspose<T_element>(getMatrix(e.getOperand())));
  }

  virtual void visit(ElementGet<vector, T_element>& e)
  {
    handleNode(e, new PElementGet<vector, T_element>(getVector(e.getOperand())));
  }
  
  virtual void visit(ElementGet<matrix, T_element>& e)
  {
    handleNode(e, new PElementGet<matrix, T_element>(getMatrix(e.getOperand())));
  }

  virtual void visit(ElementSet<vector, T_element>& e)
  {
    handleNode(e, new PElementSet<vector, T_element>(getVector(e.getOperand()), getAssignments(e)));
  }
  
  virtual void visit(ElementSet<matrix, T_element>& e)
  {
    handleNode(e, new PElementSet<matrix, T_element>(getMatrix(e.getOperand()), getAssignments(e)));
  }

  virtual void visit(Literal<scalar, T_element>& e)
  {
    handleNode(e, new PLiteral<scalar, T_element>());
  }
  
  virtual void visit(Literal<vector, T_element>& e)
  {
    handleNode(e, new PLiteral<vector, T_element>());
  }
  
  virtual void visit(Literal<matrix, T_element>& e)
  {
    handleNode(e, new PLiteral<matrix, T_element>());
  }

  virtual void visit(Negate<scalar, T_element>& e)
  {
    handleNode(e, new PNegate<scalar, T_element>(getScalar(e.getOperand())));
  }
  
  virtual void visit(Negate<vector, T_element>& e)
  {
    handleNode(e, new PNegate<vector, T_element>(getVector(e.getOperand())));
  }
  
  virtual void visit(Negate<matrix, T_element>& e)
  {
    handleNode(e, new PNegate<matrix, T_element>(getMatrix(e.getOperand())));
  }

  virtual void visit(Absolute<T_element>& e)
  {
    handleNode(e, new PAbsolute<T_element>(getScalar(e.getOperand())));
  }
  
  virtual void visit(SquareRoot<T_element>& e)
  {
    handleNode(e, new PSquareRoot<T_element>(getScalar(e.getOperand())));
  }
};

}

}

#endif
