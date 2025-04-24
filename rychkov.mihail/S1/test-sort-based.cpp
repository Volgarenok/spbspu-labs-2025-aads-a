#include "test_utils.hpp"

BOOST_AUTO_TEST_SUITE(S1_sort_based_test)

BOOST_AUTO_TEST_CASE(sort_test)
{
  rychkov::List< int > list = {3, 6, 4, -1, 7, 4, -3};
  list.sort();
  rychkov::fullCompare(list, {-3, -1, 3, 4, 4, 6, 7});
}
BOOST_AUTO_TEST_CASE(merge_test)
{
  rychkov::List< int > list1 = {-1, 3, 4, 6, 7};
  rychkov::List< int > list2 = {2, 3, 4, 8};
  list1.merge(list2);
  rychkov::fullCompare(list1, {-1, 2, 3, 3, 4, 4, 6, 7, 8});
  rychkov::fullCompare(list2, {});
}
BOOST_AUTO_TEST_CASE(unique_test)
{
  rychkov::List< int > list = {-1, -1, 3, 3, 3, 3, 4, 6, 6, 7, 7, 7};
  BOOST_TEST(list.unique() == 7);
  rychkov::fullCompare(list, {-1, 3, 4, 6, 7});
}

BOOST_AUTO_TEST_SUITE_END()
