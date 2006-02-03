#ifndef DESOLIN_TG_LITERAL_HPP
#define DESOLIN_TG_LITERAL_HPP

#include <boost/shared_ptr.hpp>
#include <desolin/tg/Desolin_tg_fwd.hpp>

namespace desolin_internal
{

template<TGExprType exprType, typename T_element>
class TGLiteral : public TGExprNode<exprType, T_element>
{
public:
  TGLiteral(typename TGInternalType<exprType, T_element>::type* internal) : 
	  TGExprNode<exprType, T_element>(internal)
  {
  }

  void accept(TGExpressionNodeVisitor<T_element>& v)
  {
    v.visit(*this);
  }

  virtual std::set<TGExpressionNode<T_element>*> getDependencies() const
  {
    return std::set<TGExpressionNode<T_element>*>();
  }
};

}
#endif
