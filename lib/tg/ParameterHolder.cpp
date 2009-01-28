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

#include <desola/tg/ParameterHolder.hpp>
#include <string>
#include <map>
#include <cassert>
#include <algorithm>
#include <utility>
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <TaskGraph>

namespace desola
{

namespace detail
{

ParameterHolder::ParameterHolder()
{
}

void ParameterHolder::addParameter(const std::string& name, void* value)
{
  assert(value != NULL);
  assert(parameters.find(name) == parameters.end());
  parameters[name]=value;
}

void ParameterHolder::setParameter(tg::tuTaskGraph& taskGraphObject, const std::pair<const std::string, void*>& parameterMapping)
{
  taskGraphObject.setParameter(parameterMapping.first.c_str(), parameterMapping.second);
}
  
void ParameterHolder::setParameters(tg::tuTaskGraph& taskGraphObject) const
{
  std::for_each(parameters.begin(), parameters.end(), boost::bind(setParameter, boost::ref(taskGraphObject), _1));
}

}

}
