#include <stdexcept>
#include <iostream>
#include <random>
#include <chrono>
#include <algorithm>
#include <iterator>
#include <boost/test/unit_test.hpp>
#include <mem_checker.hpp>
#include "map.hpp"
#include "set.hpp"

BOOST_AUTO_TEST_SUITE(S4_map_test)

BOOST_AUTO_TEST_CASE(multimap_test)
{
  rychkov::MultiMap< int, char > map = {{0, '1'}, {0, '2'}, {0, '3'}, {1, '3'}, {1, '4'}, {1, '4'}, {2, '5'}};
  BOOST_TEST(map.size() == 7);
  BOOST_TEST((map.lower_bound(0) == map.begin()));
  BOOST_TEST(std::distance(map.upper_bound(0), --map.end()) == 3);
  BOOST_TEST((map.upper_bound(1) == --map.end()));
}
BOOST_AUTO_TEST_CASE(empty_test)
{
  rychkov::Map< int, char > map;
  BOOST_TEST(map.empty());
  BOOST_TEST((map.find(234) == map.end()));
  BOOST_TEST(!map.contains(93));
  BOOST_TEST((map.begin() == map.end()));
  BOOST_CHECK_THROW(map.at(0), std::out_of_range);
}
BOOST_AUTO_TEST_CASE(map_test)
{
  rychkov::Map< int, char > map = {{0, '1'}, {3, '2'}, {-1, '3'}, {2, '4'}, {3, '5'}, {9, '6'}};
  BOOST_TEST(map.size() == 5);
  BOOST_TEST((map.find(234) == map.end()));
  BOOST_TEST((map.find(-1) == map.begin()));
  BOOST_TEST(!map.contains(-44));
  BOOST_TEST(map.contains(3));
  BOOST_TEST((map.begin() != map.end()));
  BOOST_CHECK_THROW(map.at(7), std::out_of_range);
  BOOST_CHECK_THROW(map.at(1), std::out_of_range);
  BOOST_TEST(map.at(3) == '2');
  BOOST_TEST(map.lower_bound(2)->first == 2);
  BOOST_TEST(map.upper_bound(2)->first == 3);
  map[9] = '5';
  BOOST_TEST(map[9] == '5');
  map[1] = '6';
  BOOST_TEST(map[1] == '6');
  BOOST_TEST(map[3] == '2');
}
BOOST_AUTO_TEST_CASE(erase_test)
{
  rychkov::Set< int > set = {0, 2, 1, 5, -3, 4, -2};
  BOOST_TEST(*set.erase(set.find(0)) == 1);
  BOOST_TEST(set.erase(2) == 1);
  BOOST_TEST(set.erase(-1) == 0);
  BOOST_TEST(set.erase(4) == 1);
  BOOST_TEST(set.erase(1) == 1);
  BOOST_TEST((set.erase(set.begin(), set.end()) == set.end()));
  BOOST_TEST(set.empty());
}
BOOST_AUTO_TEST_CASE(random_test)
{
  struct Wrapper
  {
    std::mt19937::result_type value;
    bool operator==(Wrapper rhs) const noexcept
    {
      return value == rhs.value;
    }
    bool operator<(Wrapper rhs) const noexcept
    {
      return value < rhs.value;
    }
  };
  rychkov::MemTrack< Wrapper > observer{};
  std::mt19937 engine;
  engine.seed(std::chrono::system_clock::now().time_since_epoch().count());
  constexpr size_t input_size = 8192;
  Wrapper data[input_size];
  rychkov::MultiSet< rychkov::MemChecker< Wrapper >, std::less<>, 10 > set;
  size_t size = 0;
  for (Wrapper& i: data)
  {
    std::uniform_int_distribution< size_t > range(0, size - 1);
    decltype(set)::iterator wrong_hint = (size == 0 ? set.begin() : set.lower_bound(data[range(engine)]));
    i = {engine()};
    set.insert(wrong_hint, i);
  }
  std::sort(data, data + input_size);
  size = input_size;
  BOOST_TEST(std::equal(set.begin(), set.end(), data));
  while (size > 0)
  {
    std::uniform_int_distribution< size_t > range(0, size - 1);
    size_t id = range(engine);
    set.erase(data[id]);
    size = std::remove(data, data + size, data[id]) - data;
    BOOST_TEST(std::equal(set.begin(), set.end(), data));
    BOOST_TEST(set.size() == size);
  }
}

BOOST_AUTO_TEST_SUITE_END()
