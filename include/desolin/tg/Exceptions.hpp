#ifndef DESOLIN_TG_EXCEPTIONS_HPP
#define DESOLIN_TG_EXCEPTIONS_HPP

namespace desolin_internal
{

class TGInvalidOperationError
{
public:
  TGInvalidOperationError(const std::string& error); 
};

}
#endif
