#include <boost/test/unit_test.hpp>
#include "list.hpp"

BOOST_AUTO_TEST_SUITE(iterators_tests)
BOOST_AUTO_TEST_CASE(begin_test)
{
  shramko::FwdList< int > list;
  list.push_front(0);
  list.push_front(1);
  auto it = list.begin();
  BOOST_TEST(*it == 1);
}

BOOST_AUTO_TEST_CASE(end_test)
{
  shramko::FwdList< int > list;
  list.push_front(0);
  list.push_front(1);
  auto it = list.end();
  BOOST_TEST(*it == 0);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(element_access_tests)
BOOST_AUTO_TEST_CASE(const_ref_front_test)
{
  shramko::FwdList< int > list;
  int n = 0;
  list.push_front(n);
  list.push_front(n);
  BOOST_TEST(list.front() == n);
}

BOOST_AUTO_TEST_CASE(move_front_test)
{
  shramko::FwdList< int > list;
  list.push_front(0);
  list.push_front(1);
  BOOST_TEST(list.front() == 1);
}

BOOST_AUTO_TEST_CASE(back_test)
{
  shramko::FwdList< int > list;
  list.push_front(0);
  list.push_front(1);
  BOOST_TEST(list.back() == 0);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(capacity_tests)
BOOST_AUTO_TEST_CASE(size_test)
{
  shramko::FwdList< int > list;
  list.push_front(0);
  list.push_front(1);
  BOOST_TEST(list.size() == 2);
}

BOOST_AUTO_TEST_CASE(empty_test)
{
  shramko::FwdList< int > list;
  BOOST_TEST(list.empty() == true);
  list.push_front(0);
  BOOST_TEST(list.empty() == false);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(modifiers_tests)
BOOST_AUTO_TEST_CASE(push_front_test)
{
  shramko::FwdList< int > list;
  list.push_front(0);
  list.push_front(1);
  auto it = list.begin();
  BOOST_TEST(*it == 1);
  ++it;
  BOOST_TEST(*it == 0);
}

BOOST_AUTO_TEST_CASE(swap_test)
{
  shramko::FwdList< int > list1;
  shramko::FwdList< int > list2;
  list1.push_front(0);
  list2.push_front(1);
  list1.swap(list2);
  BOOST_TEST(list1.front() == 1);
  BOOST_TEST(list2.front() == 0);
}

BOOST_AUTO_TEST_CASE(clear_test)
{
  shramko::FwdList< int > list;
  list.clear();
  BOOST_TEST(list.empty());
  BOOST_TEST(list.size() == 0);
  list.push_front(0);
  list.clear();
  BOOST_TEST(list.empty());
  BOOST_TEST(list.size() == 0);
}
BOOST_AUTO_TEST_SUITE_END()