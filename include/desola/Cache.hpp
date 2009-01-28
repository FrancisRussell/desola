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

#ifndef DESOLA_CACHE_HPP
#define DESOLA_CACHE_HPP

#include "ConfigurationManager.hpp"

namespace desola
{

namespace detail
{
	
class Cache
{
public:
  Cache()
  {
    ConfigurationManager::getConfigurationManager().registerCache(*this);
  }

  virtual void flush() =0;

  virtual ~Cache()
  {
    ConfigurationManager::getConfigurationManager().unregisterCache(*this);
  }
};

}

}
#endif
