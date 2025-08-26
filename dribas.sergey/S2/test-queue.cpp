#include <boost/test/unit_test.hpp>
#include <queue.hpp>

BOOST_AUTO_TEST_SUITE(QueueTests)

BOOST_AUTO_TEST_CASE(DefaultConstructorTest)
{
  dribas::Queue< int > queue;
  BOOST_CHECK(queue.empty());
  BOOST_CHECK_EQUAL(queue.size(), 0);
}

BOOST_AUTO_TEST_CASE(PushAndbackTest)
{
  dribas::Queue< int > queue;
  queue.push(10);
  BOOST_CHECK_EQUAL(queue.back(), 10);
  BOOST_CHECK_EQUAL(queue.size(), 1);

  queue.push(20);
  BOOST_CHECK_EQUAL(queue.back(), 10);
  BOOST_CHECK_EQUAL(queue.size(), 2);
}

BOOST_AUTO_TEST_CASE(PopTest)
{
  dribas::Queue< int > queue;
  queue.push(10);
  queue.push(20);

  queue.pop();
  BOOST_CHECK_EQUAL(queue.back(), 20);
  BOOST_CHECK_EQUAL(queue.size(), 1);

  queue.pop();
  BOOST_CHECK(queue.empty());
}

BOOST_AUTO_TEST_CASE(CopyConstructorTest)
{
  dribas::Queue< int > queue1;
  queue1.push(10);
  queue1.push(20);

  dribas::Queue< int > queue2(queue1);
  BOOST_CHECK_EQUAL(queue2.size(), 2);
  BOOST_CHECK_EQUAL(queue2.back(), 10);

  queue2.pop();
  BOOST_CHECK_EQUAL(queue2.back(), 20);
  BOOST_CHECK_EQUAL(queue1.size(), 2);
}

BOOST_AUTO_TEST_CASE(MoveConstructorTest)
{
  dribas::Queue< int > queue1;
  queue1.push(10);
  queue1.push(20);

  dribas::Queue< int > queue2(std::move(queue1));
  BOOST_CHECK_EQUAL(queue2.size(), 2);
  BOOST_CHECK_EQUAL(queue2.back(), 10);
  BOOST_CHECK(queue1.empty());
}

BOOST_AUTO_TEST_CASE(CopyAssignmentTest)
{
  dribas::Queue< int > queue1;
  queue1.push(10);
  queue1.push(20);

  dribas::Queue< int > queue2;
  queue2 = queue1;
  BOOST_CHECK_EQUAL(queue2.size(), 2);
  BOOST_CHECK_EQUAL(queue2.back(), 10);

  queue2.pop();
  BOOST_CHECK_EQUAL(queue1.size(), 2);
}

BOOST_AUTO_TEST_CASE(MoveAssignmentTest)
{
  dribas::Queue< int > queue1;
  queue1.push(10);
  queue1.push(20);

  dribas::Queue< int > queue2;
  queue2 = std::move(queue1);
  BOOST_CHECK_EQUAL(queue2.size(), 2);
  BOOST_CHECK_EQUAL(queue2.back(), 10);
  BOOST_CHECK(queue1.empty());
}

BOOST_AUTO_TEST_CASE(SwapTest)
{
  dribas::Queue< int > queue1;
  queue1.push(10);

  dribas::Queue< int > queue2;
  queue2.push(20);
  queue2.push(30);

  queue1.swap(queue2);
  BOOST_CHECK_EQUAL(queue1.size(), 2);
  BOOST_CHECK_EQUAL(queue1.back(), 20);
  BOOST_CHECK_EQUAL(queue2.size(), 1);
  BOOST_CHECK_EQUAL(queue2.back(), 10);
}

BOOST_AUTO_TEST_SUITE_END()
