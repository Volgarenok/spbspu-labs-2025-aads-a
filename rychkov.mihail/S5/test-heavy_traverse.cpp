#include <algorithm>
#include <boost/test/unit_test.hpp>
#include <set.hpp>
#include <map.hpp>

BOOST_AUTO_TEST_SUITE(S5_traverse_test)

BOOST_AUTO_TEST_CASE(iterator_test)
{
  rychkov::Set< int > set = {2, 5, 3, 0, 7, 9, 6, 10};
  using heavy = decltype(set)::heavy_iterator;
  using rheavy = decltype(set)::reverse_heavy_iterator;
  BOOST_TEST(std::equal(set.begin(), set.end(), heavy{set.begin()}));
  BOOST_TEST(std::equal(set.rbegin(), set.rend(), rheavy{set.rbegin()}));
  BOOST_TEST(std::equal(heavy{set.begin()}, heavy{set.end()}, set.begin()));
  BOOST_TEST(std::equal(rheavy{set.rbegin()}, rheavy{set.rend()}, set.rbegin()));

  using cheavy = decltype(set)::const_heavy_iterator;
  using crheavy = decltype(set)::const_reverse_heavy_iterator;
  BOOST_TEST(std::equal(set.cbegin(), set.cend(), cheavy{set.cbegin()}));
  BOOST_TEST(std::equal(set.crbegin(), set.crend(), crheavy{set.crbegin()}));
  BOOST_TEST(std::equal(cheavy{set.cbegin()}, cheavy{set.cend()}, set.cbegin()));
  BOOST_TEST(std::equal(crheavy{set.crbegin()}, crheavy{set.crend()}, set.crbegin()));

  int breadth_data[] = {3, 7, 0, 2, 5, 6, 9, 10};
  int rbreadth_data[] = {7, 3, 10, 9, 6, 5, 2, 0};
  using breadth = decltype(set)::breadth_iterator;
  using cbreadth = decltype(set)::const_breadth_iterator;
  BOOST_TEST(std::equal(breadth{set.end()}, breadth{}, breadth_data));
  BOOST_TEST(std::equal(cbreadth{set.cend()}, cbreadth{}, breadth_data));
  BOOST_TEST(std::equal(breadth_data, breadth_data + set.size(), breadth{set.end()}));
  BOOST_TEST(std::equal(breadth_data, breadth_data + set.size(), cbreadth{set.cend()}));

  BOOST_TEST(std::equal(breadth{set.begin()}, breadth{}, breadth_data + 2));
  BOOST_TEST(std::equal(cbreadth{set.begin()}, cbreadth{}, breadth_data + 2));
  BOOST_TEST(std::equal(breadth_data + 2, breadth_data + set.size(), breadth{set.begin()}));
  BOOST_TEST(std::equal(breadth_data + 2, breadth_data + set.size(), cbreadth{set.cbegin()}));

  using rbreadth = decltype(set)::reverse_breadth_iterator;
  using crbreadth = decltype(set)::const_reverse_breadth_iterator;
  BOOST_TEST(std::equal(rbreadth{set.rend()}, rbreadth{}, rbreadth_data));
  BOOST_TEST(std::equal(crbreadth{set.crend()}, crbreadth{}, rbreadth_data));
  BOOST_TEST(std::equal(rbreadth_data, rbreadth_data + set.size(), rbreadth{set.rend()}));
  BOOST_TEST(std::equal(rbreadth_data, rbreadth_data + set.size(), crbreadth{set.crend()}));

  BOOST_TEST(std::equal(rbreadth{set.rbegin()}, rbreadth{}, rbreadth_data + 2));
  BOOST_TEST(std::equal(crbreadth{set.rbegin()}, crbreadth{}, rbreadth_data + 2));
  BOOST_TEST(std::equal(rbreadth_data + 2, rbreadth_data + set.size(), rbreadth{set.rbegin()}));
  BOOST_TEST(std::equal(rbreadth_data + 2, rbreadth_data + set.size(), crbreadth{set.crbegin()}));

  BOOST_TEST((decltype(set)::iterator{heavy{set.begin()}} == set.begin()));
  BOOST_TEST((decltype(set)::reverse_iterator{rheavy{set.rbegin()}} == set.rbegin()));
  BOOST_TEST((decltype(set)::iterator{breadth{set.begin()}} == set.begin()));
  BOOST_TEST((decltype(set)::reverse_iterator{rbreadth{set.rbegin()}} == set.rbegin()));
}

BOOST_AUTO_TEST_SUITE_END()
