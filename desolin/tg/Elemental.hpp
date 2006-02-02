#ifndef DESOLIN_TG_ELEMENTAL_HPP
#define DESOLIN_TG_ELEMENTAL_HPP

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

  void accept(TGExpressionNodeVisitor<T_element>& v)
  {
    v.visit(*this);
  }
};

template<TGExprType exprType, typename T_element>
class TGElementSet : public TGBinOp<exprType, exprType, tg_scalar, T_element>
{
private:
  const TGElementIndex<exprType> index;

public:
  TGElementSet(typename TGInternalType<exprType, T_element>::type* internal,
		  TGExprNode<exprType, T_element>& o,
		  TGExprNode<tg_scalar, T_element>& s,
		  const TGElementIndex<exprType> i) : TGBinOp<exprType, exprType, tg_scalar, T_element>(internal, o, s), index(i)
  {
  }

  inline const TGElementIndex<exprType>& getIndex() const
  {
    return index;
  }

  void accept(TGExpressionNodeVisitor<T_element>& v)
  {
    v.visit(*this);
  }
};

}
#endif
