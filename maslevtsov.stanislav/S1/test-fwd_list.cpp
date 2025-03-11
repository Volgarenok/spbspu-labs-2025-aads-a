#include <boost/test/unit_test.hpp>
#include "fwd_list.hpp"

BOOST_AUTO_TEST_SUITE(constructors_tests)
BOOST_AUTO_TEST_CASE(default_constructor_test)
{
  maslevtsov::FwdList< int > list;
  BOOST_TEST(list.empty());
}

BOOST_AUTO_TEST_CASE(copy_constructor_test)
{
  maslevtsov::FwdList< int > list1;
  maslevtsov::FwdList< int > list2(list1);
  BOOST_TEST(list2.empty());
  list1.push_front(0);
  maslevtsov::FwdList< int > list3(list1);
  BOOST_TEST(list3.front() == 0);
  list1.push_front(1);
  maslevtsov::FwdList< int > list4(list1);
  BOOST_TEST(list4.front() == 1);
  BOOST_TEST(list4.back() == 0);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(iterators_tests)
BOOST_AUTO_TEST_CASE(begin_test)
{
  maslevtsov::FwdList< int > list;
  list.push_front(0);
  list.push_front(1);
  auto it = list.begin();
  BOOST_TEST(*it == 1);
}

BOOST_AUTO_TEST_CASE(end_test)
{
  maslevtsov::FwdList< int > list;
  list.push_front(0);
  list.push_front(1);
  auto it = list.end();
  BOOST_TEST(*it == 1);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(element_access_tests)
BOOST_AUTO_TEST_CASE(front_test)
{
  maslevtsov::FwdList< int > list;
  list.push_front(0);
  list.push_front(1);
  BOOST_TEST(list.front() == 1);
}

BOOST_AUTO_TEST_CASE(back_test)
{
  maslevtsov::FwdList< int > list;
  list.push_front(0);
  list.push_front(1);
  BOOST_TEST(list.back() == 0);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(capacity_tests)
BOOST_AUTO_TEST_CASE(size_test)
{
  maslevtsov::FwdList< int > list;
  list.push_front(0);
  list.push_front(1);
  BOOST_TEST(list.size() == 2);
}

BOOST_AUTO_TEST_CASE(empty_test)
{
  maslevtsov::FwdList< int > list;
  BOOST_TEST(list.empty() == true);
  list.push_front(0);
  BOOST_TEST(list.empty() == false);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(modifiers_tests)
BOOST_AUTO_TEST_CASE(push_front_test)
{
  maslevtsov::FwdList< int > list;
  list.push_front(0);
  list.push_front(1);
  auto it = list.begin();
  BOOST_TEST(*it == 1);
  ++it;
  BOOST_TEST(*it == 0);
}

BOOST_AUTO_TEST_CASE(push_back_test)
{
  maslevtsov::FwdList< int > list;
  list.push_back(0);
  list.push_back(1);
  auto it = list.begin();
  BOOST_TEST(*it == 0);
  ++it;
  BOOST_TEST(*it == 1);
}

BOOST_AUTO_TEST_CASE(pop_front_test)
{
  maslevtsov::FwdList< int > list;
  list.push_front(0);
  list.push_front(1);
  auto it = list.begin();
  BOOST_TEST(*it == 1);
  ++it;
  BOOST_TEST(*it == 0);
}

BOOST_AUTO_TEST_CASE(swap_test)
{
  maslevtsov::FwdList< int > list1;
  maslevtsov::FwdList< int > list2;
  list1.push_front(0);
  list2.push_front(1);
  list2.push_front(2);
  list1.swap(list2);
  BOOST_TEST(list1.front() == 2);
  BOOST_TEST(list1.back() == 1);
  BOOST_TEST(list1.size() == 2);
  BOOST_TEST(list2.front() == 0);
  BOOST_TEST(list2.size() == 1);
}

BOOST_AUTO_TEST_CASE(clear_test)
{
  maslevtsov::FwdList< int > list;
  list.clear();
  BOOST_TEST(list.empty());
  BOOST_TEST(list.size() == 0);
  list.push_front(0);
  list.clear();
  BOOST_TEST(list.empty());
  BOOST_TEST(list.size() == 0);
}
BOOST_AUTO_TEST_SUITE_END()
