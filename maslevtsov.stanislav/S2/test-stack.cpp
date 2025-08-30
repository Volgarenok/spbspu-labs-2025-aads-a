#include <boost/test/unit_test.hpp>
#include "stack.hpp"

BOOST_AUTO_TEST_SUITE(stack_constructors_tests)
BOOST_AUTO_TEST_CASE(stack_default_constructor_test)
{
  maslevtsov::Stack< int > s;
  BOOST_TEST(s.empty());
}

BOOST_AUTO_TEST_CASE(stack_copy_constructor_test)
{
  maslevtsov::Stack< int > s1;
  s1.push(1);
  maslevtsov::Stack< int > s2(s1);
  BOOST_TEST(s1.top() == s2.top());
  BOOST_TEST(s1.size() == s2.size());
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(stack_operators_tests)
BOOST_AUTO_TEST_CASE(stack_copy_assignment_operator_test)
{
  maslevtsov::Stack< int > s1;
  s1.push(1);
  maslevtsov::Stack< int > s2;
  s2 = s1;
  BOOST_TEST(s1.top() == s2.top());
  BOOST_TEST(s1.size() == s2.size());
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(stack_elements_access_tests)
BOOST_AUTO_TEST_CASE(stack_top_test)
{
  maslevtsov::Stack< int > s;
  s.push(1);
  BOOST_TEST(s.top() == 1);
  s.push(2);
  BOOST_TEST(s.top() == 2);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(stack_capacity_tests)
BOOST_AUTO_TEST_CASE(stack_empty_test)
{
  maslevtsov::Stack< int > s;
  BOOST_TEST(s.empty());
  s.push(1);
  BOOST_TEST(!s.empty());
}

BOOST_AUTO_TEST_CASE(stack_size_test)
{
  maslevtsov::Stack< int > s;
  s.push(1);
  BOOST_TEST(s.size() == 1);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(stack_modifiers_tests)
BOOST_AUTO_TEST_CASE(stack_push_test)
{
  maslevtsov::Stack< int > s;
  s.push(1);
  BOOST_TEST(s.top() == 1);
  BOOST_TEST(s.size() == 1);
  int n = 2;
  s.push(n);
  BOOST_TEST(s.top() == 2);
  BOOST_TEST(s.size() == 2);
}

BOOST_AUTO_TEST_CASE(stack_pop_test)
{
  maslevtsov::Stack< int > s;
  s.push(1);
  s.push(2);
  s.pop();
  BOOST_TEST(s.size() == 1);
  BOOST_TEST(s.top() == 1);
  s.pop();
  BOOST_TEST(s.empty());
}

BOOST_AUTO_TEST_CASE(stack_swap_test)
{
  maslevtsov::Stack< int > s1;
  s1.push(1);
  maslevtsov::Stack< int > s2;
  s2.push(2);
  s2.push(3);
  s1.swap(s2);
  BOOST_TEST(s1.top() == 3);
  BOOST_TEST(s1.size() == 2);
  s1.pop();
  BOOST_TEST(s1.top() == 2);
  BOOST_TEST(s2.top() == 1);
  BOOST_TEST(s2.size() == 1);
}
BOOST_AUTO_TEST_SUITE_END()
