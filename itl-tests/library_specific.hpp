#ifndef DESOLIN_DESOLIN_LIBRARY_SPECIFIC_HPP
#define DESOLIN_DESOLIN_LIBRARY_SPECIFIC_HPP

#include <desolin/Desolin.hpp>
#include <desolin/itl_interface.hpp>
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

void library_init()
{
}

void invokeSolver(const SolverOptions& options)
{
  typedef double Type;
  typedef desolin::Matrix<Type> Matrix;
  typedef desolin::Vector<Type> Vector;
  typedef desolin::Scalar<Type> Scalar;
  
  desolin::harwell_boeing_stream<Type> hbs(const_cast<char*>(options.getFile().c_str()));
  Matrix A;

  if (options.useSparse())
    A = Matrix::loadSparse(hbs);
  else
    A = Matrix::loadDense(hbs);

  Vector x(num_rows(A), Type(0));
  Vector b(num_cols(A), Type(1));

  solver<Matrix, Vector, Scalar>(options, A, x, b);
}

#endif
