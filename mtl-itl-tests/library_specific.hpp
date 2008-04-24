#ifndef DESOLIN_MTL_LIBRARY_SPECIFIC_HPP
#define DESOLIN_MTL_LIBRARY_SPECIFIC_HPP

#include "solver_options.hpp"
#include "statistics_generator.hpp"
#include <mtl/mtl.h>
#include <mtl/matrix.h>
#include <mtl/harwell_boeing_stream.h>
#include <mtl/matrix_market_stream.h>
#include <itl/interface/mtl.h>

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

template<typename Matrix>
inline std::size_t nnz(const Matrix& m)
{
  return m.nnz();
}

namespace
{

template<typename StreamType>
void invokeSolver(const SolverOptions& options, StreamType& stream)
{
  typedef double Type;
  typedef mtl::matrix<Type, mtl::rectangle<>, mtl::array< mtl::dense<> >, mtl::row_major>::type MatrixDense;
  typedef mtl::matrix<Type, mtl::rectangle<>, mtl::array< mtl::compressed<> >, mtl::row_major>::type MatrixSparse;
  typedef mtl::dense1D<Type> Vector;
  typedef Type Scalar;

  if (options.useSparse())
  {
    MatrixSparse A(stream);
    Vector x(num_cols(A), Type(0));
    Vector b(num_rows(A), Type(1));

    solver<MatrixSparse, Vector, Scalar>(options, A, x, b);
  }
  else
  {
    MatrixDense A(stream);
    Vector x(num_cols(A), Type(0));
    Vector b(num_rows(A), Type(1));

    solver<MatrixDense, Vector, Scalar>(options, A, x, b);
  }
}

}

void invokeSolver(const SolverOptions& options)
{
  if (options.fileIsHB())
  {
    mtl::harwell_boeing_stream<double> stream(const_cast<char*>(options.getFile().c_str()));
    invokeSolver(options, stream);
  }
  else if (options.fileIsMM())
  {
    mtl::matrix_market_stream<double> stream(const_cast<char*>(options.getFile().c_str()));
    invokeSolver(options, stream);
  }
  else
  {
    assert(false && "Unknown file type");
  }
}

void library_init()
{
}

#endif
