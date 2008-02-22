// Modified 2005-2006 by Francis Russell to work with desolin library and allow benchmarking.
// Copyright 1997, University of Notre Dame.
// Authors: Andrew Lumsdaine, Lie-Quan Lee, and Lie-Quan Lee
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

#include "solver_options.hpp"
#include "statistics_generator.hpp"
#include "blas_wrappers.hpp"
#include "blas_itl_interface.hpp"
#include "mtl/harwell_boeing_stream.h"
#include "itl/krylov/cg.h"

using namespace desolin_blas_wrappers;
using namespace itl;

typedef  double Type;
//begin
typedef BLASGeneralMatrix<Type> Matrix;
typedef BLASVector<Type> Vector;


//end
int main (int argc, char* argv[]) 
{
  using std::cout;
  using std::endl;

  SolverOptions solverOptions("Symmetric Positive Definite matrix in Harwell-Boeing format");
  solverOptions.processOptions(argc, argv);

  const int max_iter = solverOptions.getIterations();
  mtl::harwell_boeing_stream<Type> hbs(const_cast<char*>(solverOptions.getFile().c_str()));	
  //begin
  Matrix A(hbs);
  //end

  //begin
  Vector x(A.nrows(), Type(0));
  Vector b(A.ncols(), Type(1));
  //Incomplete cholesky preconditioner
  identity_preconditioner precond;
  noisy_iteration<double> iter(b, max_iter, 1e-6);
  StatisticsGenerator stats;
  cg(A, x, b, precond(), iter);
  //end

  //verify the result
  Vector b1(A.ncols());
  itl::mult(A, x, b1);
  itl::add(b1, itl::scaled(b, -1.), b1);

  cout << "True Residual: " << itl::two_norm(b1) << endl;
  stats.printResults(solverOptions.getFile(), hbs, iter, !solverOptions.singleLineResult());
  return 0;
}


