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

#include <desolin/ConfigurationManager.hpp>
#include <desolin/Cache.hpp>
#include <algorithm>
#include <boost/functional.hpp>

namespace desolin
{

ConfigurationManager ConfigurationManager::configurationManager;

ConfigurationManager::ConfigurationManager() : gcc(true), doFusion(true), doLiveness(true),
  doCodeCaching(true), doArrayContraction(true), doSparseSpecialisation(false)
{
}

ConfigurationManager& ConfigurationManager::getConfigurationManager()
{
  return configurationManager;
}

void ConfigurationManager::flushCaches()
{
  std::for_each(caches.begin(), caches.end(), boost::mem_fun(&detail::Cache::flush)); 
}


void ConfigurationManager::registerCache(detail::Cache& cache)
{
  caches.insert(&cache);
}

void ConfigurationManager::unregisterCache(detail::Cache& cache)
{
  caches.erase(&cache);
}

void ConfigurationManager::useGCC()
{
  flushCaches();
  gcc=true;
}

void ConfigurationManager::useICC()
{
  flushCaches();
  gcc=false;
}

bool ConfigurationManager::usingGCC() const
{
  return gcc;
}

bool ConfigurationManager::usingICC() const
{
  return !gcc;
}

void ConfigurationManager::enableLivenessAnalysis()
{
  flushCaches();
  doLiveness=true;
}

void ConfigurationManager::disableLivenessAnalysis()
{
  flushCaches();
  doLiveness=false;
}

bool ConfigurationManager::livenessAnalysisEnabled() const
{
  return doLiveness;
}

void ConfigurationManager::enableCodeCaching()
{
  flushCaches();
  doCodeCaching=true;
}

void ConfigurationManager::disableCodeCaching()
{
  flushCaches();
  doCodeCaching=false;
}

bool ConfigurationManager::codeCachingEnabled() const
{
  return doCodeCaching;
}

void ConfigurationManager::enableLoopFusion() 
{
  flushCaches();
  doFusion = true;
}

void ConfigurationManager::disableLoopFusion()
{
  flushCaches();
  doFusion = false;
}

bool ConfigurationManager::loopFusionEnabled() const
{
  return doFusion;
}

void ConfigurationManager::enableArrayContraction()
{
  flushCaches();
  doArrayContraction=true;
}

void ConfigurationManager::disableArrayContraction()
{
  flushCaches();
  doArrayContraction=false;
}

bool ConfigurationManager::arrayContractionEnabled() const
{
  return doArrayContraction;
}

void ConfigurationManager::enableSparseSpecialisation(const bool enabled)
{
  doSparseSpecialisation = enabled;
}

bool ConfigurationManager::sparseSpecialisationEnabled() const
{
  return doSparseSpecialisation;
}

}
