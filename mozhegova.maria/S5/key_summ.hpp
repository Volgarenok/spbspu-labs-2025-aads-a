#ifndef KEY_SUMM_HPP
#define KEY_SUMM_HPP

#include <string>

namespace mozhegova
{
  struct KeySumm
  {
    int keySum;
    std::string values;
    KeySumm();
    void operator()(const std::pair< int, std::string > & keyValue);
  };
}

#endif
