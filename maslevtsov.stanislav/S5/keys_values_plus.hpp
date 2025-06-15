#ifndef KEYS_VALUES_PLUS_HPP
#define KEYS_VALUES_PLUS_HPP

#include <string>

namespace maslevtsov {
  struct KeysValuesPlus
  {
    int keys_sum = 0;
    std::string values = "";

    void operator()(const std::pair< int, std::string >& value);
  };
}

#endif
