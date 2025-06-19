#include <boost/test/unit_test.hpp>
#include <utils/stack.hpp>

BOOST_AUTO_TEST_CASE(stack_empty_after_creation)
{
  smirnov::Stack< int > s;
  BOOST_TEST(s.empty());
}

BOOST_AUTO_TEST_CASE(stack_not_empty_after_push)
{
  smirnov::Stack< int > s;
  s.push(1);
  BOOST_TEST(!s.empty());
}

BOOST_AUTO_TEST_CASE(stack_top_returns_last_element)
{
  smirnov::Stack< int > s;
  s.push(1);
  s.push(2);
  BOOST_TEST(s.top() == 2);
}

BOOST_AUTO_TEST_CASE(stack_drop_removes_last_element)
{
  smirnov::Stack< int > s;
  s.push(1);
  s.push(2);
  BOOST_TEST(s.drop() == 2);
  BOOST_TEST(s.top() == 1);
}

BOOST_AUTO_TEST_CASE(stack_maintains_lifo_order)
{
  smirnov::Stack< int > s;
  s.push(1);
  s.push(2);
  s.push(3);
  BOOST_TEST(s.drop() == 3);
  BOOST_TEST(s.drop() == 2);
  BOOST_TEST(s.drop() == 1);
  BOOST_TEST(s.empty());
}

BOOST_AUTO_TEST_CASE(stack_size_changes_after_operations)
{
  smirnov::Stack< int > s;
  BOOST_TEST(s.empty());
  s.push(1);
  BOOST_TEST(!s.empty());
  s.push(2);
  BOOST_TEST(!s.empty());
  s.drop();
  BOOST_TEST(!s.empty());
  s.drop();
  BOOST_TEST(s.empty());
}
