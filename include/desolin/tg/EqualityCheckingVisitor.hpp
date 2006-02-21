#ifndef DESOLIN_TG_EQUALITY_CHECKING_VISITOR_HPP
#define DESOLIN_TG_EQUALITY_CHECKING_VISITOR_HPP

#include <cassert>
#include <map>
#include <typeinfo>
#include <desolin/tg/Desolin_tg_fwd.hpp>

namespace desolin_internal
{

template<typename T_element>
class TGEqualityCheckingVisitor : public TGExpressionNodeVisitor<T_element>
{
private:
  const std::map<const TGExpressionNode<T_element>*, const TGExpressionNode<T_element>*> mappings;
  bool equal;
  
  template<typename T_node>
  inline void checkMatch(const T_node& left)
  {
    assert(mappings.find(&left) != mappings.end());
    const TGExpressionNode<T_element>& right = *(mappings.find(&left)->second);

    if (typeid(left) == typeid(right))
    {
      const T_node& castedRight = static_cast<const T_node&>(right);
      equal = equal && left.isEqual(castedRight, mappings);
    }
    else
    {
      equal = false;
    }
  }
  
public:
  TGEqualityCheckingVisitor(const std::map<const TGExpressionNode<T_element>*, const TGExpressionNode<T_element>*>& m) : mappings(m), equal(true)
  {
  }

  bool isEqual() const
  {
    return equal;
  }
  
  virtual void visit(TGElementGet<tg_vector, T_element>& e)
  {
    checkMatch(e);
  }
  
  virtual void visit(TGElementGet<tg_matrix, T_element>& e)
  {
    checkMatch(e);
  }

  virtual void visit(TGElementSet<tg_vector, T_element>& e)
  {
    checkMatch(e);
  }
  
  virtual void visit(TGElementSet<tg_matrix, T_element>& e)
  {
    checkMatch(e);
  }

  virtual void visit(TGLiteral<tg_scalar, T_element>& e)
  {
    checkMatch(e);
  }
  
  virtual void visit(TGLiteral<tg_vector, T_element>& e)
  {
    checkMatch(e);
  }
  
  virtual void visit(TGLiteral<tg_matrix, T_element>& e)
  {
    checkMatch(e);
  }

  virtual void visit(TGMatrixMult<T_element>& e)
  {
    checkMatch(e);
  }
  
  virtual void visit(TGMatrixVectorMult<T_element>& e)
  {
    checkMatch(e);
  }
  
  virtual void visit(TGVectorDot<T_element>& e)
  {
    checkMatch(e);
  }
  
  virtual void visit(TGVectorCross<T_element>& e)
  {
    checkMatch(e);
  }
  
  virtual void visit(TGVectorNorm<T_element>& e)
  {
    checkMatch(e);
  }
  
  virtual void visit(TGMatrixTranspose<T_element>& e)
  {
    checkMatch(e);
  }

  virtual void visit(TGPairwise<tg_scalar, T_element>& e)
  {
    checkMatch(e);
  }
  
  virtual void visit(TGPairwise<tg_vector, T_element>& e)
  {
    checkMatch(e);
  }
  
  virtual void visit(TGPairwise<tg_matrix, T_element>& e)
  {
    checkMatch(e);
  }

  virtual void visit(TGScalarPiecewise<tg_scalar, T_element>& e)
  {
    checkMatch(e);
  }
  
  virtual void visit(TGScalarPiecewise<tg_vector, T_element>& e)
  {
    checkMatch(e);
  }
  
  virtual void visit(TGScalarPiecewise<tg_matrix, T_element>& e)
  {
    checkMatch(e);
  }

  virtual void visit(TGNegate<tg_scalar, T_element>& e)
  {
    checkMatch(e);
  }

  virtual void visit(TGNegate<tg_vector, T_element>& e)
  {
    checkMatch(e);
  }
  
  virtual void visit(TGNegate<tg_matrix, T_element>& e)
  {
    checkMatch(e);
  }

  virtual void visit(TGAbsolute<T_element>& e)
  {
    checkMatch(e);
  }
  
  virtual void visit(TGSquareRoot<T_element>& e)
  {
    checkMatch(e);
  }
};

}
#endif