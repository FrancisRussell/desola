#ifndef DESOLIN_EXCEPTIONS_HPP
#define DESOLIN_EXCEPTIONS_HPP

#include <stdexcept>
#include <string>

namespace desolin
{

class DesoLinLogicError : public std::logic_error
{
public:
  DesoLinLogicError(const std::string& message) : std::logic_error(message)
  {
  }
};

class DesoLinRuntimeError : public std::runtime_error
{
  public:
  DesoLinRuntimeError(const std::string& message) : std::runtime_error(message)
  {
  }
};

class NullSizeError : public DesoLinLogicError
{
public:
  NullSizeError(const std::string& message) : DesoLinLogicError(message) 
  {
  }
};

}
#endif
