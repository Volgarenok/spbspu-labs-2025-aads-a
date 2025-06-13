#ifndef DOUBLE_HASH_HPP
#define DOUBLE_HASH_HPP

#include <string>

namespace maslevtsov {
  struct StringDoubleHash
  {
    size_t operator()(const std::string& string) const;
  };

  struct PairDoubleHash
  {
    size_t operator()(const std::pair< std::string, std::string >& pair) const;
  };
}

#endif
