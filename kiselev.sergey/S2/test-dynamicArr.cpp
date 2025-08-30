#include <boost/test/tools/interface.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include "dynamicArr.hpp"

BOOST_AUTO_TEST_SUITE()

BOOST_AUTO_TEST_CASE(constructors)
{
  kiselev::DynamicArr< int > arr;
  BOOST_TEST(arr.empty());
  arr.push(1);
  kiselev::DynamicArr< int > copyArr(arr);
  BOOST_TEST(copyArr.front() == 1);
  kiselev::DynamicArr< int > moveArr(std::move(copyArr));
  BOOST_TEST(moveArr.front() == 1);
}

BOOST_AUTO_TEST_CASE(assignment_operators)
{
  kiselev::DynamicArr< int > arr;
  kiselev::DynamicArr< int > arr2;
  arr2.push(1);
  arr = arr2;
  BOOST_TEST(arr.front() == 1);
  arr.push(3);
  arr2 = arr;
  BOOST_TEST(arr2.back() == 3);
}

BOOST_AUTO_TEST_CASE(front_and_back)
{
  kiselev::DynamicArr< int > arr;
  arr.push(1);
  arr.push(5);
  const int i = arr.front();
  BOOST_TEST(i == 1);
  BOOST_TEST(arr.front() == 1);
  const int a = arr.back();
  BOOST_TEST(a == 5);
  BOOST_TEST(arr.back() == 5);
}

BOOST_AUTO_TEST_CASE(push_and_pop)
{
  kiselev::DynamicArr< int > arr;
  int i = 5;
  arr.push(i);
  BOOST_TEST(arr.front() == 5);
  arr.push(1);
  arr.push(7);
  BOOST_TEST(arr.back() == 7);
  arr.popFront();
  BOOST_TEST(arr.front() == 1);
  arr.popBack();
  BOOST_TEST(arr.back() == 1);
}

BOOST_AUTO_TEST_CASE(clear_and_size_and_empty)
{
  kiselev::DynamicArr< int > arr;
  for (int i = 0; i < 10; ++i)
  {
    arr.push(i);
  }
  BOOST_TEST(arr.size() == 10);
  arr.clear();
  BOOST_TEST(arr.empty());
}

BOOST_AUTO_TEST_SUITE_END()
