#ifndef DESOLIN_TG_NAME_GENERATOR_HPP
#define DESOLIN_TG_NAME_GENERATOR_HPP

#include <string>
#include <map>

namespace desolin_internal
{

class NameGenerator
{
private:
  boost::format formatter;
  std::map<const std::string, int> nameCount;
  
public:
  NameGenerator(); 
  std::string getName(const std::string& prefix);
};

}
#endif
