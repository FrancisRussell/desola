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

#ifndef DESOLIN_MTL_STATISTICS_GENERATOR_HPP
#define DESOLIN_MTL_STATISTICS_GENERATOR_HPP

#include <iostream>
#include <string>
#include <sys/time.h>
#include <boost/filesystem/path.hpp>

class StatisticsGenerator
{
private:
  double startTime;
  
public:
  StatisticsGenerator() : startTime(getTime())
  {
  }
  
  template<typename MatrixStreamType, typename IterationType>
  void printLongResults(const std::string& matrixPath, const MatrixStreamType& matrixStream, IterationType& iter)
  {
    const double elapsed = getTime() - startTime;
    std::cout.precision(5);
    std::cout.setf(std::ios::fixed);
    std::cout << "Library: MTL" << std::endl;
    std::cout << "Matrix: " << getLeaf(matrixPath) << std::endl;
    std::cout << "Matrix Size: " << matrixStream.ncols() << std::endl;
    std::cout << "Iterations: " << iter.iterations() << std::endl;
    std::cout << "Time per Iteration: " << elapsed / iter.iterations() << " seconds" << std::endl;
    std::cout << "Total Time: " << elapsed << " seconds" << std::endl;
  }

  template<typename MatrixStreamType, typename IterationType>
  void printShortResults(const std::string& matrixPath, const MatrixStreamType& matrixStream, IterationType& iter)
  {
    const double elapsed = getTime() - startTime;
    std::cout.precision(5);
    std::cout.setf(std::ios::fixed);
    std::cout << "Library: MTL\t";
    std::cout << "Matrix: " << getLeaf(matrixPath) << "\t";
    std::cout << "Matrix Size: " << matrixStream.ncols() << "\t";
    std::cout << "Iterations: " << iter.iterations() << "\t";
    std::cout << "Iter_time: " << elapsed / iter.iterations() << "\t";
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
