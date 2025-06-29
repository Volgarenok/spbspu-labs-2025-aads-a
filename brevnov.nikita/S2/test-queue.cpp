#include <boost/test/tools/interface.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include "queue.hpp"

BOOST_AUTO_TEST_SUITE(queue)

BOOST_AUTO_TEST_CASE(constructors)
{
  brevnov::Queue< int > test_queue;
  test_queue.push(6);
  brevnov::Queue< int > test_copy_queue(test_queue);
  BOOST_TEST(test_copy_queue.front() == 6);
  brevnov::Queue< int > test_move_queue(std::move(test_copy_queue));
  BOOST_TEST(test_move_queue.front() == 6);
}

BOOST_AUTO_TEST_CASE(front)
{
  brevnov::Queue< int > test_queue;
  test_queue.push(5);
  test_queue.push(2);
  const int i = test_queue.front();
  BOOST_TEST(i == 5);
  BOOST_TEST(test_queue.front() == 5);
}

BOOST_AUTO_TEST_CASE(assignment_operators)
{
  brevnov::Queue< int > test_queue;
  test_queue.push(9);
  BOOST_TEST(test_queue.front() == 9);
}

BOOST_AUTO_TEST_CASE(push_and_pop)
{
  brevnov::Queue< int > test_queue;
  int i = 2;
  test_queue.push(i);
  BOOST_TEST(test_queue.front() == 2);
  test_queue.push(4);
  test_queue.push(8);
  test_queue.pop();
  BOOST_TEST(test_queue.front() == 4);
}

BOOST_AUTO_TEST_CASE(empty_size_clear)
{
  brevnov::Queue< int > test_queue;
  for (int i = 0; i < 15; ++i)
  {
    test_queue.push(i);
  }
  BOOST_TEST(test_queue.size() == 15);
  test_queue.clear();
  BOOST_TEST(test_queue.empty());
}

BOOST_AUTO_TEST_SUITE_END()
