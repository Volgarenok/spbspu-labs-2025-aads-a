#include "double_hash.hpp"

size_t maslevtsov::StringDoubleHash::operator()(const std::string& string) const
{
  return std::hash< std::string >{}(string);
}

size_t maslevtsov::PairDoubleHash::operator()(const std::pair< std::string, std::string >& pair) const
{
  size_t hash1 = std::hash< std::string >{}(pair.first);
  size_t hash2 = std::hash< std::string >{}(pair.second);
  return hash1 ^ hash2;
}
