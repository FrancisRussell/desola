#ifndef DESOLA_DESOLA_LIBRARY_SPECIFIC_HPP
#define DESOLA_DESOLA_LIBRARY_SPECIFIC_HPP

#include <cassert>
#include <desola/Desola.hpp>
#include <desola/itl_interface.hpp>
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
  const desola::Maybe<std::size_t> n(m.nnz());

  assert(n.hasValue());
  return n.value();
}

void library_init(const SolverOptions& options)
{
}

namespace 
{

template<typename StreamType>
void invokeSolver(const SolverOptions& options, StreamType& stream)
{
  typedef double Type;
  typedef desola::Matrix<Type> Matrix;
  typedef desola::Vector<Type> Vector;
  typedef desola::Scalar<Type> Scalar;
  
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

#endif
