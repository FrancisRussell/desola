#ifndef DESOLIN_MTL_STATISTICS_GENERATOR_HPP
#define DESOLIN_MTL_STATISTICS_GENERATOR_HPP

#include <iostream>
#include <string>
#include <boost/timer.hpp>
#include <boost/filesystem/path.hpp>

class StatisticsGenerator
{
private:
  boost::timer timer;
  
public:
  StatisticsGenerator()
  {
  }
  
  template<typename MatrixStreamType, typename IterationType>
  void printLongResults(const std::string& matrixPath, const MatrixStreamType& matrixStream, IterationType& iter)
  {
    std::cout.precision(5);
    std::cout.setf(std::ios::fixed);
    std::cout << "Library: MTL" << std::endl;
    std::cout << "Matrix: " << getLeaf(matrixPath) << std::endl;
    std::cout << "Matrix Size: " << matrixStream.ncols() << std::endl;
    std::cout << "Iterations: " << iter.iterations() << std::endl;
    std::cout << "Time per Iteration: " << timer.elapsed() / iter.iterations() << " seconds" << std::endl;
    std::cout << "Total Time: " << timer.elapsed() << " seconds" << std::endl;
  }

  template<typename MatrixStreamType, typename IterationType>
  void printShortResults(const std::string& matrixPath, const MatrixStreamType& matrixStream, IterationType& iter)
  {
    std::cout.precision(5);
    std::cout.setf(std::ios::fixed);
    std::cout << "Library: MTL\t";
    std::cout << "Matrix: " << getLeaf(matrixPath) << "\t";
    std::cout << "Matrix Size: " << matrixStream.ncols() << "\t";
    std::cout << "Iterations: " << iter.iterations() << "\t";
    std::cout << "Iter_time: " << timer.elapsed() / iter.iterations() << "\t";
    std::cout << "Total_time: " << timer.elapsed() << std::endl;
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
