#include <boost/test/unit_test.hpp>
#include "queue.hpp"

using namespace kushekbaev;

BOOST_AUTO_TEST_SUITE(queueConstructors)

BOOST_AUTO_TEST_CASE(queueCopyConstructor)
{
  Queue< int > queue;
  queue.push(10);
  queue.push(20);
  Queue< int > Qcopy(queue);
  BOOST_TEST(Qcopy.front() == 10);
  BOOST_TEST(Qcopy.back() == 20);
  BOOST_TEST(Qcopy.size() == 2);
  BOOST_TEST(queue.back() == Qcopy.back());
}

BOOST_AUTO_TEST_CASE(queueMoveConstructor)
{
  Queue< int > queue;
  queue.push(10);
  queue.push(20);
  Queue< int > Qcopy(std::move(queue));
  BOOST_TEST(Qcopy.front() == 10);
  BOOST_TEST(Qcopy.back() == 20);
  BOOST_TEST(Qcopy.size() == 2);
  BOOST_TEST(queue.empty());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_CASE(queueAssignment)
{
  Queue< int > Q1;
  Q1.push(10);
  Queue< int > Q2;
  Q2 = Q1;
  BOOST_TEST(Q2.front() == 10);
}

BOOST_AUTO_TEST_SUITE(queueElementAccess)

BOOST_AUTO_TEST_CASE(queueFront)
{
  Queue< int > queue;
  queue.push(10);
  BOOST_TEST(queue.front() == 10);
  queue.push(20);
  BOOST_TEST(queue.front() == 10);
}

BOOST_AUTO_TEST_CASE(queueBack)
{
  Queue< int > queue;
  queue.push(10);
  BOOST_TEST(queue.back() == 10);
  queue.push(20);
  BOOST_TEST(queue.back() == 20);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(queueCapacity)

BOOST_AUTO_TEST_CASE(queueEmpty)
{
  Queue< int > queue;
  BOOST_TEST(queue.empty());
  queue.push(10);
  BOOST_TEST(!queue.empty());
  queue.pop();
  BOOST_TEST(queue.empty());
}

BOOST_AUTO_TEST_CASE(queueSize)
{
  Queue< int > queue;
  queue.push(10);
  queue.push(20);
  BOOST_TEST(queue.size() == 2);
  queue.pop();
  BOOST_TEST(queue.size() == 1);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_CASE(queuePushPop)
{
  Queue< int > queue;
  queue.push(10);
  BOOST_TEST(queue.front() == 10);
  queue.push(20);
  BOOST_TEST(queue.back() == 20);
  queue.pop();
  queue.pop();
  BOOST_TEST(queue.empty());
}
