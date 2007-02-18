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

#include <desolin/StatisticsCollector.hpp>

namespace desolin
{

StatisticsCollector StatisticsCollector::statsCollector;

StatisticsCollector::StatisticsCollector() : compileTime(0.0), compileCount(0)
{
}

StatisticsCollector& StatisticsCollector::getStatisticsCollector()
{
  return statsCollector;
}

double StatisticsCollector::getCompileTime() const
{
  return compileTime;
}

void StatisticsCollector::addCompileTime(const double time)
{
  compileTime += time;
}

void StatisticsCollector::resetCompileTime()
{
  compileTime=0.0;
}

int StatisticsCollector::getCompileCount() const
{
  return compileCount;
}

void StatisticsCollector::incrementCompileCount()
{
  ++compileCount;
}

void StatisticsCollector::resetCompileCount()
{
  compileCount=0;
}

}
