#ifndef DESOLIN_TG_TRAITS_HPP
#define DESOLIN_TG_TRAITS_HPP

#include <desolin/tg/Desolin_tg_fwd.hpp>

namespace desolin_internal
{

template<ExprType exprType, typename T_element>
struct ExprTGTraits
{
};  
    
template<typename T_element>
struct ExprTGTraits<scalar, T_element>
{
  static const TGExprType tgExprType = tg_scalar;
  typedef TGScalarGen<T_element> internalRepCreator;
  typedef TGScalar<T_element> internalRep;
  typedef TGConventionalScalar<T_element> conventionalStorage;
};
    
template<typename T_element>
struct ExprTGTraits<vector, T_element>
{ 
  static const TGExprType tgExprType = tg_vector;
  typedef TGVectorGen<T_element> internalRepCreator;
  typedef TGVector<T_element> internalRep;
  typedef TGConventionalVector<T_element> conventionalStorage;
};
  
template<typename T_element>
struct ExprTGTraits<matrix, T_element>
{   
  static const TGExprType tgExprType = tg_matrix;
  typedef TGMatrixGen<T_element> internalRepCreator;
  typedef TGMatrix<T_element> internalRep;
  typedef TGConventionalMatrix<T_element> conventionalStorage;
};


template<TGExprType exprType, typename T_elementType>
class TGInternalType
{
};

template<typename T_elementType>
class TGInternalType<tg_scalar, T_elementType>
{
public:
  typedef TGScalar<T_elementType> type;
};

template<typename T_elementType>
class TGInternalType<tg_vector, T_elementType>
{
public:
  typedef TGVector<T_elementType> type;
};

template<typename T_elementType>
class TGInternalType<tg_matrix, T_elementType>
{
public:
  typedef TGMatrix<T_elementType> type;
};

}
#endif
