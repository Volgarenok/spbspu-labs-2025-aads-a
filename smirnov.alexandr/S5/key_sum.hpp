#ifndef KEY_SUM_HPP
#define KEY_SUM_HPP
#include <string>

namespace smirnov
{
  struct KeySum
  {
    void operator()(const std::pair< const int, std::string > & value);
    int result = 0;
    std::string elems;
  };
}
#endif
