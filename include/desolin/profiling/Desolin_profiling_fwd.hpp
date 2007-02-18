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

#ifndef DESOLIN_PROFILING_FWD_HPP
#define DESOLIN_PROFILING_FWD_HPP

namespace desolin
{

namespace detail
{

template<typename T_element> class PExpressionNode;
template<ExprType exprType, typename T_element> class PExprNode;
template<ExprType resultType, ExprType exprType, typename T_element> class PUnOp;
template<ExprType exprType, typename T_element> class PNegate;
template<typename T_element> class PAbsolute;
template<typename T_element> class PSquareRoot;
template<ExprType resultType, ExprType leftType, ExprType rightType, typename T_element> class PBinOp;
template<ExprType exprType, typename T_element> class PElementGet;
template<ExprType exprType, typename T_element> class PElementSet;
template<typename T_element> class PMatrixMult;
template<typename T_element> class PMatrixVectorMult;
template<typename T_element> class PTransposeMatrixVectorMult;
template<typename T_element> class PVectorDot;
template<typename T_element> class PVectorCross;
template<typename T_element> class PVectorNorm;
template<typename T_element> class PMatrixTranspose;
template<ExprType exprType, typename T_element> class PPairwise;
template<ExprType exprType, typename T_element> class PScalarPiecewise;
template<ExprType exprType, typename T_element> class PLiteral;

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
