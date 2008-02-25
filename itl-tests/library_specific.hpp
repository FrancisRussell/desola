#ifndef DESOLIN_DESOLIN_LIBRARY_SPECIFIC_HPP
#define DESOLIN_DESOLIN_LIBRARY_SPECIFIC_HPP

#include <desolin/Desolin.hpp>
#include <desolin/itl_interface.hpp>
#include "solver_options.hpp"
#include "statistics_generator.hpp"

typedef double Type;
typedef desolin::Matrix<Type> Matrix;
typedef desolin::Vector<Type> Vector;
typedef desolin::Scalar<Type> Scalar;

using desolin::harwell_boeing_stream;

inline std::size_t num_rows(const Matrix& m)
{
  return m.numRows();
}


inline std::size_t num_cols(const Matrix& m)
{
  return m.numCols();
}

#endif
