#include <boost/test/unit_test.hpp>
#include "stack.hpp"

BOOST_AUTO_TEST_CASE(test_capacity_stack)
{
  karnauhova::Stack< int > stack;
  BOOST_TEST(stack.empty() == true);
  BOOST_TEST(stack.size() == 0);
  stack.push(2);
  BOOST_TEST(stack.size() == 1);
}

BOOST_AUTO_TEST_CASE(test_pop_and_push_stack)
{
  karnauhova::Stack< int > stack;
  stack.push(1);
  BOOST_TEST(stack.top() == 1);
  stack.push(2);
  BOOST_TEST(stack.top() == 2);
  BOOST_TEST(stack.size() == 2);
  stack.pop();
  BOOST_TEST(stack.size() == 1);
  BOOST_TEST(stack.top() == 1);
  stack.pop();
  BOOST_TEST(stack.size() == 0);
}

BOOST_AUTO_TEST_CASE(test_top_stack)
{
  karnauhova::Stack< int > stack;
  stack.push(1);
  stack.push(2);
  stack.push(3);
  int i1 = stack.top();
  const int i2 = stack.top();
  BOOST_TEST(i1 == 3);
  BOOST_TEST(i2 == 3);
}
