#include <desolin/tg/Exceptions.hpp>

namespace desolin_internal
{

class TGInvalidOperationError : public DesoLinRuntimeError
{
public:
  TGInvalidOperationError(const std::string& error) : DesoLinRuntimeError(error) 
  {
  }
};

}
