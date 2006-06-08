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
  void printLongResults(const MatrixStreamType& matrixStream, IterationType& iter)
  {
    std::cout.precision(5);
    std::cout.setf(std::ios::fixed);
    std::cout << "Library: MTL" << std::endl;
    std::cout << "Matrix: " << getLeaf(matrixStream.get_filename()) << std::endl;
    std::cout << "Matrix Size: " << matrixStream.ncols() << std::endl;
    std::cout << "Time per Iteration: " << timer.elapsed() / iter.iterations() << " seconds" << std::endl;
    std::cout << "Total Time: " << timer.elapsed() << " seconds" << std::endl;
  }

  template<typename MatrixStreamType, typename IterationType>
  void printShortResults(const MatrixStreamType& matrixStream, IterationType& iter)
  {
    std::cout.precision(5);
    std::cout.setf(std::ios::fixed);
    std::cout << "Library: desolin\t";
    std::cout << "Matrix: " << getLeaf(matrixStream.get_filename()) << "\t";
    std::cout << "Matrix Size: " << matrixStream.ncols() << "\t";
    std::cout << "Iter_time: " << timer.elapsed() / iter.iterations() << "\t";
    std::cout << "Total_time: " << timer.elapsed() << std::endl;
  }

  template<typename MatrixStreamType, typename IterationType>
  void printResults(const MatrixStreamType& matrixStream, IterationType& iter, const bool multiLine)
  {
    if(multiLine)
    {
      printLongResults(matrixStream, iter);
    }
    else
    {
      printShortResults(matrixStream, iter);
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
