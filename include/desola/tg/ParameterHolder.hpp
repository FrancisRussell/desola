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

#ifndef DESOLA_TG_PARAMETER_HOLDER_HPP
#define DESOLA_TG_PARAMETER_HOLDER_HPP

#include <utility>
#include <string>
#include <map>
#include <TaskGraph>

namespace desola
{

namespace detail
{

class ParameterHolder
{
private:
  ParameterHolder(const ParameterHolder&);
  ParameterHolder& operator=(const ParameterHolder&);
  std::map<std::string, void*> parameters;
 
  static void setParameter(tg::tuTaskGraph& taskGraphObject, const std::pair<const std::string, void*>& parameterMapping);

public:
  ParameterHolder();
  void addParameter(const std::string& name, void* value);
  void setParameters(tg::tuTaskGraph& taskGraphObject) const;
};

}

}
#endif
