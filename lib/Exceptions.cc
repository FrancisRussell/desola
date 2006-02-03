#include <desolin/Exceptions.hpp>
#include <stdexcept>
#include <string>

namespace desolin
{

DesoLinLogicError::DesoLinLogicError(const std::string& message) : std::logic_error(message)
{
}

DesoLinRuntimeError::DesoLinRuntimeError(const std::string& message) : std::runtime_error(message)
{
}

NullSizeError::NullSizeError(const std::string& message) : DesoLinLogicError(message) 
{
}

}
