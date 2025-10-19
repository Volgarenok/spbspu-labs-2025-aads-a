#ifndef KEY_SUMMER_HPP
#define KEY_SUMMER_HPP

#include <string>
#include <utility>
#include <limits>
#include <stdexcept>

namespace guseynov
{
  struct KeySummer
  {
    long long result_;
    std::string values_;

    KeySummer();
    void operator()(const std::pair< const long long, std::string >& key_value);
  };
}

#endif
