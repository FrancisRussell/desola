#ifndef DESOLIN_IMKL_LIBARY_SPECIFIC_HPP
#define DESOLIN_IMKL_LIBARY_SPECIFIC_HPP

#include "solver_options.hpp"
#include "statistics_generator.hpp"
#include "mtl/harwell_boeing_stream.h"

extern "C" 
{
#include "mkl.h"
#include "mkl_cblas.h"
}

#include "blas_wrappers.hpp"
#include "blas_itl_interface.hpp"

template<typename MatrixType, typename VectorType, typename ScalarType>
void solver(const SolverOptions& options, MatrixType& A, VectorType& x, VectorType& b);

template<typename Matrix>
inline std::size_t num_rows(const Matrix& m)
{
  return m.nrows();
}

template<typename Matrix>
inline std::size_t num_cols(const Matrix& m)
{
  return m.ncols();
}

void library_init()
{
  mkl_set_num_threads(1);
}

void invokeSolver(const SolverOptions& options)
{
  typedef double Type;
  typedef desolin::blas_wrappers::BLASGeneralMatrix<Type> Matrix;
  typedef desolin::blas_wrappers::BLASVector<Type> Vector;
  typedef Type Scalar;

  mtl::harwell_boeing_stream<Type> hbs(const_cast<char*>(options.getFile().c_str()));

  Matrix A(hbs);
  Vector x(num_rows(A), Type(0));
  Vector b(num_cols(A), Type(1));

  solver<Matrix, Vector, Scalar>(options, A, x, b);
}

#endif
