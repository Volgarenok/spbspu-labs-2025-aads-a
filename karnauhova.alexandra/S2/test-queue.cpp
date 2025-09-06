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
  BOOST_TEST(queue.back() == 1);
  queue.push(2);
  BOOST_TEST(queue.back() == 2);
  BOOST_TEST(queue.size() == 2);
  queue.pop();
  BOOST_TEST(queue.size() == 1);
  BOOST_TEST(queue.back() == 2);
  queue.pop();
  BOOST_TEST(queue.size() == 0);
}

BOOST_AUTO_TEST_CASE(test_back_front_queue)
{
  karnauhova::Queue< int > queue;
  queue.push(1);
  queue.push(2);
  queue.push(3);
  int i1 = queue.back();
  const int i2 = queue.back();
  int i3 = queue.front();
  const int i4 = queue.front();
  BOOST_TEST(i1 == 3);
  BOOST_TEST(i2 == 3);
  BOOST_TEST(i3 == 1);
  BOOST_TEST(i4 == 1);
}

BOOST_AUTO_TEST_CASE(test_constructors_and_operator_queue)
{
  karnauhova::Queue<int> queue;
  queue.push(2);

  karnauhova::Queue<int> copyqueue(queue);
  BOOST_TEST(copyqueue.size() == 1);
  BOOST_TEST(copyqueue.back() == 2);

  karnauhova::Queue<int> movequeue(std::move(queue));
  BOOST_TEST(movequeue.size() == 1);
  BOOST_TEST(movequeue.back() == 2);
  BOOST_TEST(queue.empty());

  karnauhova::Queue<int> another_queue;
  another_queue.push(3);
  another_queue.push(4);
  copyqueue = another_queue;
  BOOST_TEST(copyqueue.size() == 2);
  BOOST_TEST(copyqueue.back() == 4);
  BOOST_TEST(another_queue.size() == 2);
  another_queue = another_queue;
  BOOST_TEST(another_queue.size() == 2);
  BOOST_TEST(another_queue.back() == 4);
}

BOOST_AUTO_TEST_CASE(test_swap_queue)
{
  karnauhova::Queue<int> queue1;
  queue1.push(1);
  queue1.push(2);
  queue1.push(3);
  karnauhova::Queue<int> queue2;
  queue2.push(1);
  queue2.push(2);
  queue1.swap(queue2);
  BOOST_TEST(queue1.size() == 2);
  BOOST_TEST(queue1.back() == 2);
  BOOST_TEST(queue2.size() == 3);
  BOOST_TEST(queue2.back() == 3);
}
