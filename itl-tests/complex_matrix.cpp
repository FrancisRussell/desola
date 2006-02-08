//===========================================================================
//  CVS Information:                                                         
//                                                                           
//     $RCSfile: complex_matrix.cpp,v $  $Revision: 1.2 $  $State: Exp $ 
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
// $Log: complex_matrix.cpp,v $
// Revision 1.2  2001/10/18 14:08:33  llee
// re-organize the directory structures
//
// Revision 1.1  2001/06/02 05:43:01  llee1
// ITL is able to solve complex matrices now.
//
//===========================================================================
#include <complex>

#include "mtl/matrix.h"
#include "mtl/mtl.h"
#include "mtl/utils.h"
#include "mtl/matrix_market_stream.h"

#include <itl/interface/mtl.h>

#include "itl/preconditioner/ilu.h"
#include "itl/krylov/gmres.h"
#include "itl/krylov/qmr.h"

/*
  In thsi example, we show how to use GMRES(m) algorithm to solve complex 
  linear system

llee$ ./complex_matrix qc324.mtx

iteration 0: resid 28719.9
iteration 1: resid 198.482
iteration 2: resid 79.1008
iteration 3: resid 0.654307
iteration 4: resid 0.21795
iteration 5: resid 0.00177835
iteration 6: resid 0.000431149
iteration 7: resid 4.64984e-06
iteration 7: resid 4.64983e-06
finished! error code = 0
7 iterations
4.64983e-06 is actual final residual.
1.61903e-10 is actual relative tolerance achieved.
Relative tol: 1e-08  Absolute tol: 0
Ture Residual: 6.38198e-07

*/

using namespace mtl;
using namespace itl;

typedef  std::complex<double> Type;

//begin
typedef matrix< Type, 
                rectangle<>, 
	        array< compressed<> >, 
                row_major >::type Matrix;
//end

int main (int argc, char* argv[]) 
{
  using std::cout;
  using std::endl;

  if ( argc == 1 ) {
    cout << "Usage: " << argv[0] 
	 << " <Unsymmetric matrix in Matrix Market format> "
	 << endl;
    return 0;
  }

#if 1
  matrix_market_stream<Type> mms(argv[1]);
  Matrix A(mms);
#else
  Matrix A(5, 5);
  A(0, 0) = Type(5, 6);
  A(1, 1) = Type(1, 4);
  A(2, 2) = Type(2, 3);
  A(3, 3) = Type(3, 2);
  A(4, 4) = Type(4, 1);

  A(0, 3) = Type(1, 1);
  A(0, 4) = Type(1, 1);

  A(1, 2) = Type(1, 1);
  A(2, 4) = Type(1, 1);
  A(3, 1) = Type(1, 1);
  A(4, 2) = Type(1, 1);
#endif

  int max_iter = 1024;

  dense1D<Type> x(A.nrows(), Type(0));
  dense1D<Type> b(A.ncols());
  for (dense1D<Type>::iterator i=b.begin(); i!=b.end(); i++)
    *i = 1.;
  //begin

  ILU<Matrix> p(A);
  //identity_preconditioner p;

  //gmres needs teh preconditioned b to pass into iter object.
  dense1D<Type> b2(A.ncols());
  solve(p(), b, b2); 
  //iteration

  noisy_iteration<double> iter(b2, max_iter, 1e-8);
  //noisy_iteration<double> iter(b, max_iter, 1e-8);

  int restart = 20;
  typedef dense1D<Type> Vec;

  //classical_gram_schmidt<Vec> orth(restart, size(x));
  modified_gram_schmidt<Vec> orth(restart, size(x));

  //gmres algorithm
  gmres(A, x, b, p(), restart, iter, orth); 
  //qmr(A, x, b, p(), p(), iter);
  //end

  //verify the result
  dense1D<Type> b1(A.ncols());
  itl::mult(A, x, b1);
  itl::add(b1, itl::scaled(b, -1.), b1);

  cout << "Ture Residual: " << itl::two_norm(b1) << endl;

  return 0;
}


