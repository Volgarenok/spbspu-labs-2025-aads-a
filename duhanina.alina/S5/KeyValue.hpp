#ifndef KEYVALUE_HPP
#define KEYVALUE_HPP

#include <string>
#include <utility>

namespace duhanina
{
  struct KeySum
  {
    long long key = 0;
    std::string value;
    void operator()(const std::pair< const long long, std::string >& key_value);
  };
}

#endif
