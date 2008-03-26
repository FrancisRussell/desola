/****************************************************************************/
/* Copyright 2005-2008, Francis Russell                                     */
/*                                                                          */
/* Licensed under the Apache License, Version 2.0 (the License);            */
/* you may not use this file except in compliance with the License.         */
/* You may obtain a copy of the License at                                  */
/*                                                                          */
/*     http://www.apache.org/licenses/LICENSE-2.0                           */
/*                                                                          */
/* Unless required by applicable law or agreed to in writing, software      */
/* distributed under the License is distributed on an AS IS BASIS,          */
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. */
/* See the License for the specific language governing permissions and      */
/* limitations under the License.                                           */
/*                                                                          */
/****************************************************************************/

#ifndef ITL_BLAS_INTERFACE_HPP
#define ITL_BLAS_INTERFACE_HPP

#include <cassert>
#include <complex>
#include <itl/itl_tags.h>
#include <itl/number_traits.h>

namespace itl {
  using desolin_blas_wrappers::BLASVector;
  using desolin_blas_wrappers::BLASGeneralMatrix;
  
  //: The vector type used inside of the ITL routines for work space
  template <typename Vec>
  struct itl_traits {
    // TODO: Work out what this does before allowing it to be defined.
    //typedef referencing_object_tag vector_category;
    typedef typename Vec::value_type value_type;
    typedef typename Vec::size_type size_type;
  };
 
  inline void copy(const BLASVector<double>& a, BLASVector<double>& b)
  {
    cblas_dcopy(a.nrows(), a.data(), 1, b.data(), 1);
  }
  
  inline double dot(const BLASVector<double>& a, const BLASVector<double>& b)
  {
    return cblas_ddot(a.nrows(), a.data(), 1, b.data(), 1);
  }
  
  inline double dot_conj(const BLASVector<double>& a, const BLASVector<double>& b) 
  {
    return cblas_ddot(a.nrows(), a.data(), 1, b.data(), 1);
  }
  
  inline double two_norm(const BLASVector<double>& v)
  {
    return cblas_dnrm2(v.nrows(), v.data(), 1);
  }
 
  inline void add(const BLASVector<double>& x, BLASVector<double>& y)
  {
    cblas_daxpy(x.nrows(), 1.0, x.data(), 1, y.data(), 1);
  }

  inline void add(const BLASVector<double>& x, const BLASVector<double>& y, BLASVector<double>& z) 
  {
    itl::copy(x, z);
    itl::add(y, z);
  }

  inline void add(const BLASVector<double>& x, const BLASVector<double>& y, const BLASVector<double>& z, BLASVector<double>& r) 
  {
    itl::copy(x, r);
    itl::add(y, r);
    itl::add(z, r);
  }

  inline void mult(const BLASGeneralMatrix<double>& A, const BLASVector<double>& x, BLASVector<double>& y) 
  {
    cblas_dgemv(CblasRowMajor, CblasNoTrans, A.nrows(), A.ncols(), 1.0, A.data(), A.ncols(), x.data(), 1, 0.0, y.data(), 1);
  }

  inline void mult(const BLASGeneralMatrix<double>& A, const BLASVector<double>& x, const BLASVector<double>& y, BLASVector<double>& z)
  {
    itl::copy(y, z);
    cblas_dgemv(CblasRowMajor, CblasNoTrans, A.nrows(), A.ncols(), 1.0, A.data(), A.ncols(), x.data(), 1, 1.0, z.data(), 1);
  }

  inline void scale(BLASVector<double>& s, const itl_traits< BLASVector<double> >::value_type& alpha)
  {
    cblas_dscal(s.nrows(), alpha, s.data(), 1);
  }

  inline BLASVector<double> scaled(const BLASVector<double>& s, const itl_traits< BLASVector<double> >::value_type& alpha) 
  {
    BLASVector<double> result(s.nrows());
    itl::copy(s, result);
    itl::scale(result, alpha);
    return result;
  }

  template<typename T>
  inline typename BLASVector<T>::size_type size(const BLASVector<T>& x) 
  {
    return x.nrows();
  }

// Only define this when semantics are understood
/*
  template <class Vector>
  inline void resize(Vector& x, const int sz) 
  {
    x.resize(sz);
  }
*/
  
  template <class Matrix, class VecX, class VecY>
  inline void trans_mult(const Matrix& A, const VecX& x, VecY& y) 
  {
    cblas_dgemv(CblasRowMajor, CblasTrans, A.nrows(), A.ncols(), 1.0, A.data(), A.ncols(), x.data(), 1, 0.0, y.data(), 1);
  }
}

#endif
