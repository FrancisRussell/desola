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

#include "solver_options.hpp"
#include "statistics_generator.hpp"
#include <mtl/matrix.h>
#include <mtl/mtl.h>
#include <mtl/utils.h>
#include "mtl/harwell_boeing_stream.h"

#include <itl/interface/mtl.h>
#include <itl/krylov/richardson.h>

/*
  In this example, we show how to use bicgstab algorithm.
*/
using namespace mtl;
using namespace itl;

typedef  double Type;

//begin
typedef matrix< Type, rectangle<>, 
	        array< dense<> >, 
                row_major >::type Matrix;
//end

int main (int argc, char* argv[]) 
{
  using std::cout;
  using std::endl;

  SolverOptions solverOptions("Unsymmetric matrix in Harwell-Boeing format");
  solverOptions.processOptions(argc, argv);
      
  harwell_boeing_stream<Type> hbs(solverOptions.getFile().c_str());
  const int max_iter = solverOptions.getIterations();
	
  //begin
  Matrix A(hbs);

  dense1D<Type> x(A.nrows(), Type(0));
  dense1D<Type> b(A.ncols());
  for (dense1D<Type>::iterator i=b.begin(); i!=b.end(); i++)
    *i = 1.;

  identity_preconditioner precond;
  //iteration
  noisy_iteration<double> iter(b, max_iter, 1.0e-6);
  //richardson algorithm
  StatisticsGenerator stats;
  richardson(A, x, b, precond(), iter);
  //end

  //verify the result
  dense1D<Type> b1(A.ncols());
  itl::mult(A, x, b1);
  itl::add(b1, itl::scaled(b, -1.), b1);

  cout << "Residual " << itl::two_norm(b1) << endl;
  stats.printResults(solverOptions.getFile(), hbs, iter, !solverOptions.singleLineResult());
  return 0;
}


