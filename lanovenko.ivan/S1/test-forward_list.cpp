#include <boost/test/unit_test.hpp>
#include "forward_list.hpp"

BOOST_AUTO_TEST_SUITE(iterators_test)

BOOST_AUTO_TEST_CASE(begin_test)
{
  lanovenko::ForwardList< int > testList;
  testList.push_front(1);
  testList.push_front(2);
  auto it = testList.begin();
  BOOST_TEST(*it == 2);
}

BOOST_AUTO_TEST_CASE(end_test)
{
  lanovenko::ForwardList< int > testList;
  testList.push_front(1);
  testList.push_front(2);
  auto it = testList.end();
  BOOST_TEST(*it == 2);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(front_test)

BOOST_AUTO_TEST_CASE(front_emptyList_test)
{
  lanovenko::ForwardList< int > testList;
  BOOST_CHECK_THROW(testList.front(), std::logic_error);
}

BOOST_AUTO_TEST_CASE(front_value)
{
  lanovenko::ForwardList< int > testList;
  testList.push_front(1);
  testList.push_front(2);
  int a = testList.front();
  BOOST_TEST(a == 2);
}

BOOST_AUTO_TEST_CASE(front_value_ref)
{
  lanovenko::ForwardList< int > testList;
  testList.push_front(1);
  testList.push_front(2);
  testList.front() = 4;
  BOOST_TEST(testList.front() == 4);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(back_test)

BOOST_AUTO_TEST_CASE(back_emptyList_test)
{
  lanovenko::ForwardList< int > testList;
  BOOST_CHECK_THROW(testList.back(), std::logic_error);
}

BOOST_AUTO_TEST_CASE(back_value_test)
{
  lanovenko::ForwardList< int > testList;
  testList.push_front(1);
  testList.push_front(2);
  int a = testList.back();
  BOOST_TEST(a == 1);
}

BOOST_AUTO_TEST_CASE(back_value_ref)
{
  lanovenko::ForwardList< int > testList;
  testList.push_front(1);
  testList.push_front(2);
  testList.back() = 4;
  BOOST_TEST(testList.back() == 4);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(empty_test)

BOOST_AUTO_TEST_CASE(no_data_test)
{
  lanovenko::ForwardList< int > testList;
  BOOST_TEST(testList.empty() == true);
}

BOOST_AUTO_TEST_CASE(data_test)
{
  lanovenko::ForwardList< int > testList;
  testList.push_front(1);
  BOOST_TEST(testList.empty() == false);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(size_test)

BOOST_AUTO_TEST_CASE(zero_capacity_test)
{
  lanovenko::ForwardList< int > testList;
  BOOST_TEST(testList.size() == 0);
}

BOOST_AUTO_TEST_CASE(no_zero_capacity_test)
{
  lanovenko::ForwardList< int > testList;
  testList.push_front(1);
  testList.push_front(2);
  BOOST_TEST(testList.size() == 2);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_CASE(push_front_test)
{
  lanovenko::ForwardList< int > testList;
  testList.push_front(1);
  testList.push_front(2);
  BOOST_TEST(testList.empty() == false);
  BOOST_TEST(testList.size() == 2);
  BOOST_TEST(testList.front() == 2);
}

BOOST_AUTO_TEST_CASE(pop_front_test)
{
  lanovenko::ForwardList< int > testList;
  testList.push_front(1);
  testList.push_front(2);
  testList.pop_front();
  testList.pop_front();
  BOOST_TEST(testList.empty() == true);
}

BOOST_AUTO_TEST_CASE(swap_test)
{
  lanovenko::ForwardList< int > testList1;
  lanovenko::ForwardList< int > testList2;
  testList1.push_front(3);
  testList2.push_front(2);
  testList2.push_front(1);
  testList1.swap(testList2);
  BOOST_TEST(testList1.front() == 1);
  BOOST_TEST(testList2.front() == 3);
  BOOST_TEST(testList1.size() == 2);
}

BOOST_AUTO_TEST_CASE(clear_test)
{
   lanovenko::ForwardList< int > testList;
   testList.clear();
   BOOST_TEST(testList.empty() == true);
   testList.push_front(2);
   testList.push_front(3);
   testList.clear();
   BOOST_TEST(testList.empty() == true);
}

BOOST_AUTO_TEST_SUITE(push_back_test)

BOOST_AUTO_TEST_CASE(to_empty_list_test)
{
  lanovenko::ForwardList< int > testList;
  testList.push_back(1);
  BOOST_TEST((*testList.begin()) == (*testList.end() == 1));
  BOOST_TEST(testList.size() == 1);
}

BOOST_AUTO_TEST_CASE(to_not_empty_list_test)
{
  lanovenko::ForwardList< int > testList;
  testList.push_back(1);
  testList.push_back(2);
  BOOST_TEST(testList.size() == 2);
}

BOOST_AUTO_TEST_SUITE_END()
