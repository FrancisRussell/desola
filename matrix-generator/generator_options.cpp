#include <cstdlib>
#include <cstddef>
#include <string>
#include <iostream>
#include "generator_options.hpp"
#include <boost/program_options.hpp>

namespace po = boost::program_options;
  
GeneratorOptions::GeneratorOptions() : description("Supported Options")
{
  description.add_options()
    ("help", "produce help message")
    ("size", po::value<std::size_t>(&size)->default_value(1000), "size of matrix to generate")
    ("symmetric", "generate a symmetric matrix")
    ("output-file", po::value<std::string>(), "path to file to generate");

  positional_description.add("output-file", -1);
}

void GeneratorOptions::processOptions(int argc, char* argv[])
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

  if (vm.count("output-file") != 1)
  {
    std::cerr << std::endl << description << std::endl;
    std::cerr << "Please supply an output file!" << std::endl << std::endl;
    exit(EXIT_FAILURE);
  }
}

bool GeneratorOptions::getSymmetric() const
{
  return vm.count("symmetric") > 0;
}

std::size_t GeneratorOptions::getSize() const
{
  return size;
}

std::string GeneratorOptions::getOutputFile() const
{
  return vm["output-file"].as<std::string>();
}
