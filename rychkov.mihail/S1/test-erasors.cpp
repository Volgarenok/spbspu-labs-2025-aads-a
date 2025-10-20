#include <functional.hpp>
#include "test_utils.hpp"

BOOST_AUTO_TEST_SUITE(S1_erasors_test)

BOOST_AUTO_TEST_CASE(erase_single_test)
{
  rychkov::List< int > list = {3, 6, 4, -1, 7};
  BOOST_TEST(*list.erase(std::next(list.begin())) == 4);
  rychkov::fullCompare(list, {3, 4, -1, 7});
  BOOST_TEST((list.erase(std::prev(list.end())) == list.end()));
  rychkov::fullCompare(list, {3, 4, -1});
  BOOST_TEST(*list.erase(list.begin()) == 4);
  rychkov::fullCompare(list, {4, -1});
}
BOOST_AUTO_TEST_CASE(erase_multiple_test)
{
  rychkov::List< int > list = {3, 6, 4, -1, 7};
  BOOST_TEST(*list.erase(list.begin(), rychkov::shift(list.begin(), 2)) == 4);
  rychkov::fullCompare(list, {4, -1, 7});
}
BOOST_AUTO_TEST_CASE(pop_test)
{
  rychkov::List< int > list = {3, 6, 7};
  list.pop_back();
  rychkov::fullCompare(list, {3, 6});
  list.pop_front();
  rychkov::fullCompare(list, {6});
  list.pop_back();
  rychkov::fullCompare(list, {});
}
BOOST_AUTO_TEST_CASE(remove_test)
{
  rychkov::List< int > list = {3, 3, 4, -1, 3, 7, 3};
  BOOST_TEST(list.remove(3) == 4);
  rychkov::fullCompare(list, {4, -1, 7});
}
BOOST_AUTO_TEST_CASE(remove_if_test)
{
  rychkov::List< int > list = {3, 3, 4, -1, 3, 7, 3, -3, 5};
  rychkov::compare_to_value< std::greater_equal<>, int > comparator = {std::greater_equal<>(), 4};
  BOOST_TEST(list.remove_if(comparator) == 3);
  rychkov::fullCompare(list, {3, 3, -1, 3, 3, -3});
}

BOOST_AUTO_TEST_CASE(splice_all_lvalue_test)
{
  rychkov::List< int > list1 = {3, 7, 1, 0};
  rychkov::List< int > list2;
  list2.splice(list2.begin(), list1);
  rychkov::fullCompare(list1, {});
  rychkov::fullCompare(list2, {3, 7, 1, 0});
}
BOOST_AUTO_TEST_CASE(splice_all_rvalue_test)
{
  rychkov::List< int > list;
  list.splice(list.begin(), {3, 7, 1, 0});
  rychkov::fullCompare(list, {3, 7, 1, 0});
}
BOOST_AUTO_TEST_CASE(splice_part_empty_test)
{
  rychkov::List< int > list1 = {3, 7, 1, 0};
  rychkov::List< int > list2;
  list2.splice(list2.begin(), list1, list1.begin(), rychkov::shift(list1.begin(), 2));
  rychkov::fullCompare(list1, {1, 0});
  rychkov::fullCompare(list2, {3, 7});
}
BOOST_AUTO_TEST_CASE(splice_part_inner_test)
{
  rychkov::List< int > list1 = {3, 7, 1, 0};
  rychkov::List< int > list2 = {2, 4};
  list2.splice(++list2.begin(), list1, list1.begin(), rychkov::shift(list1.begin(), 2));
  rychkov::fullCompare(list1, {1, 0});
  rychkov::fullCompare(list2, {2, 3, 7, 4});
}

BOOST_AUTO_TEST_SUITE_END()
