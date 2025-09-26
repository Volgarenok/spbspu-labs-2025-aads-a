#include <boost/test/unit_test.hpp>
#include "expression-queue.hpp"

BOOST_AUTO_TEST_CASE(queue_constructors_and_assignment_operators)
{
  guseynov::Queue< int > originalQueue;
  originalQueue.push(1);
  originalQueue.push(2);
  guseynov::Queue< int > copiedQueue = originalQueue;
  BOOST_TEST(originalQueue.front() == copiedQueue.front());
  BOOST_TEST(originalQueue.size() == copiedQueue.size());

  guseynov::Queue< int > assignedQueue;
  assignedQueue = originalQueue;
  BOOST_TEST(originalQueue.front() == assignedQueue.front());
  originalQueue.pop();
  assignedQueue.pop();
  BOOST_TEST(originalQueue.front() == assignedQueue.front());
  BOOST_TEST(originalQueue.size() == assignedQueue.size());
}

BOOST_AUTO_TEST_CASE(queue_operations)
{
  guseynov::Queue< int > testQueue;
  BOOST_TEST(testQueue.empty());
  testQueue.push(1);
  testQueue.push(2);
  BOOST_TEST(!testQueue.empty());
  BOOST_TEST(testQueue.size() == 2);
  BOOST_TEST(testQueue.front() == 1);
  testQueue.pop();
  BOOST_TEST(testQueue.front() == 2);
  testQueue.pop();
  BOOST_TEST(testQueue.empty());
}
