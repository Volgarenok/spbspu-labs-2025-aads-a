#ifndef FUNCTIONALOBJECT_HPP
#define FUNCTIONALOBJECT_HPP
#include <string>

namespace finaev
{
  struct keySum
  {
    int key_sum;
    std::string values;
    keySum();
    void operator()(const std::pair< int, std::string >& key_value);
  };
}

#endif
