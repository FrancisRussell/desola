#ifndef DESOLIN_TG_CODE_GENERATOR_HPP
#define DESOLIN_TG_CODE_GENERATOR_HPP

#include <string>
#include <boost/shared_ptr.hpp>
#include <TaskGraph>
#include <desolin/tg/Desolin_tg_fwd.hpp>

//NOTE: The storage for the results is uninitialized, so make sure that
//      setExpression is used before calling addExpression on any result
//      element.

template<typename T_element>
class TGCodeGenerator : public TGExpressionNodeVisitor<T_element>
{
private:
  NameGenerator& generator;
	
  inline static const std::string getPrefix()
  {
    return std::string("index");
  }

  inline const char* getIndexName()
  {
    return generator.getName(getPrefix()).c_str();
  }
	
public:
  TGCodeGenerator(TGExpressionGraph<T_element>& g) : generator(g.getNameGenerator())
  {
  }
  
  virtual void visit(TGElementSet<tg_vector, T_element>& e)
  {
    using namespace tg;

    boost::shared_ptr< TGVector<T_element> > newVector(e.getInternal());
    boost::shared_ptr< TGVector<T_element> > vector(e.getLeft().getInternal());
    boost::shared_ptr< TGScalar<T_element> > scalar(e.getRight().getInternal());

    tVarNamed(int, i, getIndexName());
    tFor(i, 0, vector->getRows()-1)
    {
      newVector->setExpression(i, vector->getExpression(i));
    }
    newVector->setExpression(TaskExpression(e.getIndex().getRow()), scalar->getExpression());
  }

  virtual void visit(TGElementSet<tg_matrix, T_element>& e)
  {
    using namespace tg;

    boost::shared_ptr< TGMatrix<T_element> > newMatrix(e.getInternal());
    boost::shared_ptr< TGMatrix<T_element> > matrix(e.getLeft().getInternal());
    boost::shared_ptr< TGScalar<T_element> > scalar(e.getRight().getInternal());

    tVarNamed(int, i, getIndexName());
    tVarNamed(int, j, getIndexName());
    tFor(i, 0, matrix->getRows()-1)
    {
      tFor(j, 0, matrix->getCols()-1)
      {
        newMatrix->setExpression(i, j, matrix->getExpression(i, j));
      }
    }
    newMatrix->setExpression(TaskExpression(e.getIndex().getRow()), TaskExpression(e.getIndex().getCol()), scalar->getExpression());
  }

  virtual void visit(TGElementGet<tg_vector, T_element>& e)
  {
    using namespace tg;

    boost::shared_ptr< TGScalar<T_element> > result(e.getInternal());
    boost::shared_ptr< TGVector<T_element> > vector(e.getOperand().getInternal());

    result->setExpression(vector->getExpression(TaskExpression(e.getIndex().getRow())));
  }
 
  virtual void visit(TGElementGet<tg_matrix, T_element>& e)
  {
    using namespace tg;

    boost::shared_ptr< TGScalar<T_element> > result(e.getInternal());
    boost::shared_ptr< TGMatrix<T_element> > matrix(e.getOperand().getInternal());

    result->setExpression(matrix->getExpression(TaskExpression(e.getIndex().getRow()), TaskExpression(e.getIndex().getCol())));
  }

  virtual void visit(TGLiteral<tg_scalar, T_element>& e)
  {
  }
  virtual void visit(TGLiteral<tg_vector, T_element>& e)
  {
  }
 
  virtual void visit(TGLiteral<tg_matrix, T_element>& e)
  {
  }


  //NOTE: These loops have been structured to try to obtain good cache usage assuming
  //      row major storage.  If IR optimisation works, a more naive implementation
  //      of matrix multiply can be used instead and it will be automatically
  //      optimised for best cache locality.

  virtual void visit(TGMatrixMult<T_element>& e)
  {
    using namespace tg;
	 
    boost::shared_ptr< TGMatrix<T_element> > result(e.getInternal());
    boost::shared_ptr< TGMatrix<T_element> > left(e.getLeft().getInternal());
    boost::shared_ptr< TGMatrix<T_element> > right(e.getRight().getInternal());

    tVarNamed(int, i, getIndexName());
    tVarNamed(int, j, getIndexName());
    tVarNamed(int, k, getIndexName());

    tFor(i, 0, result->getRows()-1)
    {
      tFor(j, 0, result->getCols()-1)
      {
        result->setExpression(i, j, TGScalarExpr<T_element>());
      }
    }
    
    tFor(i, 0, result->getRows()-1)
    {
      tFor(k, 0, right->getRows()-1)
      {
        tFor(j, 0, result->getCols()-1)
        {
         result->addExpression(i, j, left->getExpression(i, k).mul(right->getExpression(k, j)));
        }
      }
    }
  }
 
  virtual void visit(TGMatrixVectorMult<T_element>& e)
  {
    using namespace tg;

    boost::shared_ptr< TGVector<T_element> > result(e.getInternal());
    boost::shared_ptr< TGMatrix<T_element> > matrix(e.getLeft().getInternal());
    boost::shared_ptr< TGVector<T_element> > vector(e.getRight().getInternal());

    tVarNamed(int, i, getIndexName());
    tVarNamed(int, j, getIndexName());
    
    tFor(i, 0, matrix->getRows()-1)
    {
      result->setExpression(i, TGScalarExpr<T_element>());
      tFor(j, 0, matrix->getCols()-1)
      {
        result->addExpression(i, matrix->getExpression(i, j).mul(vector->getExpression(j)));
      }
    }
  }
 
  virtual void visit(TGVectorDot<T_element>& e)
  {
    using namespace tg;
	 
    boost::shared_ptr< TGScalar<T_element> > result(e.getInternal());
    boost::shared_ptr< TGVector<T_element> > left(e.getLeft().getInternal());
    boost::shared_ptr< TGVector<T_element> > right(e.getRight().getInternal());

    tVarNamed(int, i, getIndexName());

    result->setExpression(TGScalarExpr<T_element>());
    tFor(i, 0, left->getRows()-1)
    {
      result->addExpression(left->getExpression(i).mul(right->getExpression(i)));
    }
  }
 
  virtual void visit(TGVectorCross<T_element>& e)
  {
    using namespace tg;

    boost::shared_ptr< TGVector<T_element> > result(e.getInternal());
    boost::shared_ptr< TGVector<T_element> > left(e.getLeft().getInternal());
    boost::shared_ptr< TGVector<T_element> > right(e.getRight().getInternal());

    result->setExpression(TaskExpression(0), left->getExpression(1).mul(right->getExpression(2)).sub(right->getExpression(1).mul(left->getExpression(2))));
    result->setExpression(TaskExpression(1), left->getExpression(2).mul(right->getExpression(0)).sub(right->getExpression(2).mul(left->getExpression(0))));
    result->setExpression(TaskExpression(2), left->getExpression(0).mul(right->getExpression(1)).sub(right->getExpression(0).mul(left->getExpression(1))));
  }
 
  virtual void visit(TGVectorNorm<T_element>& e)
  {
    using namespace tg;

    boost::shared_ptr< TGScalar<T_element> > result(e.getInternal());
    boost::shared_ptr< TGVector<T_element> > vector(e.getOperand().getInternal());
   
    tVarNamed(int, i, getIndexName());

    result->setExpression(TGScalarExpr<T_element>());
    tFor(i, 0, vector->getRows()-1)
    {
      result->addExpression(vector->getExpression(i).mul(vector->getExpression(i)));
    }
    result->setExpression(result->getExpression().sqrt());
  }
 
  virtual void visit(TGMatrixTranspose<T_element>& e)
  {
    using namespace tg;

    boost::shared_ptr< TGMatrix<T_element> > result(e.getInternal());
    boost::shared_ptr< TGMatrix<T_element> > matrix(e.getOperand().getInternal());

    tVarNamed(int, i, getIndexName());
    tVarNamed(int, j, getIndexName());

    tFor(i, 0, matrix->getRows()-1)
    {
      tFor(j, 0, matrix->getCols()-1)
      {
        result->setExpression(j, i, matrix->getExpression(i, j));
      }
    }
  }

  virtual void visit(TGPairwise<tg_scalar, T_element>& e)
  {
    using namespace tg;

    boost::shared_ptr< TGScalar<T_element> > result(e.getInternal());
    boost::shared_ptr< TGScalar<T_element> > left(e.getLeft().getInternal());
    boost::shared_ptr< TGScalar<T_element> > right(e.getRight().getInternal());

    result->setExpression(performOp(e.getOperation(), left->getExpression(), right->getExpression()));
  }
 
  virtual void visit(TGPairwise<tg_vector, T_element>& e)
  {
     using namespace tg;

     boost::shared_ptr< TGVector<T_element> > result(e.getInternal());
     boost::shared_ptr< TGVector<T_element> > left(e.getLeft().getInternal());
     boost::shared_ptr< TGVector<T_element> > right(e.getRight().getInternal());

     tVarNamed(int, i, getIndexName());
     tFor(i, 0, result->getRows()-1)
     {
       result->setExpression(i, performOp(e.getOperation(), left->getExpression(i), right->getExpression(i)));
     }
  }
 
  virtual void visit(TGPairwise<tg_matrix, T_element>& e)
  {
    using namespace tg;

    boost::shared_ptr< TGMatrix<T_element> > result(e.getInternal());
    boost::shared_ptr< TGMatrix<T_element> > left(e.getLeft().getInternal());
    boost::shared_ptr< TGMatrix<T_element> > right(e.getRight().getInternal());
   
      
    tVarNamed(int, i, getIndexName());
    tVarNamed(int, j, getIndexName());
    tFor(i, 0, result->getRows()-1)
    {
      tFor(j, 0, result->getCols()-1)
      {
        result->setExpression(i, j, performOp(e.getOperation(), left->getExpression(i, j), right->getExpression(i, j)));
      }
    }
  }

  virtual void visit(TGScalarPiecewise<tg_scalar, T_element>& e)
  {
    using namespace tg;

    boost::shared_ptr< TGScalar<T_element> > result(e.getInternal());
    boost::shared_ptr< TGScalar<T_element> > left(e.getLeft().getInternal());
    boost::shared_ptr< TGScalar<T_element> > right(e.getRight().getInternal());

    result->setExpression(performOp(e.getOperation(), left->getExpression(), right->getExpression()));	       
  }
 
  virtual void visit(TGScalarPiecewise<tg_vector, T_element>& e)
  {
     using namespace tg;

     boost::shared_ptr< TGVector<T_element> > result(e.getInternal());
     boost::shared_ptr< TGVector<T_element> > left(e.getLeft().getInternal());
     boost::shared_ptr< TGScalar<T_element> > right(e.getRight().getInternal());

     tVarNamed(int, i, getIndexName());
     tFor(i, 0, result->getRows()-1)
     {
       result->setExpression(i, performOp(e.getOperation(), left->getExpression(i), right->getExpression()));
     }			
  }
 
  virtual void visit(TGScalarPiecewise<tg_matrix, T_element>& e)
  {
    using namespace tg;

    boost::shared_ptr< TGMatrix<T_element> > result(e.getInternal());
    boost::shared_ptr< TGMatrix<T_element> > left(e.getLeft().getInternal());
    boost::shared_ptr< TGScalar<T_element> > right(e.getRight().getInternal());

    tVarNamed(int, i, getIndexName());
    tVarNamed(int, j, getIndexName());
    tFor(i, 0, result->getRows()-1)
    {
      tFor(j, 0, result->getCols()-1)
      {
        result->setExpression(i, j, performOp(e.getOperation(), left->getExpression(i, j), right->getExpression()));
      }
    }		    
  }

private:
  static inline TGScalarExpr<T_element> performOp(TGPairwiseOp op, const TGScalarExpr<T_element>& left, const TGScalarExpr<T_element>& right)
  {
    switch(op)
    {
      case tg_add: return left.add(right);
      case tg_sub: return left.sub(right);
      default: throw TGInvalidOperationError("Unrecognised TaskGraph Evaluator Pairwise Operation"); 
    }
  }

  static inline TGScalarExpr<T_element> performOp(TGScalarPiecewiseOp op, const TGScalarExpr<T_element>& left, const TGScalarExpr<T_element>& right)
  {
    switch(op)
    {
      case tg_multiply: return left.mul(right);
      case tg_divide: return left.div(right);
      case tg_assign: return right;
      default: throw TGInvalidOperationError("Unrecognised TaskGraph Evaluator ScalarPiecewise Operation");
    }
  }
};

#endif
