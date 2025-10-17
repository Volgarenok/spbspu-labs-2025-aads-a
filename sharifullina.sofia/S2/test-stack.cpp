#include <boost/test/unit_test.hpp>
#include "stack.hpp"
#include <stdexcept>

BOOST_AUTO_TEST_SUITE(TestStack)

BOOST_AUTO_TEST_CASE(TestStackDefaultConstructor)
{
  sharifullina::Stack< int > stack;
  BOOST_TEST(stack.empty());
  BOOST_TEST(stack.size() == 0);
}

BOOST_AUTO_TEST_CASE(TestStackPushAndTop)
{
  sharifullina::Stack< int > stack;
  stack.push(1);
  BOOST_TEST(stack.top() == 1);
  BOOST_TEST(stack.size() == 1);
  BOOST_TEST(!stack.empty());

  stack.push(2);
  BOOST_TEST(stack.top() == 2);
  BOOST_TEST(stack.size() == 2);
}

BOOST_AUTO_TEST_CASE(TestStackPop)
{
  sharifullina::Stack< int > stack;
  stack.push(1);
  stack.push(2);
  stack.push(3);

  BOOST_TEST(stack.top() == 3);
  stack.pop();
  BOOST_TEST(stack.top() == 2);
  stack.pop();
  BOOST_TEST(stack.top() == 1);
  stack.pop();
  BOOST_TEST(stack.empty());
  BOOST_TEST(stack.size() == 0);
}

BOOST_AUTO_TEST_CASE(TestStackCopyConstructor)
{
  sharifullina::Stack< int > stack1;
  stack1.push(1);
  stack1.push(2);
  stack1.push(3);

  sharifullina::Stack< int > stack2(stack1);
  BOOST_TEST(stack2.size() == 3);
  BOOST_TEST(stack2.top() == 3);

  stack2.pop();
  BOOST_TEST(stack2.top() == 2);
  BOOST_TEST(stack1.top() == 3);
}

BOOST_AUTO_TEST_CASE(TestStackMoveConstructor)
{
  sharifullina::Stack< int > stack1;
  stack1.push(1);
  stack1.push(2);
  stack1.push(3);

  sharifullina::Stack< int > stack2(std::move(stack1));
  BOOST_TEST(stack2.size() == 3);
  BOOST_TEST(stack2.top() == 3);
  BOOST_TEST(stack1.empty());
}

BOOST_AUTO_TEST_CASE(TestStackCopyAssignment)
{
  sharifullina::Stack< int > stack1;
  stack1.push(1);
  stack1.push(2);

  sharifullina::Stack< int > stack2;
  stack2 = stack1;
  BOOST_TEST(stack2.size() == 2);
  BOOST_TEST(stack2.top() == 2);

  stack1.push(3);
  stack2 = stack1;
  BOOST_TEST(stack2.size() == 3);
  BOOST_TEST(stack2.top() == 3);
}

BOOST_AUTO_TEST_CASE(TestStackMoveAssignment)
{
  sharifullina::Stack< int > stack1;
  stack1.push(1);
  stack1.push(2);

  sharifullina::Stack< int > stack2;
  stack2 = std::move(stack1);
  BOOST_TEST(stack2.size() == 2);
  BOOST_TEST(stack2.top() == 2);
  BOOST_TEST(stack1.empty());
}

BOOST_AUTO_TEST_CASE(TestStackSwap)
{
  sharifullina::Stack< int > stack1;
  stack1.push(1);
  stack1.push(2);

  sharifullina::Stack< int > stack2;
  stack2.push(3);
  stack2.push(4);
  stack2.push(5);

  stack1.swap(stack2);
  BOOST_TEST(stack1.size() == 3);
  BOOST_TEST(stack1.top() == 5);
  BOOST_TEST(stack2.size() == 2);
  BOOST_TEST(stack2.top() == 2);
}

BOOST_AUTO_TEST_CASE(TestStackExceptions)
{
  sharifullina::Stack< int > stack;
  BOOST_CHECK_THROW(stack.top(), std::logic_error);
  BOOST_CHECK_THROW(stack.pop(), std::logic_error);

  stack.push(1);
  stack.pop();
  BOOST_CHECK_THROW(stack.pop(), std::logic_error);
}

BOOST_AUTO_TEST_SUITE_END()
