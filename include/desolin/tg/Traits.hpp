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

#ifndef DESOLIN_TG_TRAITS_HPP
#define DESOLIN_TG_TRAITS_HPP

#include <desolin/tg/Desolin_tg_fwd.hpp>

namespace desolin
{

namespace detail
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

}
#endif
