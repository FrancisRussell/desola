#ifndef DESOLIN_STATISTICS_GENERATOR_HPP
#define DESOLIN_STATISTICS_GENERATOR_HPP

#include <desolin/ConfigurationManager.hpp>
#include <iostream>
#include <string>
#include <ctime>
#include <boost/timer.hpp>
#include <boost/filesystem/path.hpp>

class StatisticsGenerator
{
private:
  boost::timer timer;
  const desolin::ConfigurationManager& configManager;
  
public:
  StatisticsGenerator() : configManager(desolin::ConfigurationManager::getConfigurationManager())
  {
  }
  
  template<typename IterationType>
  void printLongResults(const std::string& matrixName, IterationType& iter)
  {
    std::cout.precision(5);
    std::cout.setf(std::ios::fixed);
    std::cout << "Library: desolin" << std::endl;
    std::cout << "Matrix: " << getLeaf(matrixName) << std::endl;
    std::cout << "Compiler: " << getCompiler() << std::endl;
    std::cout << "Code Caching: " << getStatus(configManager.codeCachingEnabled()) << std::endl;
    std::cout << "Loop Fusion: " << getStatus(configManager.loopFusionEnabled()) << std::endl;
    std::cout << "Array Contraction: " << getStatus(configManager.arrayContractionEnabled()) << std::endl;
    std::cout << "Liveness Analysis: " << getStatus(configManager.livenessAnalysisEnabled()) << std::endl;
    std::cout << "Time per Iteration: " << timer.elapsed() / iter.iterations() << " seconds" << std::endl;
    std::cout << "Total Time: " << timer.elapsed() << " seconds" << std::endl;
  }

  template<typename IterationType>
  void printShortResults(const std::string& matrixName, IterationType& iter)
  {
    std::cout.precision(5);
    std::cout.setf(std::ios::fixed);
    std::cout << "Library: desolin\t";
    std::cout << "Matrix: " << getLeaf(matrixName) << "\t";
    std::cout << "Compiler: " << getCompiler() << "\t";
    std::cout << "Code_cache: " << getStatus(configManager.codeCachingEnabled()) << "\t";
    std::cout << "Fusion: " << getStatus(configManager.loopFusionEnabled()) << "\t";
    std::cout << "Contraction: " << getStatus(configManager.arrayContractionEnabled()) << "\t";
    std::cout << "Liveness: " << getStatus(configManager.livenessAnalysisEnabled()) << "\t";
    std::cout << "Iter_time: " << timer.elapsed() / iter.iterations() << "\t";
    std::cout << "Total_time: " << timer.elapsed() << std::endl;
  }

  template<typename IterationType>
  void printResults(const std::string& matrixName, IterationType& iter, const bool multiLine)
  {
    if(multiLine)
    {
      printLongResults(matrixName, iter);
    }
    else
    {
      printShortResults(matrixName, iter);
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
};

#endif
