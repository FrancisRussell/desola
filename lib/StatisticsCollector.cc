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
