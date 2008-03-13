#ifndef DESOLIN_MTL_LIBRARY_SPECIFIC_HPP
#define DESOLIN_MTL_LIBRARY_SPECIFIC_HPP

#include "solver_options.hpp"
#include "statistics_generator.hpp"
#include <mtl/mtl.h>
#include <mtl/matrix.h>
#include <mtl/harwell_boeing_stream.h>
#include <itl/interface/mtl.h>

typedef double Type;
typedef mtl::matrix<Type, mtl::rectangle<>, mtl::array< mtl::dense<> >, mtl::row_major>::type Matrix;
typedef mtl::dense1D<Type> Vector;
typedef Type Scalar;

using mtl::harwell_boeing_stream;

inline std::size_t num_rows(const Matrix& m)
{
  return m.nrows();
}


inline std::size_t num_cols(const Matrix& m)
{
  return m.ncols();
}

void library_init()
{
}

#endif
