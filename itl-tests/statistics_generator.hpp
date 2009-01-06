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

#ifndef DESOLIN_DESOLIN_STATISTICS_GENERATOR_HPP
#define DESOLIN_DESOLIN_STATISTICS_GENERATOR_HPP

#include <desolin/ConfigurationManager.hpp>
#include <desolin/StatisticsCollector.hpp>
#include <boost/filesystem/path.hpp>
#include <iostream>
#include <string>
#include <sys/time.h>
#include "solver_options.hpp"

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
  
  template<typename MatrixType, typename IterationType>
  void printLongResults(const MatrixType& matrix, IterationType& iter, const SolverOptions& options)
  {
    const double elapsed = getTime() - startTime;
    std::cout.precision(5);
    std::cout.setf(std::ios::fixed);
    std::cout << "Library: desolin" << std::endl;
    std::cout << "Matrix: " << getLeaf(options.getFile()) << std::endl;
    std::cout << "Matrix Size: " << num_cols(matrix) << std::endl;
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
    std::cout << "FLOPs: " << statsCollector.getFlops() << std::endl;
    std::cout << "High-Level Fusion: " << getStatus(configManager.highLevelFusionEnabled()) << std::endl;
    std::cout << "Single For-Loop Sparse Iteration: " << getStatus(configManager.singleForLoopSparseIterationEnabled()) << std::endl;
    std::cout << "Sparse Row Length Specialisation: " << getStatus(configManager.sparseSpecialisationEnabled()) << std::endl;

    if (options.useSparse())
      std::cout << "NNZ: " << nnz(matrix) << std::endl;

    if (configManager.livenessAnalysisEnabled())
      std::cout << std::endl << "Warning: FLOPs figure may be misleading with liveness analysis enabled." << std::endl;
  }

  template<typename MatrixType, typename IterationType>
  void printShortResults(const MatrixType& matrix, IterationType& iter, const SolverOptions& options)
  {
    const double elapsed = getTime() - startTime;
    std::cout.precision(5);
    std::cout.setf(std::ios::fixed);
    std::cout << "Library: desolin\t";
    std::cout << "Matrix: " << getLeaf(options.getFile()) << "\t";
    std::cout << "Matrix Size: " << num_cols(matrix) << "\t";
    std::cout << "Compiler: " << getCompiler() << "\t";
    std::cout << "Code_cache: " << getStatus(configManager.codeCachingEnabled()) << "\t";
    std::cout << "Fusion: " << getStatus(configManager.loopFusionEnabled()) << "\t";
    std::cout << "Contraction: " << getStatus(configManager.arrayContractionEnabled()) << "\t";
    std::cout << "Liveness: " << getStatus(configManager.livenessAnalysisEnabled()) << "\t";
    std::cout << "Iterations: " << iter.iterations() << "\t";
    std::cout << "Iter_time: " << elapsed / iter.iterations() << "\t";
    std::cout << "Compile_time: " << statsCollector.getCompileTime() << "\t";
    std::cout << "Compile_count: " << statsCollector.getCompileCount() << "\t";
    std::cout << "Total_time: " << elapsed << "\t";
    std::cout << "FLOPs: " << statsCollector.getFlops() << "\t";
    std::cout << "High_level_fusion: " << getStatus(configManager.highLevelFusionEnabled()) << "\t";
    std::cout << "Single_for_loop_sparse: " << getStatus(configManager.singleForLoopSparseIterationEnabled()) << "\t";
    std::cout << "Specialise_sparse: " << getStatus(configManager.sparseSpecialisationEnabled()) << std::endl;
    
    if (options.useSparse())
      std::cout << "\tNNZ: " << nnz(matrix);
      
    std::cout << std::endl;
  }

  template<typename MatrixType, typename IterationType>
  void printResults(const MatrixType& matrix, IterationType& iter, const SolverOptions& options)
  {
    if(options.singleLineResult())
    {
      printShortResults(matrix, iter, options);
    }
    else
    {
      printLongResults(matrix, iter, options);
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
      return "On";
    else
      return "Off";
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
