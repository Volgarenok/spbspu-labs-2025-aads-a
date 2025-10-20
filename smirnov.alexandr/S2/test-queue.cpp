#include <boost/test/unit_test.hpp>
#include <utils/queue.hpp>

BOOST_AUTO_TEST_CASE(queue_empty_after_creation)
{
  smirnov::Queue< int > q;
  BOOST_TEST(q.empty());
  BOOST_TEST(q.size() == 0u);
}

BOOST_AUTO_TEST_CASE(queue_not_empty_after_push)
{
  smirnov::Queue< int > q;
  q.push(1);
  BOOST_TEST(!q.empty());
  BOOST_TEST(q.size() == 1u);
}

BOOST_AUTO_TEST_CASE(queue_front_returns_first_element)
{
  smirnov::Queue< int > q;
  q.push(1);
  q.push(2);
  int first = q.front();
  BOOST_TEST(first == 1);
  BOOST_TEST(q.size() == 2u);
}

BOOST_AUTO_TEST_CASE(queue_pop_removes_first_element)
{
  smirnov::Queue< int > q;
  q.push(1);
  q.push(2);
  int first = q.front();
  BOOST_TEST(first == 1);
  q.pop();
  BOOST_TEST(q.front() == 2);
  BOOST_TEST(q.size() == 1u);
}

BOOST_AUTO_TEST_CASE(queue_maintains_fifo_order)
{
  smirnov::Queue< int > q;
  q.push(1);
  q.push(2);
  q.push(3);
  int a = q.front();
  q.pop();
  int b = q.front();
  q.pop();
  int c = q.front();
  q.pop();
  BOOST_TEST(a == 1);
  BOOST_TEST(b == 2);
  BOOST_TEST(c == 3);
  BOOST_TEST(q.empty());
  BOOST_TEST(q.size() == 0u);
}

BOOST_AUTO_TEST_CASE(queue_size_changes_after_operations)
{
  smirnov::Queue< int > q;
  BOOST_TEST(q.empty());
  q.push(1);
  BOOST_TEST(q.size() == 1u);
  q.push(2);
  BOOST_TEST(q.size() == 2u);
  q.pop();
  BOOST_TEST(q.size() == 1u);
  q.pop();
  BOOST_TEST(q.empty());
  BOOST_TEST(q.size() == 0u);
}
