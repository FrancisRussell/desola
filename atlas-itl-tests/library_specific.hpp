#ifndef DESOLA_ATLAS_LIBARY_SPECIFIC_HPP
#define DESOLA_ATLAS_LIBARY_SPECIFIC_HPP

#include <cassert>
#include "solver_options.hpp"
#include <desola/file-access/mtl_harwell_boeing_stream.hpp>
#include <desola/file-access/mtl_matrix_market_stream.hpp>

extern "C"
{
#include "cblas.h"
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

namespace
{

template<typename StreamType>
void invokeSolver(const SolverOptions& options, StreamType& stream)
{
  typedef double Type;
  typedef desola::blas_wrappers::BLASGeneralMatrix<Type> Matrix;
  typedef desola::blas_wrappers::BLASVector<Type> Vector;
  typedef Type Scalar;

  Matrix A(stream);
  Vector x(num_cols(A), Type(0));
  Vector b(num_rows(A), Type(1));

  solver<Matrix, Vector, Scalar>(options, A, x, b);
}

}

void invokeSolver(const SolverOptions& options)
{
  if (options.fileIsHB())
  {
    desola::harwell_boeing_stream<double> stream(const_cast<char*>(options.getFile().c_str()));
    invokeSolver(options, stream);
  }
  else if (options.fileIsMM())
  {
    desola::matrix_market_stream<double> stream(const_cast<char*>(options.getFile().c_str()));
    invokeSolver(options, stream);
  }
  else
  {
    assert(false && "Unknown file type");
  }
}


void library_init(SolverOptions& options)
{
}

#endif
