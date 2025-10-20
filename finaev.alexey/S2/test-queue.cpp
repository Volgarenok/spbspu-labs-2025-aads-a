#include <boost/test/unit_test.hpp>
#include <queue.hpp>

BOOST_AUTO_TEST_CASE(queue_push_and_top_test)
{
  finaev::Queue< int > test;
  test.push(10);
  BOOST_TEST(test.top() == 10);
  test.push(20);
  BOOST_TEST(test.top() == 10);
}

BOOST_AUTO_TEST_CASE(queue_copy_constr_test)
{
  finaev::Queue< int > test1;
  test1.push(10);
  finaev::Queue< int > test2 = test1;
  BOOST_TEST(test2.top() == 10);
}

BOOST_AUTO_TEST_CASE(queue_copy_oper_test)
{
  finaev::Queue< int > test1;
  test1.push(10);
  finaev::Queue< int > test2;
  test2.push(20);
  test2 = test1;
  BOOST_TEST(test2.top() == 10);
}

BOOST_AUTO_TEST_CASE(queue_move_constr_test)
{
  finaev::Queue< int > test1;
  test1.push(10);
  finaev::Queue< int > test2 = std::move(test1);
  BOOST_TEST(test2.top() == 10);
  BOOST_TEST(test1.isEmpty());
}

BOOST_AUTO_TEST_CASE(queue_move_oper_test)
{
  finaev::Queue< int > test1;
  test1.push(10);
  finaev::Queue< int > test2;
  test2 = std::move(test1);
  BOOST_TEST(test2.top() == 10);
  BOOST_TEST(test1.isEmpty());
}

BOOST_AUTO_TEST_CASE(queue_pop_test)
{
  finaev::Queue< int > test;
  test.push(10);
  test.push(20);
  test.pop();
  BOOST_TEST(test.top() == 20);
}
