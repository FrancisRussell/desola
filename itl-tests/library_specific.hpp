#ifndef DESOLIN_DESOLIN_LIBRARY_SPECIFIC_HPP
#define DESOLIN_DESOLIN_LIBRARY_SPECIFIC_HPP

#include <cassert>
#include <desolin/Desolin.hpp>
#include <desolin/itl_interface.hpp>
#include <sys/time.h>
#include <sys/resource.h>
#include "solver_options.hpp"
#include "statistics_generator.hpp"

template<typename MatrixType, typename VectorType, typename ScalarType>
void solver(const SolverOptions& options, MatrixType& A, VectorType& x, VectorType& b);

template<typename Matrix>
inline std::size_t num_rows(const Matrix& m)
{
  return m.numRows();
}

template<typename Matrix>
inline std::size_t num_cols(const Matrix& m)
{
  return m.numCols();
}

template<typename Matrix>
inline std::size_t nnz(const Matrix& m)
{
  return m.nnz();
}

void library_init(const SolverOptions& options)
{
  // Increase maximum stack size because of size of vectors in runtime generated
  // code

  if (options.useSparse())
  {
    int result;
    rlimit limits;

    result = getrlimit(RLIMIT_STACK, &limits);
    assert(result == 0);

    limits.rlim_cur = limits.rlim_max;

    result = setrlimit(RLIMIT_STACK, &limits);
    assert(result == 0);
  }
}

namespace 
{

template<typename StreamType>
void invokeSolver(const SolverOptions& options, StreamType& stream)
{
  typedef double Type;
  typedef desolin::Matrix<Type> Matrix;
  typedef desolin::Vector<Type> Vector;
  typedef desolin::Scalar<Type> Scalar;
  
  Matrix A;

  if (options.useSparse())
    A = Matrix::loadSparse(stream);
  else
    A = Matrix::loadDense(stream);

  Vector x(num_cols(A), Type(0));
  Vector b(num_rows(A), Type(1));

  solver<Matrix, Vector, Scalar>(options, A, x, b);
}

}

void invokeSolver(const SolverOptions& options)
{
  if (options.fileIsHB())
  {
    desolin::harwell_boeing_stream<double> stream(const_cast<char*>(options.getFile().c_str()));
    invokeSolver(options, stream);
  }
  else if (options.fileIsMM())
  {
    desolin::matrix_market_stream<double> stream(const_cast<char*>(options.getFile().c_str()));
    invokeSolver(options, stream);
  }
  else
  {
    assert(false && "Unknown file type");
  }
}

#endif
