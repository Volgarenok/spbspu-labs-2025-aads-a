#ifndef KEYSUMMER_HPP
#define KEYSUMMER_HPP

#include <string>

namespace krylov
{
  struct KeySummator
  {
    KeySummator();
    int keySum;
    std::string values;
    void operator()(const std::pair< int, std::string >& keyValue);
  };

  KeySummator::KeySummator():
    keySum(0),
    values()
  {}

  void KeySummator::operator()(const std::pair< int, std::string > & keyValue)
  {
    keySum += keyValue.first;
    if (!values.empty())
    {
      values += " ";
    }
    values += keyValue.second;
  }
}

#endif
