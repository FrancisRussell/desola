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

#ifndef DESOLIN_TRAITS_HPP
#define DESOLIN_TRAITS_HPP

#include <desolin/Desolin_fwd.hpp>

namespace desolin_internal
{

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

}
#endif
