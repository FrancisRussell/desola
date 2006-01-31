#ifndef DESOLIN_TG_EXCEPTIONS_HPP
#define DESOLIN_TG_EXCEPTIONS_HPP

class TGInvalidOperationError : public DesoLinRuntimeError
{
public:
  TGInvalidOperationError(const std::string& error) : DesoLinRuntimeError(error) 
  {
  }
};

#endif
