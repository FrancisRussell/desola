#ifndef DESOLA_IMKL_BLAS_INTERFACE_HPP
#define DESOLA_IMKL_BLAS_INTERFACE_HPP

#include "blas_itl_interface.hpp"

namespace itl
{
  using desola::blas_wrappers::BLASVector;
  using desola::blas_wrappers::BLASCRSMatrix;

  namespace
  {
    inline void mult(const BLASCRSMatrix<double>& A, const BLASVector<double>& x, BLASVector<double>& y, const bool transpose) 
    {
      int rows = A.nrows();
      char transA = transpose ? 't' : 'n';
      mkl_cspblas_dcsrgemv(&transA, &rows, const_cast<double*>(A.get_val()), const_cast<int*>(A.get_row_ptr()), const_cast<int*>(A.get_col_ind()), const_cast<double*>(x.data()), y.data());
    }

    inline void mult(const BLASCRSMatrix<double>& A, const Scaled< BLASVector<double> >& x, BLASVector<double>& y, const bool transpose) 
    {
      BLASVector<double> unscaled_x(x.nrows());
      itl::copy(x, unscaled_x);
      mult(A, unscaled_x, y, transpose);
    }
  }

  template<typename VecX>
  inline void mult(const BLASCRSMatrix<double>& A, const VecX& x, BLASVector<double>& y) 
  {
    mult(A, x, y, false);
  }

  template <class VecX>
  inline void trans_mult(const BLASCRSMatrix<double>& A, const VecX& x, BLASVector<double>& y) 
  {
    mult(A, x, y, true);
  }

  template<typename VecX, typename VecY>
  inline void mult(const BLASCRSMatrix<double>& A, const VecX& x, const VecY& y, BLASVector<double>& z)
  {
    itl::mult(A, x, z);
    itl::add(y, z);
  }
}

#endif
