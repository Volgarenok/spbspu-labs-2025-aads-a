#include <boost/test/unit_test.hpp>
#include "queue.hpp"

BOOST_AUTO_TEST_SUITE(queue_push_test)

BOOST_AUTO_TEST_CASE(queue_lvalue_test)
{
  lanovenko::Queue< int > test{};
  int a = 7;
  test.push(a);
  BOOST_TEST(test.front() == 7);
}

BOOST_AUTO_TEST_CASE(queue_rvalue_test)
{
  lanovenko::Queue< int > test{};
  test.push(7);
  BOOST_TEST(test.front() == 7);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_CASE(queue_top_test)
{
  lanovenko::Queue< int > test{};
  test.push(3);
  test.push(4);
  BOOST_TEST(test.front() == 3);
}

BOOST_AUTO_TEST_CASE(queue_empty_test_with_value)
{
  lanovenko::Queue< int > test{};
  test.push(3);
  test.push(4);
  BOOST_TEST(test.empty() == false);
}

BOOST_AUTO_TEST_CASE(queue_empty_test_without_value)
{
  lanovenko::Queue< int > test{};
  BOOST_TEST(test.empty() == true);
}

BOOST_AUTO_TEST_CASE(size_test)
{
  lanovenko::Queue< int > test{};
  test.push(2);
  test.push(3);
  BOOST_TEST(test.size() == 2);
}

BOOST_AUTO_TEST_CASE(front_test)
{
  lanovenko::Queue< int > test{};
  test.push(1);
  BOOST_TEST(test.front() == 1);
  test.pop();
  test.push(2);
  BOOST_TEST(test.front() == 2);
}

BOOST_AUTO_TEST_CASE(swap1_test)
{
  lanovenko::Queue< int > test1{};
  for (size_t i = 0; i < 3; i++)
  {
    test1.push(i);
  }
  lanovenko::Queue< int > test2{};
  for (size_t i = 0; i < 5; i++)
  {
    test2.push(i);
  }
  test2.swap(test1);
  BOOST_TEST(test2.size() == 3);
}
