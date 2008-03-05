#ifndef DESOLIN_BLAS_LIBARY_SPECIFIC_HPP
#define DESOLIN_BLAS_LIBARY_SPECIFIC_HPP

#include "solver_options.hpp"
#include "statistics_generator.hpp"
#include "mtl/harwell_boeing_stream.h"
#include "mkl_cblas.h"
#include "blas_wrappers.hpp"
#include "blas_itl_interface.hpp"

typedef double Type;
typedef desolin_blas_wrappers::BLASGeneralMatrix<Type> Matrix;
typedef desolin_blas_wrappers::BLASVector<Type> Vector;
typedef Type Scalar;

inline std::size_t num_rows(const Matrix& m)
{
  return m.nrows();
}

inline std::size_t num_cols(const Matrix& m)
{
  return m.ncols();
}

using mtl::harwell_boeing_stream;

#endif