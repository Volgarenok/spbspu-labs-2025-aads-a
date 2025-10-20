#ifndef PAIR_HASH_HPP
#define PAIR_HASH_HPP

#include <string>
#include <boost/hash2/siphash.hpp>

namespace maslevtsov {
  struct PairStringStdHash
  {
    size_t operator()(const std::pair< std::string, std::string >& str_pair) const
    {
      size_t hash1 = std::hash< std::string >{}(str_pair.first);
      size_t hash2 = std::hash< std::string >{}(str_pair.second);
      return hash1 ^ hash2;
    }
  };

  struct PairStringSipHash
  {
    size_t operator()(const std::pair< std::string, std::string >& str_pair) const
    {
      boost::hash2::siphash_64 siphasher;
      siphasher.update(str_pair.first.data(), str_pair.first.size());
      siphasher.update(str_pair.second.data(), str_pair.second.size());
      return siphasher.result();
    }
  };
}

#endif
