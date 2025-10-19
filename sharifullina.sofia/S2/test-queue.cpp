#include <boost/test/unit_test.hpp>
#include "queue.hpp"
#include <stdexcept>

BOOST_AUTO_TEST_SUITE(TestQueue)

BOOST_AUTO_TEST_CASE(TestQueueDefaultConstructor)
{
  sharifullina::Queue< int > queue;
  BOOST_TEST(queue.empty());
  BOOST_TEST(queue.size() == 0);
}

BOOST_AUTO_TEST_CASE(TestQueuePushAndFront)
{
  sharifullina::Queue< int > queue;
  queue.push(1);
  BOOST_TEST(queue.front() == 1);
  BOOST_TEST(queue.size() == 1);
  BOOST_TEST(!queue.empty());

  queue.push(2);
  BOOST_TEST(queue.front() == 1);
  BOOST_TEST(queue.size() == 2);
}

BOOST_AUTO_TEST_CASE(TestQueuePop)
{
  sharifullina::Queue< int > queue;
  queue.push(1);
  queue.push(2);
  queue.push(3);

  BOOST_TEST(queue.front() == 1);
  queue.pop();
  BOOST_TEST(queue.front() == 2);
  queue.pop();
  BOOST_TEST(queue.front() == 3);
  queue.pop();
  BOOST_TEST(queue.empty());
  BOOST_TEST(queue.size() == 0);
}

BOOST_AUTO_TEST_CASE(TestQueueCopyConstructor)
{
  sharifullina::Queue< int > queue1;
  queue1.push(1);
  queue1.push(2);
  queue1.push(3);

  sharifullina::Queue< int > queue2(queue1);
  BOOST_TEST(queue2.size() == 3);
  BOOST_TEST(queue2.front() == 1);

  queue2.pop();
  BOOST_TEST(queue2.front() == 2);
  BOOST_TEST(queue1.front() == 1);
}

BOOST_AUTO_TEST_CASE(TestQueueMoveConstructor)
{
  sharifullina::Queue< int > queue1;
  queue1.push(1);
  queue1.push(2);
  queue1.push(3);

  sharifullina::Queue< int > queue2(std::move(queue1));
  BOOST_TEST(queue2.size() == 3);
  BOOST_TEST(queue2.front() == 1);
  BOOST_TEST(queue1.empty());
}

BOOST_AUTO_TEST_CASE(TestQueueCopyAssignment)
{
  sharifullina::Queue< int > queue1;
  queue1.push(1);
  queue1.push(2);

  sharifullina::Queue< int > queue2;
  queue2 = queue1;
  BOOST_TEST(queue2.size() == 2);
  BOOST_TEST(queue2.front() == 1);

  queue1.push(3);
  queue2 = queue1;
  BOOST_TEST(queue2.size() == 3);
  BOOST_TEST(queue2.front() == 1);
}

BOOST_AUTO_TEST_CASE(TestQueueMoveAssignment)
{
  sharifullina::Queue< int > queue1;
  queue1.push(1);
  queue1.push(2);

  sharifullina::Queue< int > queue2;
  queue2 = std::move(queue1);
  BOOST_TEST(queue2.size() == 2);
  BOOST_TEST(queue2.front() == 1);
  BOOST_TEST(queue1.empty());
}

BOOST_AUTO_TEST_CASE(TestQueueSwap)
{
  sharifullina::Queue< int > queue1;
  queue1.push(1);
  queue1.push(2);

  sharifullina::Queue< int > queue2;
  queue2.push(3);
  queue2.push(4);
  queue2.push(5);

  queue1.swap(queue2);
  BOOST_TEST(queue1.size() == 3);
  BOOST_TEST(queue1.front() == 3);
  BOOST_TEST(queue2.size() == 2);
  BOOST_TEST(queue2.front() == 1);
}

BOOST_AUTO_TEST_CASE(TestQueueExceptions)
{
  sharifullina::Queue< int > queue;
  BOOST_CHECK_THROW(queue.front(), std::logic_error);
  BOOST_CHECK_THROW(queue.pop(), std::logic_error);

  queue.push(1);
  queue.pop();
  BOOST_CHECK_THROW(queue.pop(), std::logic_error);
}

BOOST_AUTO_TEST_CASE(TestQueueOrder)
{
  sharifullina::Queue< int > queue;
  for (int i = 1; i <= 5; ++i)
  {
    queue.push(i);
  }

  for (int i = 1; i <= 5; ++i)
  {
    BOOST_TEST(queue.front() == i);
    queue.pop();
  }
  BOOST_TEST(queue.empty());
}

BOOST_AUTO_TEST_SUITE_END()
