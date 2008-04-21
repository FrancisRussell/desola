// Modified 2005-2006 by Francis Russell to work with desolin library and allow benchmarking.
// Copyright 1997, University of Notre Dame.
// Authors: Andrew Lumsdaine, Lie-Quan Lee
//
// This file is part of the Iterative Template Library
//
// You should have received a copy of the License Agreement for the
// Iterative Template Library along with the software;  see the
// file LICENSE.  If not, contact Office of Research, University of Notre
// Dame, Notre Dame, IN  46556.
//
// Permission to modify the code and to distribute modified code is
// granted, provided the text of this NOTICE is retained, a notice that
// the code was modified is included with the above COPYRIGHT NOTICE and
// with the COPYRIGHT NOTICE in the LICENSE file, and that the LICENSE
// file is distributed with the modified code.
//
// LICENSOR MAKES NO REPRESENTATIONS OR WARRANTIES, EXPRESS OR IMPLIED.
// By way of example, but not limitation, Licensor MAKES NO
// REPRESENTATIONS OR WARRANTIES OF MERCHANTABILITY OR FITNESS FOR ANY
// PARTICULAR PURPOSE OR THAT THE USE OF THE LICENSED SOFTWARE COMPONENTS
// OR DOCUMENTATION WILL NOT INFRINGE ANY PATENTS, COPYRIGHTS, TRADEMARKS
// OR OTHER RIGHTS.

#include <cstdlib>
#include <iostream>
#include "library_specific.hpp"
#include <itl/krylov/cheby.h>

using namespace itl;

template<typename MatrixType, typename VectorType, typename ScalarType>
void solver(SolverOptions& options, MatrixType& A, VectorType& x, VectorType& b)
{
  const int max_iter = options.getIterations();

  Type eigmin = 0.01;
  Type eigmax = 10.0;

  identity_preconditioner precond;
  noisy_iteration<ScalarType> iter(b, max_iter, 1e-9);
  StatisticsGenerator stats;
  cheby(A, x, b, precond(), iter, eigmin, eigmax);

  //verify the result
  Vector b1(num_cols(A));
  itl::mult(A, x, b1);
  itl::add(b1, itl::scaled(b, -1.), b1);

  std::cout << "Residual " << itl::two_norm(b1) << std::endl;
  stats.printResults(A, iter, options);
}

int main (int argc, char* argv[]) 
{
  library_init();

  SolverOptions options("Unsymmetric matrix in Harwell-Boeing format");
  options.processOptions(argc, argv);
        
  harwell_boeing_stream<Type> hbs(const_cast<char*>(options.getFile().c_str()));      
  
  Matrix A(hbs);
  Vector x(num_rows(A), Type(0));
  Vector b(num_cols(A), Type(1));

  solver<Matrix, Vector, Scalar>(options, A, x, b);

  return EXIT_SUCCESS;
}


