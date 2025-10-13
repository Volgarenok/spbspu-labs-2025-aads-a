#include <boost/test/unit_test.hpp>
#include "queue.hpp"

using namespace krylov;

BOOST_AUTO_TEST_CASE(test_queue_push)
{
  Queue< int > queue;
  queue.push(10);
  BOOST_TEST(queue.back() == 10);
  queue.push(11);
  BOOST_TEST(queue.back() == 11);
  queue.push(12);
  BOOST_TEST(queue.back() == 12);
}

BOOST_AUTO_TEST_CASE(test_queue_pop)
{
  Queue< int > queue;
  queue.push(11);
  queue.push(12);
  queue.push(13);
  queue.pop();
  BOOST_TEST(queue.front() == 12);
  queue.pop();
  BOOST_TEST(queue.front() == 13);
  queue.pop();
  BOOST_TEST(queue.empty());
}

BOOST_AUTO_TEST_CASE(test_queue_empty)
{
  Queue< int > queue;
  BOOST_TEST(queue.empty());
  queue.push(10);
  BOOST_TEST(!queue.empty());
}

BOOST_AUTO_TEST_CASE(test_queue_size)
{
  Queue< int > queue;
  BOOST_TEST(queue.size() == 0);
  queue.push(10);
  queue.push(11);
  BOOST_TEST(queue.size() == 2);
}
