#include "test-utils.hpp"
#include <algorithm>
#include <iterator>
#include <boost/test/unit_test.hpp>

void kizhin::testMapInvariants(const MapT& map)
{
  BOOST_TEST(map.empty() == (map.size() == 0));
  BOOST_TEST(map.empty() == (map.begin() == map.end()));
  BOOST_TEST(std::is_sorted(map.begin(), map.end()));
  BOOST_TEST(std::distance(map.begin(), map.end()) == map.size());
}

void kizhin::testMapComparison(const MapT& lhs, const MapT& rhs)
{
  BOOST_TEST((lhs == rhs) == !(lhs != rhs));
  BOOST_TEST((lhs < rhs) == (rhs > lhs));
  BOOST_TEST((lhs < rhs) == !(lhs >= rhs));
  BOOST_TEST((lhs > rhs) == !(lhs <= rhs));
  BOOST_TEST((lhs == rhs) == (lhs <= rhs && lhs >= rhs));
}

