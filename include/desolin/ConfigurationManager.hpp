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

namespace desolin_internal
{
  class Cache;
}

namespace desolin
{
class ConfigurationManager
{
private:
  std::set<desolin_internal::Cache*> caches;
  bool gcc;
  bool doFusion;
  bool doLiveness;
  bool doCodeCaching;
  bool doArrayContraction;
  static ConfigurationManager configurationManager;

  void flushCaches();

  ConfigurationManager(const ConfigurationManager&);
  ConfigurationManager& operator=(const ConfigurationManager&);
  ConfigurationManager();

public:
  static ConfigurationManager& getConfigurationManager();

  void registerCache(desolin_internal::Cache& cache);
  void unregisterCache(desolin_internal::Cache& cache);
	  
  void useGCC();
  void useICC();
  bool usingGCC() const;
  bool usingICC() const;
	
  void enableLivenessAnalysis();
  void disableLivenessAnalysis();
  bool livenessAnalysisEnabled() const;
  
  void enableCodeCaching();
  void disableCodeCaching();
  bool codeCachingEnabled() const;

  void enableLoopFusion();
  void disableLoopFusion();
  bool loopFusionEnabled() const;
  
  void enableArrayContraction();
  void disableArrayContraction();
  bool arrayContractionEnabled() const;
};

}
#endif
