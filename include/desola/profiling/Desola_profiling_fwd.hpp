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

#ifndef DESOLA_PROFILING_FWD_HPP
#define DESOLA_PROFILING_FWD_HPP

namespace desola
{

namespace detail
{

template<typename T_element> class PExpressionNode;
template<typename exprType, typename T_element> class PExprNode;
template<typename resultType, typename exprType, typename T_element> class PUnOp;
template<typename exprType, typename T_element> class PNegate;
template<typename T_element> class PAbsolute;
template<typename T_element> class PSquareRoot;
template<typename resultType, typename leftType, typename rightType, typename T_element> class PBinOp;
template<typename exprType, typename T_element> class PElementGet;
template<typename exprType, typename T_element> class PElementSet;
template<typename T_element> class PMatrixMult;
template<typename T_element> class PMatrixVectorMult;
template<typename T_element> class PTransposeMatrixVectorMult;
template<typename T_element> class PVectorDot;
template<typename T_element> class PVectorCross;
template<typename T_element> class PVectorTwoNorm;
template<typename T_element> class PMatrixTranspose;
template<typename exprType, typename T_element> class PPairwise;
template<typename exprType, typename T_element> class PScalarPiecewise;
template<typename exprType, typename T_element> class PLiteral;

template<typename T_element> class PExpressionNodeVisitor;
template<typename T_element> class PExpressionGraph;
template<typename T_element> class PExpressionNodeGenerator;
template<typename T_element> class PHashingVisitor;
template<typename T_element> class PEqualityCheckingVisitor;
template<typename T_element> class Profiler;
template<typename T_element> class PExpressionNodeRef;

}

}
#endif
