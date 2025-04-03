#include <boost/test/unit_test.hpp>
#include "Stack.hpp"

BOOST_AUTO_TEST_CASE(test_element_access_stack)
{
  gavrilova::Stack< int > stack{};
  stack.push(1);
  stack.push(2);
  stack.push(1 + 2);
  BOOST_TEST(stack.top() == 3);
}

BOOST_AUTO_TEST_CASE(test_constructors_stack)
{
  gavrilova::Stack< int > stack1{};
  stack1.push(1);
  stack1.push(2);
  stack1.push(1 + 2);
  gavrilova::Stack< int > stack2 = stack1;
  BOOST_TEST(stack1.top() == stack2.top());
  BOOST_TEST(stack1.size() == stack2.size());
}

BOOST_AUTO_TEST_CASE(test_assignment_operators_stack)
{
  gavrilova::Stack< int > stack1;
  stack1.push(1);
  stack1.push(2);
  stack1.push(1 + 2);
  gavrilova::Stack< int > stack2;
  stack2 = stack1;
  BOOST_TEST(stack1.top() == stack2.top());
  BOOST_TEST(stack1.size() == stack2.size());
}

BOOST_AUTO_TEST_CASE(test_pop_stack)
{
  gavrilova::Stack< int > stack;
  stack.push(1);
  stack.push(2);
  stack.push(1 + 2);
  stack.pop();
  BOOST_TEST(stack.top() == 2);
  stack.pop();
  BOOST_TEST(stack.top() == 1);
}
