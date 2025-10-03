#ifndef TRAVERSE_INFO_HANDLER_HPP
#define TRAVERSE_INFO_HANDLER_HPP

#include <functional>
#include <string>
#include <queue.hpp>
#include <iostream>

namespace petrov
{
  struct SumUpKeysDisplayValues
  {
    int result = 0;
    void operator()(const std::pair< const long long, std::string > & key_value);
    std::ostream & displayValuesInTraverseOrder(std::ostream & out);
  private:
    Queue< std::string > queue_;
  };
}

#endif
