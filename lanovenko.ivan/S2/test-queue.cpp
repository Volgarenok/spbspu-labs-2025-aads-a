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
