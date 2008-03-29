#ifndef DESOLIN_TG_TASKGRAPHWRAPPERS_HPP
#define DESOLIN_TG_TASKGRAPHWRAPPERS_HPP

#include <string>
#include <cassert>
#include <algorithm>
#include <boost/scoped_ptr.hpp>
#include <TaskGraph>

namespace desolin
{

namespace detail
{

template<typename T>
class TaskScalarVariableWrapper
{
private:
  const bool param;
  const std::string name;
  mutable boost::scoped_ptr<tg::TaskScalarVariable> var;

  tg::TaskScalarVariable construct() const
  {
    if (param)
    {
      tParameter(tVarTemplateTypeNamed(T, val, name.c_str()));
      return val;
    }
    else
    {
      tVarTemplateTypeNamed(T, val, name.c_str());
      return val;
    }
  }

public:
  TaskScalarVariableWrapper(const bool _param, const std::string& _name) : param(_param), name(_name), var(NULL)
  {
  }

  void instantiate() const
  {
    if (var.get() == NULL)
    {
      boost::scoped_ptr<tg::TaskScalarVariable> constructed(new tg::TaskScalarVariable(construct()));
      var.swap(constructed);
    }
  }

  tg::TaskScalarVariable& operator*()
  {
    assert(var.get() != NULL);
    return *var;
  }

  const tg::TaskScalarVariable& operator*() const
  {
    assert(var.get() != NULL);
    return *var;
  }
};

template<typename T, unsigned dimensions>
class TaskArrayWrapper
{
private:
  const bool param;
  const std::string name;
  unsigned dims[dimensions];
  mutable boost::scoped_ptr< tg::TaskArray<dimensions> > var;

  tg::TaskArray<dimensions> construct() const
  {
    unsigned mutableDims[dimensions];
    std::copy(&dims[0], &dims[dimensions], &mutableDims[0]);

    if (param)
    {
      tParameter(tArrayFromListNamed(T, val, dimensions, mutableDims, name.c_str()));
      return val;
    }
    else
    {
      tArrayFromListNamed(T, val, dimensions, mutableDims, name.c_str());
      return val;
    }
  }

public:
  TaskArrayWrapper(const bool _param, const std::string& _name, const unsigned _dims[dimensions]) : param(_param),
                name(_name), dims(_dims), var(NULL)
  {
  }

  TaskArrayWrapper(const bool _param, const std::string& _name, const unsigned dim0) : param(_param),
                name(_name)
  {
    assert(dimensions == 1);
    dims[0] = dim0;
  }

  TaskArrayWrapper(const bool _param, const std::string& _name, const unsigned dim0, const unsigned dim1) : param(_param),
                name(_name)
  {
    assert(dimensions == 2);
    dims[0] = dim0;
    dims[1] = dim1;
  }

  void instantiate() const
  {
    if (var.get() == NULL)
    {
      boost::scoped_ptr< tg::TaskArray<dimensions> > constructed(new tg::TaskArray<dimensions>(construct()));
      var.swap(constructed);
    }
  }

  tg::TaskArray<dimensions>& operator*()
  {
    assert(var.get() != NULL);
    return *var;
  }

  const tg::TaskArray<dimensions>& operator*() const
  {
    assert(var.get() != NULL);
    return *var;
  }
};

}

}

#endif
