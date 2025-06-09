#include <algorithm>
#include <boost/test/unit_test.hpp>

#include "stack.hpp"

BOOST_AUTO_TEST_CASE(StackDefaultConstructor)
{
  ivanova::Stack<int> s;
  BOOST_CHECK(s.empty());
}

BOOST_AUTO_TEST_CASE(StackCopyConstructor)
{
  ivanova::Stack<int> q;
  q.push(1);
  ivanova::Stack<int> u = q;
  BOOST_CHECK(!u.empty());
  BOOST_CHECK_EQUAL(u.top(), 1);
}

BOOST_AUTO_TEST_CASE(StackMoveConstructor)
{
  ivanova::Stack<int> q;
  q.push(1);
  ivanova::Stack<int> u = std::move(q);
  BOOST_CHECK_EQUAL(u.top(), 1);
  BOOST_CHECK(q.empty());
}

BOOST_AUTO_TEST_CASE(StackPush)
{
  ivanova::Stack<int> a;
  for (int i = 0; i < 10; ++i)
  {
    a.push(i * 10);
    BOOST_CHECK_EQUAL(a.top(), i * 10);
  }
}

BOOST_AUTO_TEST_CASE(StackPushPop)
{
  ivanova::Stack<int> a;

  a.push(10);
  BOOST_CHECK_EQUAL(a.top(), 10);
  a.pop();
  BOOST_CHECK(a.empty());

  for (int i = 1; i <= 5; ++i)
  {
    a.push(i);
    BOOST_CHECK_EQUAL(a.top(), i);
  }
  for (int i = 1; i <= 5; ++i)
  {
    BOOST_CHECK_EQUAL(a.top(), 6 - i);
    a.pop();
  }
  BOOST_CHECK(a.empty());
}
