#include <desolin/tg/NameGenerator.hpp>
#include <string>

namespace desolin_internal
{

NameGenerator::NameGenerator() : formatter("%s_%i")
{
}
  
std::string NameGenerator::getName(const std::string& prefix)
{
  return (formatter % prefix % nameCount[prefix]++).str();
}

}
