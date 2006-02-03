#ifndef DESOLIN_TG_PARAMETER_HOLDER_HPP
#define DESOLIN_TG_PARAMETER_HOLDER_HPP

#include <string>
#include <map>
#include <cassert>
#include <algorithm>
#include <TaskGraph>

namespace desolin_internal
{

class ParameterHolder
{
private:
  ParameterHolder(const ParameterHolder&);
  ParameterHolder& operator=(const ParameterHolder&);
  std::map<std::string, void*> parameters;
  
public:
  ParameterHolder();
  void addParameter(const std::string& name, void* value);
  static void setParameter(tg::tuTaskGraph& taskGraphObject, const std::pair<std::string, void*>& parameterMapping);
  void setParameters(tg::tuTaskGraph& taskGraphObject) const;
};

}
#endif
