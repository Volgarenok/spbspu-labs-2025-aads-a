#include <stdexcept>
#include <iterator>
#include <boost/test/unit_test.hpp>
#include <mem_checker.hpp>
#include <unordered_map.hpp>
#include <unordered_set.hpp>

BOOST_AUTO_TEST_SUITE(S7_traverse_test)

BOOST_AUTO_TEST_CASE(set_test)
{
  rychkov::UnorderedSet< int > set = {2, 6, 7, 4, 5, 2, 3, 3};
  BOOST_TEST(set.size() == 6);
  BOOST_TEST(set.contains(2));
  BOOST_TEST(set.contains(3));
  BOOST_TEST(set.contains(4));
  BOOST_TEST(set.contains(5));
  BOOST_TEST(set.contains(6));
  BOOST_TEST(set.contains(7));
  BOOST_TEST(!set.contains(1));
  BOOST_TEST(!set.contains(8));
}
BOOST_AUTO_TEST_CASE(multiset_test)
{
  rychkov::UnorderedMultiSet< int > set = {2, 6, 7, 4, 5, 2, 3, 3, 3};
  BOOST_TEST(set.size() == 9);
  BOOST_TEST(set.count(2) == 2);
  BOOST_TEST(set.count(3) == 3);
  BOOST_TEST(set.count(4) == 1);
  BOOST_TEST(set.count(5) == 1);
  BOOST_TEST(set.count(6) == 1);
  BOOST_TEST(set.count(7) == 1);
  BOOST_TEST(set.count(1) == 0);
  BOOST_TEST(set.count(8) == 0);
  BOOST_TEST(!set.contains(1));
  BOOST_TEST(!set.contains(8));
  struct erasor
  {
    rychkov::UnorderedMultiSet< int >& set;
    size_t counts[9] = {0, 0, 2, 3, 1, 1, 1, 1, 0};
    void operator()(int num)
    {
      set.erase(set.find(num));
      counts[num]--;
      check();
    }
    void check()
    {
      size_t size = 0;
      for (int i = 0; i < 9; i++)
      {
        size += counts[i];
        BOOST_TEST(set.count(i) == counts[i]);
      }
      BOOST_TEST(set.size() == size);
      BOOST_TEST(std::distance(set.begin(), set.end()) == size);
    }
  };
  erasor erase{set};
  erase(5);
  BOOST_TEST(set.erase(3) == 3);
  erase.counts[3] = 0;
  erase.check();
  erase(2);
  erase(2);
  int rest = *set.begin();
  set.erase(++set.begin(), set.end());
  for (int i = 0; i < 9; i++)
  {
    erase.counts[i] = 0;
  }
  erase.counts[rest] = 1;
  erase.check();
}
BOOST_AUTO_TEST_CASE(map_test)
{
  rychkov::UnorderedMap< int, char > map = {{2, '1'}, {6, '2'}, {7, '3'}, {4, '4'}, {5, '5'}, {2, '6'}};
  BOOST_TEST(map.size() == 5);
  map[1] = '7';
  map[6] = '8';
  map[9] = '9';
  BOOST_TEST(map.size() == 7);
  BOOST_TEST(map.at(1) == '7');
  BOOST_TEST(map.at(2) == '1');
  BOOST_TEST(map.at(4) == '4');
  BOOST_TEST(map.at(5) == '5');
  BOOST_TEST(map.at(6) == '8');
  BOOST_TEST(map.at(7) == '3');
  BOOST_TEST(map.at(9) == '9');
  BOOST_TEST(map[7] == '3');
  BOOST_TEST(map[9] == '9');
  BOOST_CHECK_THROW(map.at(3), std::out_of_range);
  BOOST_CHECK_THROW(map.at(8), std::out_of_range);
}
BOOST_AUTO_TEST_CASE(traverse_stability_test)
{
  struct Wrapper
  {
    int value;
    Wrapper(int v):
      value(v)
    {}
    bool operator==(int rhs) const noexcept
    {
      return value == rhs;
    }
    operator int() const noexcept
    {
      return value;
    }
  };

  rychkov::MemTrack< Wrapper > observer{};
  rychkov::UnorderedSet< rychkov::MemChecker< Wrapper >, rychkov::Hash< int > > set;
  constexpr int input_size = 1000;
  for (int i = 0; i < input_size; i++)
  {
    BOOST_TEST((*set.insert(set.end(), i) == i));
    BOOST_TEST((*set.insert(set.end(), i) == i));
  }
  size_t counts[input_size]{};
  for (rychkov::MemChecker< Wrapper > i: set)
  {
    BOOST_TEST(i.value >= 0);
    BOOST_TEST(i.value < input_size);
    counts[i.value]++;
  }
  for (int i = 0; i < input_size; i++)
  {
    BOOST_TEST(counts[i] == 1);
  }
}

BOOST_AUTO_TEST_SUITE_END()
