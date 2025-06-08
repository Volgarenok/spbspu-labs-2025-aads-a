#include <boost/test/unit_test.hpp>
#include <sstream>
#include <stack.hpp>

namespace
{
  std::ostream & printStack(std::ostream & out, maslov::Stack< int > stack)
  {
    while (!stack.empty())
    {
      out << stack.top();
      stack.pop();
    }
    return out;
  }
}

BOOST_AUTO_TEST_CASE(defaultConstructorStack)
{
  maslov::Stack< int > stack;
  BOOST_TEST(stack.empty());
}

BOOST_AUTO_TEST_CASE(copyConstructorStack)
{
  maslov::Stack< int > stack;
  stack.push(1);
  stack.push(2);
  maslov::Stack< int > copyStack(stack);
  std::ostringstream out;
  printStack(out, copyStack);
  BOOST_TEST(out.str() == "21");
  BOOST_TEST(stack.size() == 2);
}

BOOST_AUTO_TEST_CASE(moveConstructorStack)
{
  maslov::Stack< int > stack;
  stack.push(1);
  stack.push(2);
  maslov::Stack< int > moveStack(std::move(stack));
  std::ostringstream out;
  printStack(out, moveStack);
  BOOST_TEST(out.str() == "21");
  BOOST_TEST(stack.empty());
}

BOOST_AUTO_TEST_CASE(copyOperatorStack)
{
  maslov::Stack< int > stack;
  stack.push(1);
  stack.push(2);
  maslov::Stack< int > copyStack;
  copyStack = stack;
  std::ostringstream out;
  printStack(out, copyStack);
  BOOST_TEST(out.str() == "21");
  BOOST_TEST(stack.size() == 2);
}

BOOST_AUTO_TEST_CASE(moveOperatorStack)
{
  maslov::Stack< int > stack;
  stack.push(1);
  stack.push(2);
  maslov::Stack< int > moveStack;
  moveStack = std::move(stack);
  std::ostringstream out;
  printStack(out, moveStack);
  BOOST_TEST(out.str() == "21");
  BOOST_TEST(stack.empty());
}

BOOST_AUTO_TEST_CASE(pushTopStack)
{
  maslov::Stack< int > stack;
  stack.push(1);
  stack.push(2);
  BOOST_TEST(stack.top() == 2);
  BOOST_TEST(!stack.empty());
}

BOOST_AUTO_TEST_CASE(popStack)
{
  maslov::Stack< int > stack;
  stack.push(1);
  stack.push(2);
  stack.pop();
  BOOST_TEST(stack.top() == 1);
}

BOOST_AUTO_TEST_CASE(swapStack)
{
  maslov::Stack< int > stack;
  stack.push(1);
  stack.push(2);
  maslov::Stack< int > otherStack;
  stack.swap(otherStack);
  std::ostringstream out;
  printStack(out, otherStack);
  BOOST_TEST(stack.empty());
  BOOST_TEST(out.str() == "21");
}
