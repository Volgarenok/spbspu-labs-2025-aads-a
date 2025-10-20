#include "array.hpp"
#include <hash_table/hashTable.hpp>

namespace smirnov
{
  using Dict = HashTable< std::string, Array >;
  using Dicts = HashTable< std::string, Dict >;
}
