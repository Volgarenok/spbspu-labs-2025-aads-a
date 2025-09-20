#include <boost/test/unit_test.hpp>
#include "queue.hpp"

using namespace kushekbaev;

BOOST_AUTO_TEST_SUITE(queueConstructors)

BOOST_AUTO_TEST_CASE(queueCopyConstructor)
{
  Queue< int > Q;
  Q.push(10);
  Q.push(20);
  Queue< int > Qcopy(Q);
  BOOST_TEST(Qcopy.front() == 10);
  BOOST_TEST(Qcopy.back() == 20);
  BOOST_TEST(Qcopy.size() == 2);
  BOOST_TEST(Q.back() == Qcopy.back());
}

BOOST_AUTO_TEST_CASE(queueMoveConstructor)
{
  Queue< int > Q;
  Q.push(10);
  Q.push(20);
  Queue< int > Qcopy(std::move(Q));
  BOOST_TEST(Qcopy.front() == 10);
  BOOST_TEST(Qcopy.back() == 20);
  BOOST_TEST(Qcopy.size() == 2);
  BOOST_TEST(Q.empty());
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
  Queue< int > Q;
  Q.push(10);
  BOOST_TEST(Q.front() == 10);
  Q.push(20);
  BOOST_TEST(Q.front() == 10);
}

BOOST_AUTO_TEST_CASE(queueBack)
{
  Queue< int > Q;
  Q.push(10);
  BOOST_TEST(Q.back() == 10);
  Q.push(20);
  BOOST_TEST(Q.back() == 20);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(queueCapacity)

BOOST_AUTO_TEST_CASE(queueEmpty)
{
  Queue< int > Q;
  BOOST_TEST(Q.empty());
  Q.push(10);
  BOOST_TEST(!Q.empty());
  Q.pop();
  BOOST_TEST(Q.empty());
}

BOOST_AUTO_TEST_CASE(queueSize)
{
  Queue< int > Q;
  Q.push(10);
  Q.push(20);
  BOOST_TEST(Q.size() == 2);
  Q.pop();
  BOOST_TEST(Q.size() == 1);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_CASE(queuePushPop)
{
  Queue< int > Q;
  Q.push(10);
  BOOST_TEST(Q.front() == 10);
  Q.push(20);
  BOOST_TEST(Q.back() == 20);
  Q.pop();
  Q.pop();
  BOOST_TEST(Q.empty());
}
