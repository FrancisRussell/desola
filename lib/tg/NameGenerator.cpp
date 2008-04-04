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

#include <boost/scoped_array.hpp>
#include <desolin/tg/NameGenerator.hpp>
#include <cstring>
#include <cmath>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <iostream>


namespace desolin
{

namespace detail
{

NameGenerator::NameGenerator()
{
}

void NameGenerator::itoa10(const unsigned value, char* result)
{
  const int base = 10;
  char* out = result;
  int quotient = value;
  do 
  {
    *out = "0123456789"[std::abs(quotient%base)];
    ++out;
    quotient /= base;
  } 
  while (quotient);
       
  std::reverse(result, out);
  *out = 0;
}

// Name generation with either boost::format or std::stringstream appears to be enough of an overhead to
// justify this rather ugly implementation

std::string NameGenerator::getName(const std::string& prefix)
{
  // Conservative approximation
  const std::size_t maxSuffixLength = 1 + (sizeof(unsigned)*CHAR_BIT)/3;

  // String format: prefix+_+suffix+terminator
  boost::scoped_array<char> name(new char[prefix.length() + 1 + maxSuffixLength + 1]);
  char* out = name.get();

  strcpy(out, prefix.c_str());
  out+=prefix.length();
  
  *out++='_';

  itoa10(nameCount[prefix]++, out);
  return name.get();
}

}

}
