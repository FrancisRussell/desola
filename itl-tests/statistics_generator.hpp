#ifndef DESOLIN_STATISTICS_GENERATOR_HPP
#define DESOLIN_STATISTICS_GENERATOR_HPP

#include <desolin/ConfigurationManager.hpp>
#include <desolin/StatisticsCollector.hpp>
#include <iostream>
#include <string>
#include <sys/time.h>
#include <boost/filesystem/path.hpp>

class StatisticsGenerator
{
private:
  double startTime;
  const desolin::ConfigurationManager& configManager;
  const desolin::StatisticsCollector& statsCollector;
  
public:
  StatisticsGenerator() : startTime(getTime()), configManager(desolin::ConfigurationManager::getConfigurationManager()), statsCollector(desolin::StatisticsCollector::getStatisticsCollector())
  {
  }
  
  template<typename MatrixStreamType, typename IterationType>
  void printLongResults(const std::string& matrixPath, const MatrixStreamType& matrixStream, IterationType& iter)
  {
    const double elapsed = getTime() - startTime;
    std::cout.precision(5);
    std::cout.setf(std::ios::fixed);
    std::cout << "Library: desolin" << std::endl;
    std::cout << "Matrix: " << getLeaf(matrixPath) << std::endl;
    std::cout << "Matrix Size: " << matrixStream.ncols() << std::endl;
    std::cout << "Compiler: " << getCompiler() << std::endl;
    std::cout << "Code Caching: " << getStatus(configManager.codeCachingEnabled()) << std::endl;
    std::cout << "Loop Fusion: " << getStatus(configManager.loopFusionEnabled()) << std::endl;
    std::cout << "Array Contraction: " << getStatus(configManager.arrayContractionEnabled()) << std::endl;
    std::cout << "Iterations: " << iter.iterations() << std::endl;
    std::cout << "Liveness Analysis: " << getStatus(configManager.livenessAnalysisEnabled()) << std::endl;
    std::cout << "Time per Iteration: " << elapsed / iter.iterations() << " seconds" << std::endl;
    std::cout << "Compile Time: " << statsCollector.getCompileTime() << " seconds" << std::endl;
    std::cout << "Compile Count: " << statsCollector.getCompileCount() << std::endl;
    std::cout << "Total Time: " << elapsed << " seconds" << std::endl;
  }

  template<typename MatrixStreamType, typename IterationType>
  void printShortResults(const std::string& matrixPath, const MatrixStreamType& matrixStream, IterationType& iter)
  {
    const double elapsed = getTime() - startTime;
    std::cout.precision(5);
    std::cout.setf(std::ios::fixed);
    std::cout << "Library: desolin\t";
    std::cout << "Matrix: " << getLeaf(matrixPath) << "\t";
    std::cout << "Matrix Size: " << matrixStream.ncols() << "\t";
    std::cout << "Compiler: " << getCompiler() << "\t";
    std::cout << "Code_cache: " << getStatus(configManager.codeCachingEnabled()) << "\t";
    std::cout << "Fusion: " << getStatus(configManager.loopFusionEnabled()) << "\t";
    std::cout << "Contraction: " << getStatus(configManager.arrayContractionEnabled()) << "\t";
    std::cout << "Liveness: " << getStatus(configManager.livenessAnalysisEnabled()) << "\t";
    std::cout << "Iterations: " << iter.iterations() << "\t";
    std::cout << "Iter_time: " << elapsed / iter.iterations() << "\t";
    std::cout << "Compile_time: " << statsCollector.getCompileTime() << "\t";
    std::cout << "Compile_count: " << statsCollector.getCompileCount() << "\t";
    std::cout << "Total_time: " << elapsed << std::endl;
  }

  template<typename MatrixStreamType, typename IterationType>
  void printResults(const std::string& matrixPath, const MatrixStreamType& matrixStream, IterationType& iter, const bool multiLine)
  {
    if(multiLine)
    {
      printLongResults(matrixPath, matrixStream, iter);
    }
    else
    {
      printShortResults(matrixPath, matrixStream, iter);
    }
  }

  std::string getCompiler() const
  {
    if (configManager.usingGCC())
    {
      return "GCC";
    }
    else if (configManager.usingICC())
    {
      return "ICC";
    }
    else
    {
      return "Unknown";
    }
  }

  static std::string getStatus(const bool b)
  {
    if (b)
    {
      return "On";
    }
    else
    {
      return "Off";
    }
  }

  static std::string getLeaf(const std::string& pathString)
  {
    boost::filesystem::path path(pathString);
    return path.leaf();
  }

  static double getTime()
  {
   timeval time;
   gettimeofday(&time, NULL);
   return time.tv_sec + time.tv_usec/1000000.0;
  }
};

#endif
