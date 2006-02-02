#ifndef DESOLIN_TG_PARAMETER_HOLDER_HPP
#define DESOLIN_TG_PARAMETER_HOLDER_HPP

#include <string>
#include <map>
#include <cassert>
#include <algorithm>
#include <boost/bind.hpp>
#include <boost/ref.hpp>
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
  ParameterHolder()
  {
  }

  void addParameter(const std::string& name, void* value)
  {
    assert(value != NULL);
    assert(parameters.find(name) == parameters.end());
    parameters[name]=value;
  }

  static void setParameter(tg::tuTaskGraph& taskGraphObject, const std::pair<std::string, void*>& parameterMapping)
  {
    taskGraphObject.setParameter(parameterMapping.first.c_str(), parameterMapping.second);
  }
  
  void setParameters(tg::tuTaskGraph& taskGraphObject) const
  {
    std::for_each(parameters.begin(), parameters.end(), boost::bind(setParameter, boost::ref(taskGraphObject), _1));
  }
};

}
#endif
