#include <boost/test/unit_test.hpp>
#include "stack.hpp"

BOOST_AUTO_TEST_SUITE(push_test)

BOOST_AUTO_TEST_CASE(lvalue_test)
{
  lanovenko::Stack< int > test{};
  int a = 7;
  test.push(a);
  BOOST_TEST(test.top() == 7);
}

BOOST_AUTO_TEST_CASE(rvalue_test)
{
  lanovenko::Stack< int > test{};
  test.push(7);
  BOOST_TEST(test.top() == 7);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_CASE(top_test)
{
  lanovenko::Stack< int > test{};
  test.push(3);
  test.push(4);
  BOOST_TEST(test.top() == 4);
}

BOOST_AUTO_TEST_CASE(empty_test_with_value)
{
  lanovenko::Stack< int > test{};
  test.push(3);
  test.push(4);
  BOOST_TEST(test.empty() == false);
}

BOOST_AUTO_TEST_CASE(empty_test_without_value)
{
  lanovenko::Stack< int > test{};
  BOOST_TEST(test.empty() == true);
}

BOOST_AUTO_TEST_CASE(swap2_test)
{
  lanovenko::Stack< int > test1{};
  for (size_t i = 0; i < 3; i++)
  {
    test1.push(i);
  }
  lanovenko::Stack< int > test2{};
  for (size_t i = 0; i < 5; i++)
  {
    test2.push(i);
  }
  test2.swap(test1);
  BOOST_TEST(test2.size() == 3);
  BOOST_TEST(test2.top() == 2);
}
