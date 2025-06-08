#include <boost/test/unit_test.hpp>
#include "stack.hpp"
#include "mem_checker.hpp"

BOOST_AUTO_TEST_SUITE(S2_stack_test)

BOOST_AUTO_TEST_CASE(empty_constructor_test)
{
  rychkov::Stack< int > stack;
  BOOST_TEST(stack.empty());
  BOOST_TEST(stack.size() == 0);
}
BOOST_AUTO_TEST_CASE(push_test)
{
  rychkov::Stack< int > stack;
  stack.push(3);
  stack.push(9);
  BOOST_TEST(!stack.empty());
  BOOST_TEST(stack.size() == 2);
  BOOST_TEST(stack.capacity() >= 2);
  BOOST_TEST(stack.top() == 9);
}
BOOST_AUTO_TEST_CASE(copy_test)
{
  rychkov::Stack< int > stack1;
  stack1.push(3);
  stack1.push(9);
  rychkov::Stack< int > stack2 = stack1;
  BOOST_TEST(stack2.size() == stack1.size());
  BOOST_TEST(stack2.capacity() >= stack1.size());
  BOOST_TEST(stack2.top() == stack1.top());
}
BOOST_AUTO_TEST_CASE(move_test)
{
  rychkov::Stack< int > stack1;
  stack1.push(3);
  stack1.push(9);
  rychkov::Stack< int > stack2 = std::move(stack1);
  BOOST_TEST(stack2.size() == 2);
  BOOST_TEST(stack2.capacity() >= stack2.size());
  BOOST_TEST(stack2.top() == 9);
}
BOOST_AUTO_TEST_CASE(pop_test)
{
  rychkov::Stack< int > stack;
  stack.push(3);
  stack.push(9);
  stack.push(27);
  stack.pop();
  BOOST_TEST(stack.top() == 9);
  BOOST_TEST(stack.size() == 2);
  stack.pop();
  BOOST_TEST(stack.top() == 3);
  BOOST_TEST(stack.size() == 1);
  stack.pop();
  BOOST_TEST(stack.empty());
}
BOOST_AUTO_TEST_CASE(destructors_leak_test)
{
  struct A
  {};
  rychkov::MemTrack< A > observer{};
  rychkov::Stack< rychkov::MemChecker< A > > stack;
  stack.emplace();
  stack.emplace();
  stack.pop();
  stack.emplace();
  stack.emplace();
  stack.emplace();
  stack.pop();
  stack.pop();
}

BOOST_AUTO_TEST_SUITE_END()
