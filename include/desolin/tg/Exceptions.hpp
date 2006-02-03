#ifndef DESOLIN_TG_EXCEPTIONS_HPP
#define DESOLIN_TG_EXCEPTIONS_HPP

#include <desolin/Exceptions.hpp>
#include <string>

namespace desolin_internal
{
using desolin::DesoLinRuntimeError;
	
class TGInvalidOperationError : public DesoLinRuntimeError
{
public:
  TGInvalidOperationError(const std::string& error); 
};

}
#endif
