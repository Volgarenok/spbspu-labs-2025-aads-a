#include <algorithm>
#include "test_utils.hpp"

namespace rychkov
{
  template< class T, size_t N >
  static size_t size(const T (&array)[N]) noexcept
  {
    return N;
  }
}

BOOST_AUTO_TEST_SUITE(S1_constructors_test)

BOOST_AUTO_TEST_CASE(empty_test)
{
  rychkov::List< int > list;
  BOOST_TEST(list.size() == 0);
  BOOST_TEST(list.empty());
  BOOST_TEST((list.begin() == list.end()));
}
BOOST_AUTO_TEST_CASE(initializer_list_test)
{
  std::initializer_list< int > expected = {2, 5, 3, 9};
  rychkov::List< int > list = expected;
  BOOST_TEST(list.size() == expected.size());
  BOOST_TEST(list.front() == *expected.begin());
  BOOST_TEST(list.back() == *(expected.end() - 1));
  BOOST_TEST(std::equal(list.begin(), list.end(), expected.begin(), expected.end()));
}
BOOST_AUTO_TEST_CASE(S1_array_ptrs_test)
{
  int expected[] = {2, 5, 3, 9};
  rychkov::List< int > list(expected, expected + rychkov::size(expected));
  BOOST_TEST(list.size() == 4);
  BOOST_TEST(!list.empty());
  BOOST_TEST(list.front() == expected[0]);
  BOOST_TEST(list.back() == expected[rychkov::size(expected) - 1]);

  BOOST_TEST(std::equal(list.begin(), list.end(), expected, expected + rychkov::size(expected)));
}
BOOST_AUTO_TEST_CASE(fill_test)
{
  constexpr int initValue = 4;
  constexpr size_t initSize = 9;
  rychkov::List< int > list(initValue, initSize);
  BOOST_TEST(list.size() == initSize);
  BOOST_TEST(!list.empty());
  BOOST_TEST(list.front() == initValue);
  BOOST_TEST(list.back() == initValue);

  size_t len = 0;
  for (int i: list)
  {
    BOOST_TEST(len < initSize);
    BOOST_TEST(i == initValue);
    ++len;
  }
}
BOOST_AUTO_TEST_CASE(copy_test)
{
  rychkov::List< int > list = {2, 5, 3, 9};
  rychkov::List< int > list2 = list;
  rychkov::fullCompare(list, list2);
}
BOOST_AUTO_TEST_CASE(move_test)
{
  rychkov::List< int > list = {2, 5, 3, 9};
  rychkov::List< int > list1 = list;
  rychkov::List< int > list2 = std::move(list1);
  rychkov::fullCompare(list, list2);
  rychkov::fullCompare(list1, {});
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(S1_assignment_test)

BOOST_AUTO_TEST_CASE(copy_test)
{
  rychkov::List< int > list1 = {1, 2, 3};
  rychkov::List< int > list2;
  rychkov::fullCompare(list1, list2 = list1);
}
BOOST_AUTO_TEST_CASE(move_test)
{
  rychkov::List< int > list = {7, 3, 5};
  rychkov::List< int > list1 = list;
  rychkov::List< int > list2;
  rychkov::fullCompare(list, list2 = std::move(list1));
  rychkov::fullCompare(list1, {});
}
BOOST_AUTO_TEST_CASE(initializer_list_test)
{
  rychkov::List< int > list1 = {7, 3, 5};
  rychkov::List< int > list2;
  rychkov::fullCompare(list1, list2 = {7, 3, 5});
}

BOOST_AUTO_TEST_SUITE_END()
