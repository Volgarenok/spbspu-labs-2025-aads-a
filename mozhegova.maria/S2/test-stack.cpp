#include <boost/test/unit_test.hpp>
#include "stack.hpp"

BOOST_AUTO_TEST_CASE(stack_constructors_and_operators)
{
  mozhegova::Stack< int > stack1;
  stack1.push(1);
  stack1.push(2);
  mozhegova::Stack< int > stack2 = stack1;
  BOOST_TEST(stack1.top() == stack2.top());
  BOOST_TEST(stack1.size() == stack2.size());

  mozhegova::Stack< int > stack3;
  stack3 = stack1;
  BOOST_TEST(stack1.top() == stack3.top());
  BOOST_TEST(stack1.size() == stack3.size());
}

BOOST_AUTO_TEST_CASE(stack_metods)
{
  mozhegova::Stack< int > stack1;
  BOOST_TEST(stack1.empty());
  stack1.push(1);
  BOOST_TEST(stack1.top() == 1);
  stack1.push(2);
  BOOST_TEST(stack1.top() == 2);
  BOOST_TEST(!stack1.empty());
  BOOST_TEST(stack1.size() == 2);
  stack1.pop();
  BOOST_TEST(stack1.top() == 1);
  stack1.pop();
  BOOST_TEST(stack1.empty());
}
