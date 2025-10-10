#include <boost/test/unit_test.hpp>
#include <stack.hpp>

using namespace kushekbaev;

BOOST_AUTO_TEST_SUITE(stackConstructors)

BOOST_AUTO_TEST_CASE(stackCopyConstructor)
{
  Stack< int > stack;
  stack.push(10);
  Stack< int > stackcopy(stack);
  BOOST_TEST(stackcopy.top() == 10);
  BOOST_TEST(stackcopy.size() == 1);
  BOOST_TEST(stack.top() == stackcopy.top());
}

BOOST_AUTO_TEST_CASE(stackMoveConstructor)
{
  Stack< int > stack;
  stack.push(10);
  Stack< int > stackcopy(std::move(stack));
  BOOST_TEST(stackcopy.top() == 10);
  BOOST_TEST(stackcopy.size() == 1);
  BOOST_TEST(stack.empty());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_CASE(stackAssignment)
{
  Stack< int > stack1;
  stack1.push(10);
  Stack< int > stack2;
  stack2 = stack1;
  BOOST_TEST(stack2.top() == 10);
}

BOOST_AUTO_TEST_CASE(stackTop)
{
  Stack< int > stack;
  stack.push(10);
  BOOST_TEST(stack.top() == 10);
  stack.push(20);
  BOOST_TEST(stack.top() == 20);
  stack.pop();
  BOOST_TEST(stack.top() == 10);
}

BOOST_AUTO_TEST_SUITE(stackCapacity)

BOOST_AUTO_TEST_CASE(stackEmpty)
{
  Stack< int > stack;
  BOOST_TEST(stack.empty());
  stack.push(123);
  BOOST_TEST(!stack.empty());
  stack.pop();
  BOOST_TEST(stack.empty());
}

BOOST_AUTO_TEST_CASE(stackSize)
{
  Stack< int > stack;
  stack.push(10);
  stack.push(22);
  BOOST_TEST(stack.size() == 2);
  stack.pop();
  BOOST_TEST(stack.size() == 1);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_CASE(stackPushPop)
{
  Stack< int > stack;
  stack.push(10);
  BOOST_TEST(stack.top() == 10);
  stack.push(20);
  BOOST_TEST(stack.top() == 20);
  stack.pop();
  stack.pop();
  BOOST_TEST(stack.empty());
}
