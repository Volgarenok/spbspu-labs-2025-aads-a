#include <boost/test/unit_test.hpp>
#include <string>
#include <Stack.hpp>

BOOST_AUTO_TEST_CASE(TestElementAccessStack)
{
  gavrilova::Stack< int > stack{};
  stack.push(1);
  stack.push(2);
  stack.push(1 + 2);
  BOOST_TEST(stack.top() == 3);
}

BOOST_AUTO_TEST_CASE(TestConstructorsStack)
{
  gavrilova::Stack< int > stack1{};
  stack1.push(1);
  stack1.push(2);
  stack1.push(1 + 2);
  gavrilova::Stack< int > stack2(stack1);
  BOOST_TEST(stack1.top() == stack2.top());
  BOOST_TEST(stack1.size() == stack2.size());
}

BOOST_AUTO_TEST_CASE(TestAssignmentOperatorsStack)
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

BOOST_AUTO_TEST_CASE(TestPopStack)
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

BOOST_AUTO_TEST_CASE(TestEmptyStack)
{
  gavrilova::Stack< int > stack;
  BOOST_TEST(stack.empty());
  BOOST_TEST(stack.size() == 0);
}

BOOST_AUTO_TEST_CASE(TestPushTopStack)
{
  gavrilova::Stack< int > stack;
  stack.push(1);
  BOOST_TEST(stack.top() == 1);
  stack.push(2);
  BOOST_TEST(stack.top() == 2);
}

BOOST_AUTO_TEST_CASE(TestMoveSemanticsStack)
{
  gavrilova::Stack< std::string > stack1;
  stack1.push("test");
  gavrilova::Stack< std::string > stack2 = std::move(stack1);
  BOOST_TEST(stack2.top() == "test");
  BOOST_TEST(stack1.empty());
}

BOOST_AUTO_TEST_CASE(TestSwapStack)
{
  gavrilova::Stack< int > stack1;
  stack1.push(1);
  stack1.push(2);
  gavrilova::Stack< int > stack2;
  stack2.push(3);
  stack1.swap(stack2);
  BOOST_TEST(stack1.top() == 3);
  BOOST_TEST(stack1.size() == 1);
  BOOST_TEST(stack2.top() == 2);
  BOOST_TEST(stack2.size() == 2);
}

BOOST_AUTO_TEST_CASE(TestPopEmptyStack)
{
  gavrilova::Stack< int > stack;
  BOOST_CHECK_THROW(stack.pop(), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(TestTopEmptyStack)
{
  gavrilova::Stack< int > stack;
  BOOST_CHECK_THROW(stack.top(), std::out_of_range);
}
