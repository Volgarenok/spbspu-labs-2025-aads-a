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

BOOST_AUTO_TEST_CASE(test_constructors_and_operator_stack)
{
  karnauhova::Stack< int > stack;
  stack.push(2);

  karnauhova::Stack< int > copystack(stack);
  BOOST_TEST(copystack.size() == 1);
  BOOST_TEST(copystack.top() == 2);

  karnauhova::Stack< int > movestack(std::move(stack));
  BOOST_TEST(movestack.size() == 1);
  BOOST_TEST(movestack.top() == 2);
  BOOST_TEST(stack.empty());

  karnauhova::Stack< int > another_stack;
  another_stack.push(3);
  another_stack.push(4);
  copystack = another_stack;
  BOOST_TEST(copystack.size() == 2);
  BOOST_TEST(copystack.top() == 4);
  BOOST_TEST(another_stack.size() == 2);
  another_stack = another_stack;
  BOOST_TEST(another_stack.size() == 2);
  BOOST_TEST(another_stack.top() == 4);
}

BOOST_AUTO_TEST_CASE(test_swap_stack)
{
  karnauhova::Stack< int > stack1;
  stack1.push(1);
  stack1.push(2);
  stack1.push(3);
  karnauhova::Stack< int > stack2;
  stack2.push(1);
  stack2.push(2);
  stack1.swap(stack2);
  BOOST_TEST(stack1.size() == 2);
  BOOST_TEST(stack1.top() == 2);
  BOOST_TEST(stack2.size() == 3);
  BOOST_TEST(stack2.top() == 3);
}
