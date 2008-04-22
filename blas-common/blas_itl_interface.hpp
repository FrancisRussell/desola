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
  using desolin::blas_wrappers::BLASVector;
  using desolin::blas_wrappers::BLASGeneralMatrix;
  
  //: The vector type used inside of the ITL routines for work space
  template <typename Vec>
  struct itl_traits {
    // TODO: Work out what this does before allowing it to be defined.
    //typedef referencing_object_tag vector_category;
    typedef typename Vec::value_type value_type;
    typedef typename Vec::size_type size_type;
  };
 
  template <class Vec>
  struct Scaled {
    typedef typename Vec::value_type T;
    inline Scaled(const Vec& v, const T& alpha) : _alpha(alpha), _v(v) { }
    inline T alpha() const { return _alpha; }
    inline const Vec& vec() const { return _v; }
    inline int nrows() const { return _v.nrows(); }

  protected:
    T _alpha;  
    const Vec _v;
  };

  // Getting the scaling factor for scaled and unscaled vectors
  template<typename Vec>
  inline typename Vec::value_type scale_factor(const Vec& v)
  {
    return static_cast<typename Vec::value_type>(1.0);
  }
  
  template<typename Vec>
  inline typename Vec::value_type scale_factor(const Scaled<Vec>& s)
  {
    return s.alpha();
  }

  // Getting the data for scaled and unscaled vectors
  template<typename Vec>
  inline typename Vec::value_type* get_data(Vec& v)
  {
    return v.data();
  }

  template<typename Vec>
  inline const typename Vec::value_type* get_data(const Vec& v)
  {
    return v.data();
  }

  template<typename Vec>
  inline const typename Vec::value_type* get_data(const Scaled<Vec>& s)
  {
    return s.vec().data();
  }

  // ITL interface implementation
  inline void copy(const BLASVector<double>& a, BLASVector<double>& b)
  {
    cblas_dcopy(a.nrows(), a.data(), 1, b.data(), 1);
  }
 
  inline void copy(const Scaled< BLASVector<double> >& a, BLASVector<double>& b)
  {
    cblas_dcopy(a.nrows(), a.vec().data(), 1, b.data(), 1);
    cblas_dscal(b.nrows(), a.alpha(), b.data(), 1);  
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
 
  template<typename VecX>
  inline void add(const VecX& x, BLASVector<double>& y)
  {
    cblas_daxpy(x.nrows(), scale_factor(x), get_data(x), 1, y.data(), 1);
  }

  template<typename VecX, typename VecY>
  inline void add(const VecX& x, const VecY& y, BLASVector<double>& z) 
  {
    itl::copy(x, z);
    itl::add(y, z);
  }

  template<typename VecX, typename VecY, typename VecZ>
  inline void add(const VecX& x, const VecY& y, const VecZ& z, BLASVector<double>& r) 
  {
    itl::copy(x, r);
    itl::add(y, r);
    itl::add(z, r);
  }

  template<typename VecX>
  inline void mult(const BLASGeneralMatrix<double>& A, const VecX& x, BLASVector<double>& y) 
  {
    cblas_dgemv(CblasRowMajor, CblasNoTrans, A.nrows(), A.ncols(), scale_factor(x), A.data(), A.ncols(), get_data(x), 1, 0.0, y.data(), 1);
  }

  template<typename VecX, typename VecY>
  inline void mult(const BLASGeneralMatrix<double>& A, const VecX& x, const VecY& y, BLASVector<double>& z)
  {
    cblas_dcopy(y.nrows(), y.data(), 1, z.data(), 1);
    cblas_dgemv(CblasRowMajor, CblasNoTrans, A.nrows(), A.ncols(), scale_factor(x), A.data(), A.ncols(), get_data(x), 1, scale_factor(y), z.data(), 1);
  }

  inline void scale(BLASVector<double>& s, const itl_traits< BLASVector<double> >::value_type& alpha)
  {
    cblas_dscal(s.nrows(), alpha, s.data(), 1);
  }

  inline Scaled< BLASVector<double> > scaled(const BLASVector<double>& s, const itl_traits< BLASVector<double> >::value_type& alpha) 
  {
    return Scaled< BLASVector<double> >(s, alpha);
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
