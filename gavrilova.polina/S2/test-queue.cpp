#include <boost/test/unit_test.hpp>
#include <Queue.hpp>

BOOST_AUTO_TEST_CASE(TestElementAccessQueue)
{
  gavrilova::Queue< int > queue{};
  queue.push(1);
  queue.push(2);
  queue.push(1 + 2);
  BOOST_TEST(queue.front() == 1);
}

BOOST_AUTO_TEST_CASE(TestConstructorsQueue)
{
  gavrilova::Queue< int > queue1{};
  queue1.push(1);
  queue1.push(2);
  queue1.push(1 + 2);
  gavrilova::Queue< int > queue2 = queue1;
  BOOST_TEST(queue1.front() == 1);
  BOOST_TEST(queue1.size() == queue2.size());
}

BOOST_AUTO_TEST_CASE(TestAssignmentOperatorsQueue)
{
  gavrilova::Queue< int > queue1;
  queue1.push(1);
  queue1.push(2);
  queue1.push(1 + 2);
  gavrilova::Queue< int > queue2;
  queue2 = queue1;
  BOOST_TEST(queue1.front() == 1);
  BOOST_TEST(queue2.front() == 1);
  BOOST_TEST(queue1.size() == queue2.size());
}

BOOST_AUTO_TEST_CASE(TestPopQueue)
{
  gavrilova::Queue< int > queue;
  queue.push(1);
  queue.push(2);
  queue.push(1 + 2);
  queue.pop();
  BOOST_TEST(queue.front() == 2);
  queue.pop();
  BOOST_TEST(queue.front() == 3);
}

BOOST_AUTO_TEST_CASE(TestEmptyQueue)
{
  gavrilova::Queue< int > queue;
  BOOST_TEST(queue.empty());
  BOOST_TEST(queue.size() == 0);
}

BOOST_AUTO_TEST_CASE(TestPushFrontQueue)
{
  gavrilova::Queue< int > queue;
  queue.push(1);
  BOOST_TEST(queue.front() == 1);
  queue.push(2);
  BOOST_TEST(queue.front() == 1);
}

BOOST_AUTO_TEST_CASE(TestMoveSemanticsQueue)
{
  gavrilova::Queue< std::string > queue1;
  queue1.push("test");
  gavrilova::Queue< std::string > queue2 = std::move(queue1);
  BOOST_TEST(queue2.front() == "test");
  BOOST_TEST(queue1.empty());
}

BOOST_AUTO_TEST_CASE(TestSwapQueue)
{
  gavrilova::Queue< int > queue1;
  queue1.push(1);
  queue1.push(2);

  gavrilova::Queue< int > queue2;
  queue2.push(3);

  queue1.swap(queue2);

  BOOST_TEST(queue1.front() == 3);
  BOOST_TEST(queue1.size() == 1);
  BOOST_TEST(queue2.front() == 1);
  BOOST_TEST(queue2.size() == 2);
}

BOOST_AUTO_TEST_CASE(TestPopEmptyQueue)
{
  gavrilova::Queue< int > queue;
  BOOST_CHECK_THROW(queue.pop(), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(TestFrontEmptyQueue)
{
  gavrilova::Queue< int > queue;
  BOOST_CHECK_THROW(queue.front(), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(TestClearQueue)
{
  gavrilova::Queue< int > queue;
  queue.push(1);
  queue.push(2);
  queue.clear();
  BOOST_TEST(queue.empty());
  BOOST_TEST(queue.size() == 0);
  BOOST_CHECK_THROW(queue.front(), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(TestLargeQueue)
{
  gavrilova::Queue< int > queue;
  for (int i = 0; i < 1000; ++i) {
    queue.push(i);
  }
  BOOST_TEST(queue.size() == 1000);
  for (int i = 0; i < 1000; ++i) {
    BOOST_TEST(queue.front() == i);
    queue.pop();
  }
  BOOST_TEST(queue.empty());
}
