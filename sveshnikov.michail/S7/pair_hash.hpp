#include <functional>
#include <cstddef>
#include <string>

namespace sveshnikov
{
  struct PairHash
  {
    size_t operator()(const std::pair< std::string, std::string > &p) const noexcept
    {
      return std::hash< std::string >{}(p.first) ^ std::hash< std::string >{}(p.second);
    }
  };
}
