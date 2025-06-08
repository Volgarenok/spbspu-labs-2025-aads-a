#include <boost/test/tools/interface.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include "stack.hpp"

BOOST_AUTO_TEST_SUITE(stack)

BOOST_AUTO_TEST_CASE(constructors)
{
  kiselev::Stack< int > stack;
  stack.push(1);
  kiselev::Stack< int > copyStack(stack);
  BOOST_TEST(copyStack.size() == 1);
  kiselev::Stack< int > moveStack(std::move(stack));
  BOOST_TEST(moveStack.size() == 1);
}

BOOST_AUTO_TEST_CASE(assignment_operators)
{
  kiselev::Stack< int > stack;
  kiselev::Stack< int > stack2;
  stack.push(1);
  stack2 = stack;
  BOOST_TEST(stack2.size() == 1);
  kiselev::Stack< int > stack3;
  stack3 = std::move(stack);
  BOOST_TEST(stack3.size() == 1);
}

BOOST_AUTO_TEST_CASE(back_and_push_and_pop)
{
  kiselev::Stack< int > stack;
  stack.push(1);
  int i = 5;
  stack.push(i);
  const int a = stack.top();
  BOOST_TEST(a == 5);
  stack.pop();
  BOOST_TEST(stack.top() == 1);
}

BOOST_AUTO_TEST_CASE(empty_and_size_and_clear)
{
  kiselev::Stack< int > stack;
  stack.push(1);
  stack.push(2);
  BOOST_TEST(stack.size() == 2);
  stack.clear();
  BOOST_TEST(stack.empty());
}
BOOST_AUTO_TEST_SUITE_END()
