#include <boost/test/tools/interface.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include "queue.hpp"
BOOST_AUTO_TEST_SUITE(queue)

BOOST_AUTO_TEST_CASE(constructors)
{
  kiselev::Queue< int > queue;
  queue.push(1);
  kiselev::Queue< int > copyQueue(queue);
  BOOST_TEST(copyQueue.size() == 1);
  kiselev::Queue< int > moveQueue(std::move(queue));
  BOOST_TEST(moveQueue.size() == 1);
}

BOOST_AUTO_TEST_CASE(assignment_operators)
{
  kiselev::Queue< int > queue;
  kiselev::Queue< int > queue2;
  queue.push(1);
  queue2 = queue;
  BOOST_TEST(queue.size() == 1);
  kiselev::Queue< int > queue3;
  queue3 = queue;
  BOOST_TEST(queue3.size() == 1);
}

BOOST_AUTO_TEST_CASE(back_and_front)
{
  kiselev::Queue< int > queue;
  queue.push(1);
  queue.push(3);
  const int i = queue.back();
  BOOST_TEST(i == 3);
  BOOST_TEST(queue.back() == 3);
  const int a = queue.front();
  BOOST_TEST(a == 1);
  BOOST_TEST(queue.front() == 1);
}

BOOST_AUTO_TEST_CASE(pop_and_push)
{
  kiselev::Queue< int > queue;
  int i = 7;
  queue.push(i);
  queue.push(1);
  queue.pop();
  BOOST_TEST(queue.front() == 1);
}

BOOST_AUTO_TEST_CASE(empty_and_size_and_clear)
{
  kiselev::Queue< int > queue;
  queue.push(1);
  queue.push(3);
  BOOST_TEST(queue.size() == 2);
  queue.clear();
  BOOST_TEST(queue.empty());
}

BOOST_AUTO_TEST_SUITE_END()
