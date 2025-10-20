#include "test_utils.hpp"

BOOST_AUTO_TEST_SUITE(S1_constructors_test)

BOOST_AUTO_TEST_CASE(emplace_test)
{
  rychkov::List< int > list;
  BOOST_TEST(list.emplace(list.begin(), 5) == 5);
  rychkov::fullCompare(list, {5});
  BOOST_TEST(list.emplace(list.begin(), 2) == 2);
  rychkov::fullCompare(list, {2, 5});
  BOOST_TEST(list.emplace(list.end(), 6) == 6);
  rychkov::fullCompare(list, {2, 5, 6});
  BOOST_TEST(list.emplace(rychkov::shift(list.begin(), 2), 8) == 8);
  rychkov::fullCompare(list, {2, 5, 8, 6});
  BOOST_TEST(list.emplace(rychkov::shift(list.begin(), 2), 0) == 0);
  rychkov::fullCompare(list, {2, 5, 0, 8, 6});
}
BOOST_AUTO_TEST_CASE(emplace_spec_test)
{
  rychkov::List< int > list;
  BOOST_TEST(list.emplace_front(5) == 5);
  rychkov::fullCompare(list, {5});
  BOOST_TEST(list.emplace_front(2) == 2);
  rychkov::fullCompare(list, {2, 5});
  BOOST_TEST(list.emplace_back(6) == 6);
  rychkov::fullCompare(list, {2, 5, 6});
}
BOOST_AUTO_TEST_CASE(push_test)
{
  rychkov::List< int > list;
  list.push_front(5);
  rychkov::fullCompare(list, {5});
  list.push_front(2);
  rychkov::fullCompare(list, {2, 5});
  list.push_back(6);
  rychkov::fullCompare(list, {2, 5, 6});
}
BOOST_AUTO_TEST_CASE(insert_value_test)
{
  rychkov::List< int > list = {1, 2, 3};
  list.insert(list.begin(), 5);
  list.insert(list.end(), 4);
  list.insert(++list.begin(), 7);
  rychkov::fullCompare(list, {5, 7, 1, 2, 3, 4});
}
BOOST_AUTO_TEST_CASE(insert_repeated_value_test)
{
  rychkov::List< int > list = {1, 2, 3};
  list.insert(list.begin(), 2ULL, 5);
  rychkov::fullCompare(list, {5, 5, 1, 2, 3});
  list.insert(list.end(), 2ULL, 4);
  rychkov::fullCompare(list, {5, 5, 1, 2, 3, 4, 4});
  list.insert(++list.begin(), 2ULL, 7);
  rychkov::fullCompare(list, {5, 7, 7, 5, 1, 2, 3, 4, 4});
}
BOOST_AUTO_TEST_CASE(insert_initializer_list_test)
{
  rychkov::List< int > list = {1, 2, 3};
  list.insert(list.begin(), {6, 7});
  rychkov::fullCompare(list, {6, 7, 1, 2, 3});
  list.insert(list.end(), {9, 4, 0});
  rychkov::fullCompare(list, {6, 7, 1, 2, 3, 9, 4, 0});
  list.insert(++list.begin(), {1, 6});
  rychkov::fullCompare(list, {6, 1, 6, 7, 1, 2, 3, 9, 4, 0});
}

BOOST_AUTO_TEST_SUITE_END()
