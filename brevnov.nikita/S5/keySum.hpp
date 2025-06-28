#ifndef KEYSUM_HPP
#define KEYSUM_HPP
#include <string>
namespace brevnov
{
  struct KeySum
  {
    void operator()(const std::pair< const int, std::string >& pair);
    int sum = 0;
    std::string str;
  };
}
#endif
