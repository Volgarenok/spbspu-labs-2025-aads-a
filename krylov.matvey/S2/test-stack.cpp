#include <boost/test/unit_test.hpp>
#include "stack.hpp"

using namespace krylov;

BOOST_AUTO_TEST_CASE(test_stack_push)
{
  Stack< int > stack;
  stack.push(10);
  BOOST_TEST(stack.top() == 10);
  stack.push(11);
  BOOST_TEST(stack.top() == 11);
  stack.push(12);
  BOOST_TEST(stack.top() == 12);
}

BOOST_AUTO_TEST_CASE(test_stack_pop)
{
  Stack< int > stack;
  stack.push(11);
  stack.push(12);
  stack.push(13);
  stack.pop();
  BOOST_TEST(stack.top() == 12);
  stack.pop();
  BOOST_TEST(stack.top() == 11);
  stack.pop();
  BOOST_TEST(stack.empty());
}

BOOST_AUTO_TEST_CASE(test_stack_empty)
{
  Stack< int > stack;
  BOOST_TEST(stack.empty());
  stack.push(10);
  BOOST_TEST(!stack.empty());
}

BOOST_AUTO_TEST_CASE(test_stack_size)
{
  Stack< int > stack;
  BOOST_TEST(stack.size() == 0);
  stack.push(10);
  stack.push(11);
  BOOST_TEST(stack.size() == 2);
}

