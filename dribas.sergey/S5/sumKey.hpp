#ifndef SUMKEY_HPP
#define SUMKEY_HPP

#include <string>
#include <utility>

namespace dribas
{
  struct SumKey
  {
    long long key = 0;
    std::string value;
    void operator()(const std::pair< const long long, std::string >&);
  };
}

#endif
