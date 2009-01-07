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

#ifndef DESOLIN_CONFIGURATION_MANAGER_HPP
#define DESOLIN_CONFIGURATION_MANAGER_HPP

#include <set>

namespace desolin
{

namespace detail
{
  class Cache;
}

class ConfigurationManager
{
private:
  std::set<detail::Cache*> caches;
  bool gcc;
  bool doCodeCaching;
  bool doFusion;
  bool doHighLevelFusion;
  bool doArrayContraction;
  bool doLiveness;
  bool doSingleForLoopSparse;
  bool doSparseSpecialisation;
  static ConfigurationManager configurationManager;

  void flushCaches();

  ConfigurationManager(const ConfigurationManager&);
  ConfigurationManager& operator=(const ConfigurationManager&);
  ConfigurationManager();

public:
  static ConfigurationManager& getConfigurationManager();

  void registerCache(detail::Cache& cache);
  void unregisterCache(detail::Cache& cache);
	  
  void useGCC();
  bool usingGCC() const;

  void useICC();
  bool usingICC() const;
	
  void enableLivenessAnalysis(const bool enabled);
  bool livenessAnalysisEnabled() const;
  
  void enableCodeCaching(const bool enabled);
  bool codeCachingEnabled() const;

  void enableLoopFusion(const bool enabled);
  bool loopFusionEnabled() const;
  
  void enableHighLevelFusion(const bool enabled);
  bool highLevelFusionEnabled() const;

  void enableArrayContraction(const bool enabled);
  bool arrayContractionEnabled() const;

  void enableSingleForLoopSparseIteration(const bool enabled);
  bool singleForLoopSparseIterationEnabled() const;

  void enableSparseSpecialisation(const bool enabled);
  bool sparseSpecialisationEnabled() const;

};

}
#endif
