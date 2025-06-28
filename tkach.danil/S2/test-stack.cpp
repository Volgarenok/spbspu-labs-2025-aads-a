#include <boost/test/unit_test.hpp>
#include <sstream>
#include "stack.hpp"
#include "print.hpp"
using namespace tkach;

BOOST_AUTO_TEST_CASE(stack_basic_constructor_test)
{
  Stack< int > stack;
  BOOST_TEST(stack.empty());
  BOOST_TEST(stack.size() == 0);
}

BOOST_AUTO_TEST_CASE(stack_copy_constructor_test)
{
  Stack< size_t > stack;
  for (size_t i = 1; i <= 5; ++i)
  {
    stack.push(i);
  }
  Stack< size_t > stack2(stack);
  std::ostringstream out;
  outputStack(out, stack2);
  BOOST_TEST(out.str() == "5 4 3 2 1");
}

BOOST_AUTO_TEST_CASE(stack_move_constructor_test)
{
  Stack< size_t > stack;
  for (size_t i = 1; i <= 5; ++i)
  {
    stack.push(i);
  }
  Stack< size_t > stack2(std::move(stack));
  std::ostringstream out;
  outputStack(out, stack2);
  BOOST_TEST(out.str() == "5 4 3 2 1");
}

BOOST_AUTO_TEST_CASE(stack_equal_operator_test)
{
  Stack< size_t > stack;
  for (size_t i = 1; i <= 5; ++i)
  {
    stack.push(i);
  }
  Stack< size_t > stack2;
  stack2 = stack;
  std::ostringstream out;
  outputStack(out, stack2);
  BOOST_TEST(out.str() == "5 4 3 2 1");
}

BOOST_AUTO_TEST_CASE(stack_pop_test)
{
  Stack< size_t > stack;
  for (size_t i = 1; i <= 5; ++i)
  {
    stack.push(i);
  }
  BOOST_TEST(stack.size() == 5);
  stack.pop();
  BOOST_TEST(stack.size() == 4);
  std::ostringstream out;
  outputStack(out, stack);
  BOOST_TEST(out.str() == "4 3 2 1");
}

BOOST_AUTO_TEST_CASE(stack_lvalue_push_test)
{
  Stack< size_t > stack;
  size_t temp = 3;
  stack.push(temp);
  std::ostringstream out;
  outputStack(out, stack);
  BOOST_TEST(out.str() == "3");
  BOOST_TEST(stack.size() == 1);
}

BOOST_AUTO_TEST_CASE(stack_top_size_test)
{
  Stack< size_t > stack;
  stack.push(2);
  stack.push(3);
  BOOST_TEST(stack.top() == 3);
  BOOST_TEST(stack.size() == 2);
}

BOOST_AUTO_TEST_CASE(stack_move_equal_operator_test)
{
  Stack< size_t > stack;
  for (size_t i = 1; i <= 5; ++i)
  {
    stack.push(i);
  }
  Stack< size_t > stack2;
  stack2 = std::move(stack);
  std::ostringstream out;
  outputStack(out, stack2);
  BOOST_TEST(stack.empty());
  BOOST_TEST(out.str() == "5 4 3 2 1");
}

BOOST_AUTO_TEST_CASE(stack_swap_test)
{
  Stack< size_t > stack;
  for (size_t i = 1; i <= 5; ++i)
  {
    stack.push(i);
  }
  Stack< size_t > stack2;
  stack.swap(stack2);
  std::ostringstream out;
  outputStack(out, stack2);
  BOOST_TEST(stack.empty());
  BOOST_TEST(out.str() == "5 4 3 2 1");
}

BOOST_AUTO_TEST_CASE(stack_move_push_test)
{
  Stack< int > stack;
  stack.push(1);
  BOOST_TEST(!stack.empty());
  BOOST_TEST(stack.size() == 1);
  BOOST_TEST(stack.top() == 1);
  stack.push(2);
  BOOST_TEST(stack.top() == 2);
  BOOST_TEST(stack.size() == 2);
}
