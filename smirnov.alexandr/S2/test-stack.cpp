#include <boost/test/unit_test.hpp>
#include <utils/stack.hpp>

BOOST_AUTO_TEST_CASE(stack_empty_after_creation)
{
  smirnov::Stack< int > s;
  BOOST_TEST(s.empty());
  BOOST_TEST(s.size() == 0u);
}

BOOST_AUTO_TEST_CASE(stack_not_empty_after_push)
{
  smirnov::Stack< int > s;
  s.push(1);
  BOOST_TEST(!s.empty());
  BOOST_TEST(s.size() == 1u);
}

BOOST_AUTO_TEST_CASE(stack_top_returns_last_element)
{
  smirnov::Stack< int > s;
  s.push(1);
  s.push(2);
  int last = s.top();
  BOOST_TEST(last == 2);
  BOOST_TEST(s.size() == 2u);
}

BOOST_AUTO_TEST_CASE(stack_pop_removes_last_element)
{
  smirnov::Stack< int > s;
  s.push(1);
  s.push(2);
  int last = s.top();
  BOOST_TEST(last == 2);
  s.pop();
  BOOST_TEST(s.top() == 1);
  BOOST_TEST(s.size() == 1u);
}

BOOST_AUTO_TEST_CASE(stack_maintains_lifo_order)
{
  smirnov::Stack< int > s;
  s.push(1);
  s.push(2);
  s.push(3);
  int a = s.top();
  s.pop();
  int b = s.top();
  s.pop();
  int c = s.top();
  s.pop();
  BOOST_TEST(a == 3);
  BOOST_TEST(b == 2);
  BOOST_TEST(c == 1);
  BOOST_TEST(s.empty());
  BOOST_TEST(s.size() == 0u);
}

BOOST_AUTO_TEST_CASE(stack_size_changes_after_operations)
{
  smirnov::Stack< int > s;
  BOOST_TEST(s.empty());
  s.push(1);
  BOOST_TEST(s.size() == 1u);
  s.push(2);
  BOOST_TEST(s.size() == 2u);
  s.pop();
  BOOST_TEST(s.size() == 1u);
  s.pop();
  BOOST_TEST(s.empty());
  BOOST_TEST(s.size() == 0u);
}
