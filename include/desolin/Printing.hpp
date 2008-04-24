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

#include <iosfwd>
#include <cstddef>
#include <desolin/Desolin_fwd.hpp>

namespace desolin
{

template<typename T_element>
std::ostream& operator<< (std::ostream& o, const Scalar<T_element>& s)
{
  return o << s.value();
}

template<typename T_element>
std::ostream& operator<< (std::ostream& o, const Vector<T_element>& v)
{
  const std::size_t rows = v.numRows();
  o << "Vector: " << rows << std:: endl << "[";
  for(std::size_t row=0; row<rows; ++row)
  {
    o.width(10);
    o << v(row).value();
    if (row!=rows-1)
      o <<", ";
  }
  o << "]";
  return o << std::endl;
}

template<typename T_element>
std::ostream& operator<< (std::ostream& o, const Matrix<T_element>& m)
{
  const std::size_t rows = m.numRows();
  const std::size_t cols = m.numCols();

  o << "Matrix: " << rows << " * " << cols << std::endl << "[" << std::endl;
  for(std::size_t row=0; row<rows; ++row)
  {
    o << "[";
    for(std::size_t col=0; col<cols; ++col)
    {
      o.width(10);
      o << m(row,col).value();
      if (col!=cols-1)
        o <<", ";
    }
    o << "]" << std::endl;
  }
  o << "]";
  return o << std::endl;
}

}
