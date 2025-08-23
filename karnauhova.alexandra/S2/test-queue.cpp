#include <boost/test/unit_test.hpp>
#include "queue.hpp"

BOOST_AUTO_TEST_CASE(test_capacity_queue)
{
  karnauhova::Queue< int > queue;
  BOOST_TEST(queue.empty() == true);
  BOOST_TEST(queue.size() == 0);
  queue.push(2);
  BOOST_TEST(queue.size() == 1);
}

BOOST_AUTO_TEST_CASE(test_pop_and_push_queue)
{
  karnauhova::Queue< int > queue;
  queue.push(1);
  BOOST_TEST(queue.front() == 1);
  queue.push(2);
  BOOST_TEST(queue.size() == 2);
  queue.pop();
  BOOST_TEST(queue.size() == 1);
  BOOST_TEST(queue.front() == 2);
  queue.pop();
  BOOST_TEST(queue.size() == 0);
}

BOOST_AUTO_TEST_CASE(test_front_front_queue)
{
  karnauhova::Queue< int > queue;
  queue.push(1);
  queue.push(2);
  queue.push(3);
  int i1 = queue.front();
  const int i2 = queue.front();
  BOOST_TEST(i1 == 1);
  BOOST_TEST(i2 == 1);
}

BOOST_AUTO_TEST_CASE(test_constructors_and_operator_queue)
{
  karnauhova::Queue< int > queue;
  queue.push(2);

  karnauhova::Queue< int > copyqueue(queue);
  BOOST_TEST(copyqueue.size() == 1);
  BOOST_TEST(copyqueue.front() == 2);

  karnauhova::Queue< int > movequeue(std::move(queue));
  BOOST_TEST(movequeue.size() == 1);
  BOOST_TEST(movequeue.front() == 2);
  BOOST_TEST(queue.empty());

  karnauhova::Queue< int > another_queue;
  another_queue.push(3);
  another_queue.push(4);
  copyqueue = another_queue;
  BOOST_TEST(copyqueue.size() == 2);
  BOOST_TEST(copyqueue.front() == 3);
  BOOST_TEST(another_queue.size() == 2);
  another_queue = another_queue;
  BOOST_TEST(another_queue.size() == 2);
  BOOST_TEST(another_queue.front() == 3);
}

BOOST_AUTO_TEST_CASE(test_swap_queue)
{
  karnauhova::Queue< int > queue1;
  queue1.push(5);
  queue1.push(2);
  queue1.push(3);
  karnauhova::Queue< int > queue2;
  queue2.push(6);
  queue2.push(2);
  queue1.swap(queue2);
  BOOST_TEST(queue1.size() == 2);
  BOOST_TEST(queue1.front() == 6);
  BOOST_TEST(queue2.size() == 3);
  BOOST_TEST(queue2.front() == 5);
}
