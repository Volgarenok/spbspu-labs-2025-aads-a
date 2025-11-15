#include <boost/test/unit_test.hpp>
#include "queue.hpp"

BOOST_AUTO_TEST_CASE(queue_push_and_top_test)
{
  lebedev::Queue< int > test;
  test.push(17);
  BOOST_TEST(test.top() == 17);
  test.push(19);
  BOOST_TEST(test.top() == 17);
}

BOOST_AUTO_TEST_CASE(queue_copy_constr_test)
{
  lebedev::Queue< int > test1;
  test1.push(17);
  lebedev::Queue< int > test2 = test1;
  BOOST_TEST(test2.top() == 17);
}

BOOST_AUTO_TEST_CASE(queue_copy_oper_test)
{
  lebedev::Queue< int > test1;
  test1.push(17);
  lebedev::Queue< int > test2;
  test2.push(19);
  test2 = test1;
  BOOST_TEST(test2.top() == 17);
}

BOOST_AUTO_TEST_CASE(queue_move_constr_test)
{
  lebedev::Queue< int > test1;
  test1.push(17);
  lebedev::Queue< int > test2 = std::move(test1);
  BOOST_TEST(test2.top() == 17);
  BOOST_TEST(test1.isEmpty());
}

BOOST_AUTO_TEST_CASE(queue_move_oper_test)
{
  lebedev::Queue< int > test1;
  test1.push(17);
  lebedev::Queue< int > test2;
  test2 = std::move(test1);
  BOOST_TEST(test2.top() == 17);
  BOOST_TEST(test1.isEmpty());
}

BOOST_AUTO_TEST_CASE(queue_pop_test)
{
  lebedev::Queue< int > test;
  test.push(17);
  test.push(19);
  test.pop();
  BOOST_TEST(test.top() == 19);
}
