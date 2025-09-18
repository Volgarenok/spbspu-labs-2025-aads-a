#include <boost/test/unit_test.hpp>
#include "stack.hpp"

BOOST_AUTO_TEST_CASE(copy_constructor_stack)
{
  abramov::Stack< int > stack1;
  stack1.push(1);
  stack1.push(2);
  stack1.push(3);
  abramov::Stack< int > stack2(stack1);
  BOOST_TEST(stack2.size() == 3);
  BOOST_TEST(stack2.top() == 3);
}

BOOST_AUTO_TEST_CASE(operator_test_stack)
{
  abramov::Stack< int > stack1;
  stack1.push(1);
  stack1.push(2);
  stack1.push(3);
  abramov::Stack< int > stack2;
  stack2 = stack1;
  BOOST_TEST(stack2.size() == 3);
  BOOST_TEST(stack2.top() == 3);
}

BOOST_AUTO_TEST_CASE(push_stack)
{
  abramov::Stack< int > stack;
  stack.push(1);
  stack.push(3);
  stack.push(2);
  BOOST_TEST(stack.top() == 2);
}

BOOST_AUTO_TEST_CASE(top_stack)
{
  abramov::Stack< std::string > stack;
  stack.push("a");
  stack.push("b");
  BOOST_TEST(stack.top() == "b");
}

BOOST_AUTO_TEST_CASE(pop_stack)
{
  abramov::Stack< int > stack;
  stack.push(1);
  stack.push(3);
  stack.push(2);
  stack.pop();
  BOOST_TEST(stack.top() == 3);
}

BOOST_AUTO_TEST_CASE(size_stack)
{
  abramov::Stack< int > stack;
  for (size_t i = 0; i < 202; ++i)
  {
    stack.push(i);
  }
  BOOST_TEST(stack.size() == 202);
}

BOOST_AUTO_TEST_CASE(empty_stack)
{
  abramov::Stack< int > stack;
  BOOST_TEST(stack.empty());
  stack.push(1);
  BOOST_TEST(!stack.empty());
  stack.pop();
  BOOST_TEST(stack.empty());
}

BOOST_AUTO_TEST_CASE(swap_stack)
{
  abramov::Stack< int > stack1;
  stack1.push(1);
  stack1.push(2);
  abramov::Stack< int > stack2;
  stack2.push(2);
  stack2.push(1);
  stack1.swap(stack2);
  BOOST_TEST(stack1.top() == 1);
  BOOST_TEST(stack2.top() == 2);
}
