#ifndef DESOLIN_MATRIX_GENERATOR_GENERATOR_OPTIONS_HPP
#define DESOLIN_MATRIX_GENERATOR_GENERATOR_OPTIONS_HPP

#include <boost/program_options.hpp>
#include <string>

namespace po = boost::program_options;

class GeneratorOptions
{
private:
  po::options_description description;
  po::positional_options_description positional_description;
  po::variables_map vm;
  int size;

public:
  GeneratorOptions();
  void processOptions(int argc, char* argv[]);
  bool getSymmetric() const;
  int getSize() const;
  std::string getOutputFile() const;
};

#endif