#ifndef DESOLIN_PROFILING_FWD_HPP
#define DESOLIN_PROFILING_FWD_HPP

#include <desolin/Desolin_fwd.hpp>

namespace desolin_internal
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

}

#endif
