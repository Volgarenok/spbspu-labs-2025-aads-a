#ifndef FUNCTIONALOBJECT_HPP
#define FUNCTIONALOBJECT_HPP
#include <string>

namespace sharifullina
{
  struct Collector
  {
    int collector;
    std::string values;
    Collector();
    void operator()(const std::pair< int, std::string >& key_value);
  };
}

#endif
