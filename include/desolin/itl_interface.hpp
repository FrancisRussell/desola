/****************************************************************************/
/* Copyright 2005-2006, Francis Russell                                     */
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

#ifndef ITL_DESOLIN_INTERFACE_HPP
#define ITL_DESOLIN_INTERFACE_HPP

#include <cassert>
#include <complex>
#include <itl/itl_tags.h>
#include <itl/number_traits.h>

namespace itl {
  
  //: The vector type used inside of the ITL routines for work space
  template <typename Vec>
  struct itl_traits {
    typedef referencing_object_tag vector_category;
    typedef typename Vec::value_type value_type;
    typedef typename Vec::size_type size_type;
  };
  
  template <class Vec>
  inline typename itl::number_traits< typename Vec::value_type >::magnitude_type two_norm(const Vec& v)
  {
    return v.norm();
  }

  template <class VecA, class VecB>
  inline void copy(const VecA& a, VecB& b)
  {
    b = a;
  }
  
  template <class Matrix, class VecX, class VecY>
  inline void mult(const Matrix& A, const VecX& x, VecY& y) 
  {
    y = A * x;
  }

  template <class Matrix, class VecX, class VecY, class VecZ>
  inline void mult(const Matrix& A, const VecX& x, const VecY& y, VecZ& z)
  {
    z = y + (A * x);
  }

  template <class VecA, class VecB>
  inline typename VecA::value_type dot(const VecA& a, const VecB& b)
  {
    return a.dot(b);
  }
  
  template <class VecA, class VecB>
  inline typename VecA::value_type dot_conj(const VecA& a, const VecB& b) 
  {
    return a.dot(b);
  }
  
  template <class VecX, class VecY>
  inline void add(const VecX& x, VecY& y)
  {
    y = y + x;
  }

  template <class VecX, class VecY, class VecZ>
  inline void add(const VecX& x, const VecY& y, VecZ& z) 
  {
    z = x + y;
  }

  template <class VecX, class VecY, class VecZ, class VecR>
  inline void add(const VecX& x, const VecY& y, const VecZ& z, VecR& r) 
  {
    r = x + y + z;
  }

  template <class Scalable>
  inline Scalable scaled(const Scalable& s, const typename itl_traits<Scalable>::value_type& alpha) 
  {
    return s * alpha;
  }

  template <typename Scalable>
  inline void scale(Scalable& s, const typename itl_traits<Scalable>::value_type& alpha)
  {
    s = s * alpha;
  }

  template <class Vector>
  inline typename Vector::size_type size(const Vector& x) 
  {
    return x.numRows();
  }

  template <class Matrix, class VecX, class VecY>
  inline void trans_mult(const Matrix& A, const VecX& x, VecY& y) 
  {
    y = A.trans_mult(x);
  }
}

#endif
