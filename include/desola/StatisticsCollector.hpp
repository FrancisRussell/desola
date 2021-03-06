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

#ifndef DESOLA_STATISTICS_COLLECTOR
#define DESOLA_STATISTICS_COLLECTOR

#include "Desola_fwd.hpp"
#include "Maybe.hpp"

namespace desola
{
	
class StatisticsCollector
{
private:
  double compileTime;
  int compileCount;
  Maybe<double> flops;
	
  StatisticsCollector(const StatisticsCollector&);
  StatisticsCollector& operator=(const StatisticsCollector&);
  StatisticsCollector();
  
  static StatisticsCollector statsCollector;

public:
  static StatisticsCollector& getStatisticsCollector();

  double getCompileTime() const;
  void addCompileTime(const double time);
  void resetCompileTime();

  int getCompileCount() const;
  void incrementCompileCount();
  void resetCompileCount();

  Maybe<double> getFlops() const;
  void addFlops(const Maybe<double>& flops);
  void resetFlops();
};

}
#endif
