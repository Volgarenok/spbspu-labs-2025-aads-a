#include <boost/test/unit_test.hpp>
#include <utils/queue.hpp>

BOOST_AUTO_TEST_CASE(queue_empty_after_creation)
{
  smirnov::Queue< int > q;
  BOOST_TEST(q.empty());
}

BOOST_AUTO_TEST_CASE(queue_not_empty_after_push)
{
  smirnov::Queue< int > q;
  q.push(1);
  BOOST_TEST(!q.empty());
}

BOOST_AUTO_TEST_CASE(queue_front_returns_first_element)
{
  smirnov::Queue< int > q;
  q.push(1);
  q.push(2);
  BOOST_TEST(q.front() == 1);
}

BOOST_AUTO_TEST_CASE(queue_drop_removes_first_element)
{
  smirnov::Queue< int > q;
  q.push(1);
  q.push(2);
  BOOST_TEST(q.drop() == 1);
  BOOST_TEST(q.front() == 2);
}

BOOST_AUTO_TEST_CASE(queue_maintains_fifo_order)
{
  smirnov::Queue< int > q;
  q.push(1);
  q.push(2);
  q.push(3);
  BOOST_TEST(q.drop() == 1);
  BOOST_TEST(q.drop() == 2);
  BOOST_TEST(q.drop() == 3);
  BOOST_TEST(q.empty());
}

BOOST_AUTO_TEST_CASE(queue_size_changes_after_operations)
{
  smirnov::Queue< int > q;
  BOOST_TEST(q.empty());
  q.push(1);
  BOOST_TEST(!q.empty());
  q.push(2);
  BOOST_TEST(!q.empty());
  q.drop();
  BOOST_TEST(!q.empty());
  q.drop();
  BOOST_TEST(q.empty());
}
