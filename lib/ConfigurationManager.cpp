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

#include <desola/ConfigurationManager.hpp>
#include <desola/Cache.hpp>
#include <algorithm>
#include <boost/functional.hpp>

namespace desola
{

ConfigurationManager ConfigurationManager::configurationManager;

ConfigurationManager::ConfigurationManager() : gcc(true), doCodeCaching(true), 
  doFusion(true), doHighLevelFusion(true), doArrayContraction(true), doLiveness(false), doSingleForLoopSparse(false), 
  doSparseSpecialisation(false)
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

void ConfigurationManager::enableLivenessAnalysis(const bool enabled)
{
  flushCaches();
  doLiveness=enabled;
}

bool ConfigurationManager::livenessAnalysisEnabled() const
{
  return doLiveness;
}

void ConfigurationManager::enableCodeCaching(const bool enabled)
{
  flushCaches();
  doCodeCaching=enabled;
}

bool ConfigurationManager::codeCachingEnabled() const
{
  return doCodeCaching;
}

void ConfigurationManager::enableLoopFusion(const bool enabled) 
{
  flushCaches();
  doFusion = enabled;
}

bool ConfigurationManager::loopFusionEnabled() const
{
  return doFusion;
}

void ConfigurationManager::enableHighLevelFusion(const bool enabled)
{
  flushCaches();
  doHighLevelFusion = enabled;
}

bool ConfigurationManager::highLevelFusionEnabled() const
{
  return doHighLevelFusion;
}

void ConfigurationManager::enableArrayContraction(const bool enabled)
{
  flushCaches();
  doArrayContraction=enabled;
}

bool ConfigurationManager::arrayContractionEnabled() const
{
  return doArrayContraction;
}

void ConfigurationManager::enableSingleForLoopSparseIteration(const bool enabled)
{
  flushCaches();
  doSingleForLoopSparse = enabled;
}

bool ConfigurationManager::singleForLoopSparseIterationEnabled() const
{
  return doSingleForLoopSparse;
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
