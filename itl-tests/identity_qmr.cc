//===========================================================================
//  CVS Information:                                                         
//                                                                           
//     $RCSfile: ilu_qmr.cpp,v $  $Revision: 1.3 $  $State: Exp $ 
//     $Author: llee $  $Date: 2001/10/18 14:08:33 $ 
//     $Locker:  $ 
//---------------------------------------------------------------------------
//                                                                           
// DESCRIPTION                                                               
//                                                                           
//---------------------------------------------------------------------------
//                                                                           
// LICENSE AGREEMENT                                                         
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
//---------------------------------------------------------------------------
//                                                                           
// REVISION HISTORY:                                                         
//                                                                           
// $Log: ilu_qmr.cpp,v $
// Revision 1.3  2001/10/18 14:08:33  llee
// re-organize the directory structures
//
// Revision 1.2  2000/07/27 04:39:21  llee1
// *** empty log message ***
//
// Revision 1.1  2000/07/26 21:50:09  llee1
// change file extension from .cc to .cpp
//
// Revision 1.1  2000/07/18 16:41:01  llee1
// add Harwell-Boeing matrix test
//
// Revision 1.2  2000/07/17 15:44:04  llee1
// *** empty log message ***
//
// Revision 1.1  1999/11/21 20:47:07  lums
// *** empty log message ***
//
//===========================================================================

#include "solver_options.hpp"
#include "statistics_generator.hpp"
#include <desolin/itl_interface.hpp>
#include <desolin/Desolin.hpp>
#include <itl/krylov/qmr.h>
#include <boost/timer.hpp>

/*
  In thsi example, we show how to use QMR algorithm
*/
using namespace itl;

typedef double Type;
typedef desolin::Scalar<Type> Scalar;
typedef desolin::Vector<Type> Vector;
typedef desolin::Matrix<Type> Matrix;

int main(int argc, char* argv[])
{
  using std::cout;
  using std::endl;

  SolverOptions solverOptions("Unsymmetric matrix in Harwell-Boeing format");
  solverOptions.processOptions(argc, argv);

  desolin::harwell_boeing_stream<Type> hbs(solverOptions.getFile().c_str());
  const int max_iter = solverOptions.getIterations();

  //begin
  Matrix A(hbs);
  //end

  //begin
  Vector x(A.numRows(), Type(0));
  Vector b(A.numCols(), Type(1.));

  //Identity preconditioner
  identity_preconditioner precond;
  //iteration
  noisy_iteration<Scalar> iter(b, max_iter, 1e-6);
  //qmr algorithm
  StatisticsGenerator stats;
  qmr(A, x, b, precond.left(), precond.right(), iter);
  //end

  //verify the result
  Vector b1(A.numCols());
  itl::mult(A, x, b1);
  itl::add(b1, itl::scaled(b, -1.), b1);
  
  cout << "Residual " << itl::two_norm(b1) << endl;
  stats.printResults(solverOptions.getFile(), hbs, iter, !solverOptions.singleLineResult());
  return 0;
}


