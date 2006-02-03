#ifndef DESOLIN_TG_PARAMETER_HOLDER_HPP
#define DESOLIN_TG_PARAMETER_HOLDER_HPP

#include <string>
#include <map>
#include <TaskGraph>

namespace desolin_internal
{

class ParameterHolder
{
private:
  ParameterHolder(const ParameterHolder&);
  ParameterHolder& operator=(const ParameterHolder&);
  std::map<std::string, void*> parameters;
 
  static void setParameter(tg::tuTaskGraph& taskGraphObject, const std::pair<std::string, void*>& parameterMapping);

public:
  ParameterHolder();
  void addParameter(const std::string& name, void* value);
  void setParameters(tg::tuTaskGraph& taskGraphObject) const;
};

}
#endif
