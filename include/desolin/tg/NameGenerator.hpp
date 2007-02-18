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

#ifndef DESOLIN_TG_NAME_GENERATOR_HPP
#define DESOLIN_TG_NAME_GENERATOR_HPP

#include <boost/format.hpp>
#include <string>
#include <map>

namespace desolin
{

namespace detail
{

class NameGenerator
{
private:
  boost::format formatter;
  std::map<const std::string, int> nameCount;
  
public:
  NameGenerator(); 
  std::string getName(const std::string& prefix);
};

}

}
#endif
