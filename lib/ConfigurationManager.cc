#include <desolin/Desolin_fwd.hpp>
#include <algorithm>
#include <boost/functional.hpp>

namespace desolin
{

ConfigurationManager ConfigurationManager::configurationManager;

ConfigurationManager::ConfigurationManager() : gcc(true), doFusion(true), doLiveness(true), doCodeCaching(true), doArrayContraction(true)
{
}

ConfigurationManager& ConfigurationManager::getConfigurationManager()
{
  return configurationManager;
}

void ConfigurationManager::flushCaches()
{
  std::for_each(caches.begin(), caches.end(), boost::mem_fun(&desolin_internal::Cache::flush)); 
}


void ConfigurationManager::registerCache(desolin_internal::Cache& cache)
{
  caches.insert(&cache);
}

void ConfigurationManager::unregisterCache(desolin_internal::Cache& cache)
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

}
