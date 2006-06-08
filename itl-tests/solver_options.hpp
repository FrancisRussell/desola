#ifndef DESOLIN_ITL_SOLVER_OPTIONS
#define DESOLIN_ITL_SOLVER_OPTIONS

#include <string>
#include <iostream>
#include <boost/program_options.hpp>
#include <desolin/ConfigurationManager.hpp>

namespace po = boost::program_options;
class SolverOptions
{
private:
  po::options_description description;
  po::positional_options_description positional_description;
  po::variables_map vm;
  bool useLivenessAnalysis;
  bool useCodeCaching;
  bool useLoopFusion;
  bool useArrayContraction;
  bool useSingleLineResult;
  int iterations;
  
public:
  SolverOptions(const std::string fileDesc) : description("Allowed Options")
  {
    description.add_options()
      ("help", "produce help message")
      ("enable-gcc", "use GNU C Compiler")
      ("enable-icc", "use Intel C Compiler")
      ("iterations", po::value<int>(&iterations)->default_value(256), "number of iterations to perform")
      ("liveness-analysis", po::value<bool>(&useLivenessAnalysis)->default_value(true), "enable runtime liveness analysis")
      ("code-caching", po::value<bool>(&useCodeCaching)->default_value(true), "enable code caching and resue")
      ("loop-fusion", po::value<bool>(&useLoopFusion)->default_value(true), "enable loop fusion of runtime generated code")
      ("array-contraction", po::value<bool>(&useArrayContraction)->default_value(true), "enable array contraction on runtime generated code")
      ("single-line-result", "print statistics on single line")
      ("input-file", po::value<std::string>(), fileDesc.c_str());

    positional_description.add("input-file", -1);
  }

  void processOptions(int argc, char* argv[])
  {
    po::store(po::parse_command_line(argc, argv, description), vm);
    po::store(po::command_line_parser(argc, argv).options(description).
		                                  positional(positional_description).run(), vm);
    po::notify(vm);

    if (vm.count("help"))
    {
      std::cout << std::endl << description << std::endl;
      exit(0);
    }

    if (vm.count("input-file") == 0)
    {
      std::cerr << std::endl << description << std::endl;
      std::cerr << "Please supply input file!" << std::endl << std::endl;
      exit(1);
    }

    desolin::ConfigurationManager& configurationManager(desolin::ConfigurationManager::getConfigurationManager());
     
    if (vm.count("enable-gcc"))
    {
      configurationManager.useGCC();
    }

    if(vm.count("enable-icc"))
    {
      configurationManager.useICC();
    }

    if(useLivenessAnalysis)
    {
      configurationManager.enableLivenessAnalysis();
    }
    else
    {
      configurationManager.disableLivenessAnalysis();
    }

    if(useCodeCaching)
    {
      configurationManager.enableCodeCaching();
    }
    else
    {
      configurationManager.disableCodeCaching();
    }
	 
    if(useLoopFusion)
    {
      configurationManager.enableLoopFusion();
    }
    else
    {
      configurationManager.disableLoopFusion();
    }

    if(useArrayContraction)
    {
      configurationManager.enableArrayContraction();
    }
    else
    {
      configurationManager.disableArrayContraction();
    }
  }

  std::string getFile() const
  {
    return vm["input-file"].as<std::string>();
  }

  bool singleLineResult() const
  {
    return vm.count("single-line-result") > 0;
  }

  int getIterations() const
  {
    return iterations;
  }
};

#endif
