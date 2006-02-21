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
  inline bool isEqual(const TGLiteral& node, const std::map<TGExpressionNode<T_element>*, TGExpressionNode<T_element>*>& mappings) const
  {
    return TGExprNode<exprType, T_element>::isEqual(node, mappings);
  }
  
  TGLiteral(typename TGInternalType<exprType, T_element>::type* internal) : 
	  TGExprNode<exprType, T_element>(internal)
  {
  }

  void accept(TGExpressionNodeVisitor<T_element>& v)
  {
    v.visit(*this);
  }

  virtual bool matches(const TGExpressionNode<T_element>& node, const std::map<TGExpressionNode<T_element>*, TGExpressionNode<T_element>*>& mappings) const
  {
    return TGExpressionNode<T_element>::matches(*this, node, mappings);
  }  
};

}
#endif
