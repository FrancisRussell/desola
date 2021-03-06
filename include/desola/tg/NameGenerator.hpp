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

#ifndef DESOLA_TG_NAME_GENERATOR_HPP
#define DESOLA_TG_NAME_GENERATOR_HPP

#include <string>
#include <map>

namespace desola
{

namespace detail
{

class NameGenerator
{
private:
  std::map<const std::string, unsigned> nameCount;
  void itoa10(const unsigned value, char* result);
  
public:
  NameGenerator(); 
  std::string getName(const std::string& prefix);
};

}

}
#endif
