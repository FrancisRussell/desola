#ifndef DESOLIN_TRAITS_HPP
#define DESOLIN_TRAITS_HPP

#include <desolin/Desolin_fwd.hpp>

template<ExprType exprType, typename T_element>
struct ExprTraits
{
};

template<typename T_element>
struct ExprTraits<scalar, T_element>
{
  typedef InternalScalar<T_element> internalRep;
};

template<typename T_element>
struct ExprTraits<vector, T_element>
{
  typedef InternalVector<T_element> internalRep;
};

template<typename T_element>
struct ExprTraits<matrix, T_element>
{
  typedef InternalMatrix<T_element> internalRep;
};

#endif
