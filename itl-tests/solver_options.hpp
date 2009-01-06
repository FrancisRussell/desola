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

#ifndef DESOLIN_DESOLIN_SOLVER_OPTIONS
#define DESOLIN_DESOLIN_SOLVER_OPTIONS

#include <string>
#include <boost/program_options.hpp>

namespace po = boost::program_options;
class SolverOptions
{
private:
  po::options_description description;
  po::positional_options_description positional_description;
  po::variables_map vm;
  std::string format;
  bool useLivenessAnalysis;
  bool useCodeCaching;
  bool useLoopFusion;
  bool useHighLevelFusion;
  bool useArrayContraction;
  bool useSingleLineResult;
  bool useSingleForLoopSparse;
  bool useSparseSpecialisation;
  int iterations;
  
public:
  SolverOptions(const std::string& fileDesc);
  void processOptions(int argc, char* argv[]);
  std::string getFile() const;
  bool singleLineResult() const;
  bool useSparse() const;
  bool fileIsHB() const;
  bool fileIsMM() const;
  int getIterations() const;
};

#endif
