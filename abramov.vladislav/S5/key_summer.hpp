#ifndef KEY_SUMMER_HPP
#define KEY_SUMMER_HPP
#include <string>
#include <utility>

namespace abramov
{
  struct KeySummer
  {
    long long int res;
    std::string val;

    void operator()(std::pair< long long int, std::string > p);
  };
}
#endif
