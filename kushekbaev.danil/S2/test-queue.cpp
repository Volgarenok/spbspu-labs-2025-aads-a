#include <boost/test/unit_test.hpp>
#include <queue.hpp>

using namespace kushekbaev;

BOOST_AUTO_TEST_SUITE(queueConstructors)

BOOST_AUTO_TEST_CASE(queueCopyConstructor)
{
  Queue< int > queue;
  queue.push(10);
  queue.push(20);
  Queue< int > queuecopy(queue);
  BOOST_TEST(queuecopy.front() == 10);
  BOOST_TEST(queuecopy.back() == 20);
  BOOST_TEST(queuecopy.size() == 2);
  BOOST_TEST(queue.back() == queuecopy.back());
}

BOOST_AUTO_TEST_CASE(queueMoveConstructor)
{
  Queue< int > queue;
  queue.push(10);
  queue.push(20);
  Queue< int > queuecopy(std::move(queue));
  BOOST_TEST(queuecopy.front() == 10);
  BOOST_TEST(queuecopy.back() == 20);
  BOOST_TEST(queuecopy.size() == 2);
  BOOST_TEST(queue.empty());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_CASE(queueAssignment)
{
  Queue< int > queue1;
  queue1.push(10);
  Queue< int > queue2;
  queue2 = queue1;
  BOOST_TEST(queue2.front() == 10);
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
