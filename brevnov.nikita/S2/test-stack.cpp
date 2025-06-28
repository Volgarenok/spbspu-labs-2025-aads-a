#include <boost/test/tools/interface.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include "stack.hpp"

BOOST_AUTO_TEST_SUITE(stack)

BOOST_AUTO_TEST_CASE(constructors)
{
  brevnov::Stack< int > test_stack;
  test_stack.push(6);
  brevnov::Stack< int > test_copy_stack(test_stack);
  BOOST_TEST(test_copy_stack.top() == 6);
  brevnov::Stack< int > test_move_stack(std::move(test_copy_stack));
  BOOST_TEST(test_move_stack.top() == 6);
}

BOOST_AUTO_TEST_CASE(pop)
{
  brevnov::Stack< int > test_stack;
  test_stack.push(5);
  test_stack.push(2);
  const int i = test_stack.top();
  BOOST_TEST(i == 2);
  BOOST_TEST(test_stack.top() == 2);
}

BOOST_AUTO_TEST_CASE(assignment_operators)
{
  brevnov::Stack< int > test_stack;
  brevnov::Stack< int > test_stack2;
  test_stack2.push(9);
  test_stack = test_stack2;
  BOOST_TEST(test_stack.top() == 9);
  test_stack.push(8);
  test_stack2 = test_stack;
  BOOST_TEST(test_stack2.top() == 8);
}

BOOST_AUTO_TEST_CASE(push_and_pop)
{
  brevnov::Stack< int > test_stack;
  int i = 2;
  test_stack.push(i);
  BOOST_TEST(test_stack.top() == 2);
  test_stack.push(4);
  test_stack.push(8);
  BOOST_TEST(test_stack.top() == 8);
  test_stack.pop();
  BOOST_TEST(test_stack.top() == 4);
}

BOOST_AUTO_TEST_CASE(empty_size_clear)
{
  brevnov::Stack< int > test_stack;
  for (int i = 0; i < 15; ++i)
  {
    test_stack.push(i);
  }
  BOOST_TEST(test_stack.size() == 15);
  test_stack.clear();
  BOOST_TEST(test_stack.empty());
}

BOOST_AUTO_TEST_SUITE_END()
