#include "list-utils.hpp"
#include <limits>
#include <stdexcept>

kizhin::ForwardList< kizhin::NumbersT > kizhin::transpose(
    const ForwardList< NumbersT >& source)
{
  using NumberIter = NumbersT::const_iterator;
  ForwardList< std::pair< NumberIter, NumberIter > > iters;
  for (const auto& nums: source) {
    iters.emplaceBack(nums.begin(), nums.end());
  }
  ForwardList< NumbersT > result;
  bool isEmpty = false;
  while (!isEmpty) {
    NumbersT current;
    for (auto& pair: iters) {
      if (pair.first != pair.second) {
        current.pushBack(*pair.first);
        ++pair.first;
      }
    }
    isEmpty = current.empty();
    if (!isEmpty) {
      result.pushBack(std::move(current));
    }
  }
  return result;
}

kizhin::NumbersT::value_type kizhin::safeAccumulate(const NumbersT& nums,
    NumbersT::value_type init)
{
  constexpr auto limit = std::numeric_limits< NumbersT::value_type >::max();
  for (const auto& num: nums) {
    if (init > limit - num) {
      throw std::overflow_error("Failed to compute sum because of an overflow");
    }
    init += num;
  }
  return init;
}

