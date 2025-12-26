#ifndef KV_AGGREGATOR_HPP
#define KV_AGGREGATOR_HPP
#include <string>

namespace sveshnikov
{
  struct KeyValAggr
  {
    int key_sum_ = 0;
    std::string values_;

    void operator()(const std::pair< int, std::string > &key_value);
  };
}

#endif
