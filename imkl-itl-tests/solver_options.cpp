/****************************************************************************/
/* Copyright 2005-2008, Francis Russell                                     */
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

#include "solver_options.hpp"
#include <cstdlib>
#include <string>
#include <iostream>
#include <boost/program_options.hpp>

namespace po = boost::program_options;
  
SolverOptions::SolverOptions(const std::string& fileDesc) : description("Allowed Options")
{
  description.add_options()
    ("help", "produce help message")
    ("iterations", po::value<int>(&iterations)->default_value(256), "number of iterations to perform")
    ("sparse", "use sparse representation for matrix storage")
    ("single-line-result", "print statistics on single line")
    ("format", po::value<std::string>(&format)->default_value("hb"), "format of input file: Harwell-Boeing=hb, Matrix-Market=mm")
    ("input-file", po::value<std::string>(), fileDesc.c_str());

  positional_description.add("input-file", -1);
}

void SolverOptions::processOptions(int argc, char* argv[])
{
  po::store(po::parse_command_line(argc, argv, description), vm);
  po::store(po::command_line_parser(argc, argv).options(description).
                                  positional(positional_description).run(), vm);
  po::notify(vm);

  if (vm.count("help"))
  {
    std::cout << std::endl << description << std::endl;
    exit(EXIT_SUCCESS);
  }

  if (vm.count("input-file") == 0)
  {
    std::cerr << std::endl << description << std::endl;
    std::cerr << "Please supply input file!" << std::endl << std::endl;
    exit(EXIT_FAILURE);
  }
}

std::string SolverOptions::getFile() const
{
  return vm["input-file"].as<std::string>();
}

bool SolverOptions::singleLineResult() const
{
  return vm.count("single-line-result") > 0;
}

bool SolverOptions::useSparse() const
{
  return vm.count("sparse") > 0;
}

int SolverOptions::getIterations() const
{
  return iterations;
}

bool SolverOptions::fileIsHB() const
{
  return format=="hb";
}

bool SolverOptions::fileIsMM() const
{
  return format=="mm";
}
