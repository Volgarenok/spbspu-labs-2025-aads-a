#include <boost/test/unit_test.hpp>
#include "stack.hpp"

BOOST_AUTO_TEST_CASE(stack_push_and_top_test)
{
  lebedev::Stack< int > test;
  test.push(17);
  BOOST_TEST(test.size() == 1);
  test.push(19);
  BOOST_TEST(test.size() == 2);
}

BOOST_AUTO_TEST_CASE(stack_copy_constr_test)
{
  lebedev::Stack< int > test1;
  test1.push(17);
  lebedev::Stack< int > test2 = test1;
  BOOST_TEST(test2.top() == 17);
}

BOOST_AUTO_TEST_CASE(stack_copy_oper_test)
{
  lebedev::Stack< int > test1;
  test1.push(17);
  lebedev::Stack< int > test2;
  test2.push(19);
  test2 = test1;
  BOOST_TEST(test2.top() == 17);
}

BOOST_AUTO_TEST_CASE(stack_move_constr_test)
{
  lebedev::Stack< int > test1;
  test1.push(17);
  lebedev::Stack< int > test2 = std::move(test1);
  BOOST_TEST(test2.top() == 17);
  BOOST_TEST(test1.isEmpty());
}

BOOST_AUTO_TEST_CASE(stack_move_oper_test)
{
  lebedev::Stack< int > test1;
  test1.push(17);
  lebedev::Stack< int > test2;
  test2 = std::move(test1);
  BOOST_TEST(test2.top() == 17);
  BOOST_TEST(test1.isEmpty());
}

BOOST_AUTO_TEST_CASE(stack_pop_test)
{
  lebedev::Stack< int > test;
  test.push(17);
  test.push(19);
  test.pop();
  BOOST_TEST(test.top() == 17);
}
