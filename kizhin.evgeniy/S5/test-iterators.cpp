#include <algorithm>
#include <iterator>
#include <numeric>
#include <vector>
#include <boost/test/unit_test.hpp>
#include <map.hpp>

template < typename Iter >
std::vector< int > extract_keys(Iter begin, const Iter end)
{
  std::vector< int > keys;
  for (; begin != end; ++begin) {
    keys.push_back(begin->first);
  }
  return keys;
}

struct MapFixture
{
  using MapT = kizhin::Map< int, int >;
  MapT map;
  std::vector< MapT::key_type > mapKeys;
  const std::vector< int > bfsExpected;
  MapFixture():
    map(),
    mapKeys(15),
    bfsExpected({ 7, 3, 11, 1, 5, 9, 13, 0, 2, 4, 6, 8, 10, 12, 14 })
  {
    std::iota(mapKeys.begin(), mapKeys.end(), 0);
    for (auto key: mapKeys) {
      map.emplace(key, 0);
    }
  }
};

BOOST_FIXTURE_TEST_SUITE(map_iterator_tests, MapFixture)

BOOST_AUTO_TEST_CASE(iterator)
{
  auto keys = extract_keys(map.begin(), map.end());
  BOOST_TEST(keys == mapKeys);
  BOOST_TEST(keys.size() == map.size());
  BOOST_TEST(std::is_sorted(keys.begin(), keys.end()));
}

BOOST_AUTO_TEST_CASE(lmr_iterator)
{
  auto keys = extract_keys(map.lmrBegin(), map.lmrEnd());
  BOOST_TEST(keys == mapKeys);
  BOOST_TEST(keys.size() == map.size());
  BOOST_TEST(std::is_sorted(keys.begin(), keys.end()));
}

BOOST_AUTO_TEST_CASE(rml_iterator)
{
  auto keys = extract_keys(map.rmlBegin(), map.rmlEnd());
  std::reverse(keys.begin(), keys.end());
  BOOST_TEST(keys == mapKeys);
  BOOST_TEST(keys.size() == map.size());
  BOOST_TEST(std::is_sorted(keys.begin(), keys.end()));
}

BOOST_AUTO_TEST_CASE(bfs_iterator)
{
  auto keys = extract_keys(map.bfsBegin(), map.bfsEnd());
  BOOST_TEST(keys == bfsExpected);
}

BOOST_AUTO_TEST_CASE(iterator_to_lmr)
{
  auto keys = extract_keys(MapT::lmr_iterator(map.begin()), map.lmrEnd());
  BOOST_TEST(keys == mapKeys);
  BOOST_TEST(keys.size() == map.size());
  BOOST_TEST(std::is_sorted(keys.begin(), keys.end()));
}

BOOST_AUTO_TEST_CASE(iterator_to_rml)
{
  auto keys = extract_keys(MapT::rml_iterator(map.begin()), map.rmlEnd());
  std::reverse(keys.begin(), keys.end());
  BOOST_TEST(keys == mapKeys);
  BOOST_TEST(keys.size() == map.size());
  BOOST_TEST(std::is_sorted(keys.begin(), keys.end()));
}

BOOST_AUTO_TEST_CASE(iterator_to_bfs)
{
  auto keys = extract_keys(MapT::bfs_iterator(map.begin()), map.bfsEnd());
  BOOST_TEST(keys == bfsExpected);
}

BOOST_AUTO_TEST_CASE(lmr_to_iterator)
{
  auto keys = extract_keys(MapT::iterator(map.lmrBegin()), map.end());
  BOOST_TEST(keys == mapKeys);
  BOOST_TEST(keys.size() == map.size());
  BOOST_TEST(std::is_sorted(keys.begin(), keys.end()));
}

BOOST_AUTO_TEST_CASE(rml_to_iterator)
{
  auto begin = std::make_reverse_iterator(MapT::iterator(map.rmlBegin()));
  auto end = std::make_reverse_iterator(map.begin());
  auto keys = extract_keys(begin, end);
  std::reverse(keys.begin(), keys.end());
  keys.push_back(MapT::iterator(map.rmlBegin())->first);
  BOOST_TEST(keys == mapKeys);
  BOOST_TEST(keys.size() == map.size());
  BOOST_TEST(std::is_sorted(keys.begin(), keys.end()));
}

BOOST_AUTO_TEST_CASE(bfs_to_iterator)
{
  const std::vector< int > expected = { 7, 8, 9, 10, 11, 12, 13, 14 };
  auto keys = extract_keys(MapT::iterator(map.bfsBegin()), map.end());
  BOOST_TEST(keys == expected);
  BOOST_TEST(std::is_sorted(keys.begin(), keys.end()));
}

BOOST_AUTO_TEST_SUITE_END()

