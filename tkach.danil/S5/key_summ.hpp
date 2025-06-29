#ifndef KEY_SUMM_HPP
#define KEY_SUMM_HPP

#include <string>

namespace tkach
{
  struct KeySumm
  {
    int key_sum;
    std::string values;
    KeySumm();
    void operator()(const std::pair< int, std::string >& key_value);
  };
}

#endif
