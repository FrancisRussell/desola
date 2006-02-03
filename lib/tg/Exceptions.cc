#include <desolin/tg/Exceptions.hpp>
#include <string>

namespace desolin_internal
{

TGInvalidOperationError::TGInvalidOperationError(const std::string& error) : DesoLinRuntimeError(error)
{
}

}
