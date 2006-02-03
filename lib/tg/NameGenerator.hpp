#include <desolin/tg/NameGenerator.hpp>
#include <string>
#include <map>
#include <boost/format.hpp>

namespace desolin_internal
{

class NameGenerator
{
private:
  boost::format formatter;
  std::map<const std::string, int> nameCount;
  
public:
  NameGenerator() : formatter("%s_%i")
  {
  }
  
  std::string getName(const std::string& prefix)
  {
    return (formatter % prefix % nameCount[prefix]++).str();
  }
};

}
