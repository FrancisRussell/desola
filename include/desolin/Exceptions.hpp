#ifndef DESOLIN_EXCEPTIONS_HPP
#define DESOLIN_EXCEPTIONS_HPP

#include <stdexcept>
#include <string>

namespace desolin
{

class DesoLinLogicError : public std::logic_error
{
public:
  DesoLinLogicError(const std::string& message);
};


class DesoLinRuntimeError
{
public:
  DesoLinRuntimeError(const std::string& message); 
};

class NullSizeError : public DesoLinLogicError
public:
  NullSizeError(const std::string& message); 
};

}
#endif
