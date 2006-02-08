//===========================================================================
//  CVS Information:                                                         
//                                                                           
//     $RCSfile: ssor_bicgstab.cpp,v $  $Revision: 1.4 $  $State: Exp $ 
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
// $Log: ssor_bicgstab.cpp,v $
// Revision 1.4  2001/10/18 14:08:33  llee
// re-organize the directory structures
//
// Revision 1.3  2000/07/27 15:50:22  llee1
// for vc++
//
// Revision 1.2  2000/07/27 04:39:22  llee1
// *** empty log message ***
//
// Revision 1.1  2000/07/26 21:50:17  llee1
// change file extension from .cc to .cpp
//
// Revision 1.1  2000/07/18 16:41:04  llee1
// add Harwell-Boeing matrix test
//
// Revision 1.3  2000/07/18 14:30:44  llee1
// *** empty log message ***
//
// Revision 1.2  2000/07/17 15:44:04  llee1
// *** empty log message ***
//
//===========================================================================

#include <mtl/matrix.h>
#include <mtl/mtl.h>
#include <mtl/utils.h>
#include "mtl/harwell_boeing_stream.h"

#include <itl/interface/mtl.h>
#include <itl/preconditioner/ssor.h>
#include <itl/krylov/bicgstab.h>

/*
  In thsi example, we show how to use bicgstab algorithm.
*/
using namespace mtl;
using namespace itl;

typedef  double Type;

//begin
typedef matrix< Type, rectangle<>, 
	        array< compressed<> >, 
                row_major >::type Matrix;
//end

int main (int argc, char* argv[]) 
{
  using std::cout;
  using std::endl;

  if ( argc == 1 ) {
    cout << "Usage: " << argv[0] 
	 << " <Unsymmetric matrix in Harwell-Boeing format> "
	 << endl;
    return 0;
  }

  harwell_boeing_stream<Type> hbs(argv[1]);

  int max_iter = 256;
  //begin
  Matrix A(hbs);

  dense1D<Type> x(A.nrows(), Type(0));
  dense1D<Type> b(A.ncols());
  for (dense1D<Type>::iterator i=b.begin(); i!=b.end(); i++)
    *i = 1.;
  //SSOR preconditioner
  SSOR<Matrix> precond(A);
  //iteration
  noisy_iteration<double> iter(b, max_iter, 1e-6);
  //bicgstab algorithm
  bicgstab(A, x, b, precond(), iter);
  //end

  //verify the result
  dense1D<Type> b1(A.ncols());
  itl::mult(A, x, b1);
  itl::add(b1, itl::scaled(b, -1.), b1);


  cout << "Residual " << itl::two_norm(b1) << endl;
  return 0;
}


