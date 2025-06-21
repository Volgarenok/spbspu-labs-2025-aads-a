#include <boost/test/unit_test.hpp>
#include "stack.hpp"

BOOST_AUTO_TEST_CASE(default_constructor_stack)
{
  zakirov::Stack< int > stack_t;
  BOOST_TEST(stack_t.size() == 0);
  BOOST_TEST(stack_t.empty() == true);
}

BOOST_AUTO_TEST_CASE(copy_constructor_stack)
{
  zakirov::Stack< int > stack_o;
  stack_o.push(0);
  stack_o.push(1);
  zakirov::Stack< int > stack_t(stack_o);
  BOOST_TEST(stack_t.size() == 2);
  BOOST_TEST(stack_t.top() == 1);
  BOOST_TEST(stack_o.size() == 2);
  BOOST_TEST(stack_o.top() == 1);
}

BOOST_AUTO_TEST_CASE(move_constructor_stack)
{
  zakirov::Stack< int > stack_o;
  stack_o.push(0);
  stack_o.push(1);
  zakirov::Stack< int > stack_t(std::move(stack_o));
  BOOST_TEST(stack_t.size() == 2);
  BOOST_TEST(stack_t.top() == 1);
  BOOST_TEST(stack_o.size() == 0);
  BOOST_TEST(stack_o.empty() == true);
}

BOOST_AUTO_TEST_CASE(copy_assignment_operator_stack)
{
  zakirov::Stack< int > stack_o;
  zakirov::Stack< int > stack_t;
  stack_o.push(0);
  stack_o.push(1);
  stack_t = stack_o;
  BOOST_TEST(stack_t.size() == 2);
  BOOST_TEST(stack_t.top() == 1);
  BOOST_TEST(stack_o.size() == 2);
  BOOST_TEST(stack_o.top() == 1);
}

BOOST_AUTO_TEST_CASE(move_assignment_operator_stack)
{
  zakirov::Stack< int > stack_o;
  zakirov::Stack< int > stack_t;
  stack_o.push(0);
  stack_o.push(1);
  stack_t = std::move(stack_o);
  BOOST_TEST(stack_t.size() == 2);
  BOOST_TEST(stack_t.top() == 1);
  BOOST_TEST(stack_o.size() == 0);
  BOOST_TEST(stack_o.empty() == true);
}

BOOST_AUTO_TEST_CASE(empty_stack)
{
  zakirov::Stack< int > stack_t;
  BOOST_TEST(stack_t.empty() == true);
  stack_t.push(0);
  BOOST_TEST(stack_t.empty() == false);
}

BOOST_AUTO_TEST_CASE(size_stack)
{
  zakirov::Stack< int > stack_t;
  BOOST_TEST(stack_t.size() == 0);
  stack_t.push(0);
  BOOST_TEST(stack_t.size() == 1);
  stack_t.push(1);
  BOOST_TEST(stack_t.size() == 2);
}

BOOST_AUTO_TEST_CASE(top_stack)
{
  zakirov::Stack< int > stack_t;
  stack_t.push(0);
  BOOST_TEST(stack_t.top() == 0);
  stack_t.push(1);
  BOOST_TEST(stack_t.top() == 1);
}

BOOST_AUTO_TEST_CASE(push_lvalue_stack)
{
  zakirov::Stack< int > stack_t;
  stack_t.push(0);
  stack_t.push(1);
  stack_t.push(2);
  BOOST_TEST(stack_t.top() == 2);
  BOOST_TEST(stack_t.size() == 3);
  BOOST_TEST(stack_t.empty() == false);
}

BOOST_AUTO_TEST_CASE(push_rvalue_stack)
{
  zakirov::Stack< zakirov::Stack< int > > stack_t;
  zakirov::Stack< int > stack_o;
  stack_o.push(0);
  stack_o.push(1);
  stack_o.push(2);
  stack_t.push(std::move(stack_o));
  BOOST_TEST(stack_t.size() == 1);
  BOOST_TEST(stack_t.empty() == false);
  BOOST_TEST(stack_o.size() == 0);
  BOOST_TEST(stack_o.empty() == true);
}

BOOST_AUTO_TEST_CASE(pop_stack)
{
  zakirov::Stack< int > stack_t;
  stack_t.push(0);
  stack_t.push(1);
  stack_t.push(2);
  BOOST_TEST(stack_t.top() == 2);
  stack_t.pop();
  BOOST_TEST(stack_t.top() == 1);
}

BOOST_AUTO_TEST_CASE(swap_stack)
{
  zakirov::Stack< int > stack_t;
  stack_t.push(0);
  stack_t.push(1);
  stack_t.push(2);
  zakirov::Stack< int > stack_o;
  stack_o.push(5);
  stack_o.push(10);
  BOOST_TEST(stack_t.top() == 2);
  BOOST_TEST(stack_t.size() == 3);
  BOOST_TEST(stack_o.top() == 10);
  BOOST_TEST(stack_o.size() == 2);
  stack_t.swap(stack_o);
  BOOST_TEST(stack_o.top() == 2);
  BOOST_TEST(stack_o.size() == 3);
  BOOST_TEST(stack_t.top() == 10);
  BOOST_TEST(stack_t.size() == 2);
}
