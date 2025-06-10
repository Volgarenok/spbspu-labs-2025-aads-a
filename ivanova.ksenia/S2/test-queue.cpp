#include <algorithm>
#include <boost/test/unit_test.hpp>

#include "queue.hpp"

BOOST_AUTO_TEST_CASE(QueueDefaultConstructor)
{
  ivanova::Queue<int> q;
  BOOST_CHECK(q.empty());
}

BOOST_AUTO_TEST_CASE(QueueCopyConstructor)
{
  ivanova::Queue<int> q;
  q.push(1);
  ivanova::Queue<int> u = q;
  BOOST_CHECK(!u.empty());
  BOOST_CHECK_EQUAL(u.front(), 1);
}

BOOST_AUTO_TEST_CASE(QueueMoveConstructor)
{
  ivanova::Queue<int> q;
  q.push(1);
  ivanova::Queue<int> u = std::move(q);
  BOOST_CHECK_EQUAL(u.front(), 1);
  BOOST_CHECK(q.empty());
}

BOOST_AUTO_TEST_CASE(QueuePush)
{
  ivanova::Queue<int> a;
  for (int i = 0; i < 10; ++i)
  {
    a.push(i * 10);
    BOOST_CHECK_EQUAL(a.back(), i * 10);
  }
  BOOST_CHECK_EQUAL(a.front(), 0);
}

BOOST_AUTO_TEST_CASE(QueuePushPop)
{
  ivanova::Queue<int> a;

  a.push(10);
  BOOST_CHECK_EQUAL(a.back(), 10);
  BOOST_CHECK_EQUAL(a.front(), 10);
  a.pop();
  BOOST_CHECK(a.empty());

  a.push(1);
  a.push(2);
  a.push(3);
  for (int i = 1; i <= 3; ++i)
  {
    BOOST_CHECK_EQUAL(a.front(), i);
    a.pop();
  }
  BOOST_CHECK(a.empty());
}
