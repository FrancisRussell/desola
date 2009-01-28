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

#include <desola/StatisticsCollector.hpp>
#include <desola/Maybe.hpp>

namespace desola
{

StatisticsCollector StatisticsCollector::statsCollector;

StatisticsCollector::StatisticsCollector() : compileTime(0.0), compileCount(0), flops(0.0)
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

Maybe<double> StatisticsCollector::getFlops() const
{
  return flops;
}

void StatisticsCollector::addFlops(const Maybe<double>& f)
{
  flops+=f;
}

void StatisticsCollector::resetFlops()
{
  flops = Maybe<double>(0.0);
}

}
