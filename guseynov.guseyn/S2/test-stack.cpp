#include <boost/test/unit_test.hpp>
#include "calculation-stack.hpp"

BOOST_AUTO_TEST_CASE(stack_constructors_and_assignment_operators)
{
  guseynov::Stack< int > originalStack;
  originalStack.push(1);
  originalStack.push(2);
  guseynov::Stack< int > copiedStack = originalStack;
  BOOST_TEST(originalStack.top() == copiedStack.top());
  BOOST_TEST(originalStack.size() == copiedStack.size());

  guseynov::Stack< int > assignedStack;
  assignedStack = originalStack;
  BOOST_TEST(originalStack.top() == assignedStack.top());
  BOOST_TEST(originalStack.size() == assignedStack.size());
}

BOOST_AUTO_TEST_CASE(stack_operations)
{
  guseynov::Stack< int > testStack;
  BOOST_TEST(testStack.empty());
  testStack.push(1);
  BOOST_TEST(testStack.top() == 1);
  testStack.push(2);
  BOOST_TEST(testStack.top() == 2);
  BOOST_TEST(!testStack.empty());
  BOOST_TEST(testStack.size() == 2);
  testStack.pop();
  BOOST_TEST(testStack.top() == 1);
  testStack.pop();
  BOOST_TEST(testStack.empty());
}
