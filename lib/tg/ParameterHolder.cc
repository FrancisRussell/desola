#include <desolin/tg/ParameterHolder.hpp>
#include <string>
#include <map>
#include <cassert>
#include <algorithm>
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <TaskGraph>

namespace desolin_internal
{

ParameterHolder::ParameterHolder()
{
}

void ParameterHolder::addParameter(const std::string& name, void* value)
{
  assert(value != NULL);
  assert(parameters.find(name) == parameters.end());
  parameters[name]=value;
}

void ParameterHolder::setParameter(tg::tuTaskGraph& taskGraphObject, const std::pair<std::string, void*>& parameterMapping)
{
  taskGraphObject.setParameter(parameterMapping.first.c_str(), parameterMapping.second);
}
  
void ParameterHolder::setParameters(tg::tuTaskGraph& taskGraphObject) const
{
  std::for_each(parameters.begin(), parameters.end(), boost::bind(setParameter, boost::ref(taskGraphObject), _1));
}

}
