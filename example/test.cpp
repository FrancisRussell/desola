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

#include <desolin/Desolin.hpp>
#include <TaskGraph>
#include <vector>
#include <iostream>
using namespace desolin;

int main()
{
  Scalar<double> aa = -5.4;
  Scalar<double> bb = -12.2;

  std::cout << abs(aa + bb) << std::endl;
  
  Scalar<double> x(10);
  Scalar<double> y(44);
  Scalar<double> c = x+y;
  c = c+c;
  std::cout << c << std::endl;
  Vector<double> q(5, 4);
  Vector<double> r = q*c;
  std::cout << r.norm() << std::endl;

  Matrix<double> m1(5,4);
  Matrix<double> m2(4,7);
  Matrix<double> m3 = m1*m2;
  std::cout << "m3(1,1) = " << m3(1,1).value() << std::endl;
  m3(1,1) = c;
  std::cout << "m3 = " << m3 << std::endl;
  std::cout << "-m3 = " << -m3 << q << std::endl;
  std::vector< Scalar<double> > hello;
  hello.push_back(m3(1,1) = c);

  harwell_boeing_stream<double> hbstream("../test-matrices/bcsstk14.rsa");
  Matrix<double> hb(hbstream);
}
