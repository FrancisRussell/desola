#ifndef DESOLIN_STATISTICS_COLLECTOR
#define DESOLIN_STATISTICS_COLLECTOR

namespace desolin
{
	
class StatisticsCollector
{
private:
  double compileTime;
  int compileCount;
	
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
};

}
#endif
