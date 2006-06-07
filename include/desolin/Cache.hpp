#ifndef DESOLIN_CACHE_HPP
#define DESOLIN_CACHE_HPP

#include "ConfigurationManager.hpp"

namespace desolin_internal
{
	
class Cache
{
public:
  Cache()
  {
    ConfigurationManager::getConfigurationManager().registerCache(*this);
  }

  virtual void flush() =0;

  virtual ~Cache()
  {
    ConfigurationManager::getConfigurationManager().unregisterCache(*this);
  }
};

}
#endif
