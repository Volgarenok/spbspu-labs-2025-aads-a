#include <boost/test/unit_test.hpp>
#include <sstream>
#include <queue.hpp>

namespace
{
  std::ostream & printQueue(std::ostream & out, maslov::Queue< int > queue)
  {
    while (!queue.empty())
    {
      out << queue.front();
      queue.pop();
    }
    return out;
  }
}

BOOST_AUTO_TEST_CASE(defaultConstructorQueue)
{
  maslov::Queue< int > queue;
  BOOST_TEST(queue.empty());
}

BOOST_AUTO_TEST_CASE(copyConstructorQueue)
{
  maslov::Queue< int > queue;
  queue.push(1);
  queue.push(2);
  maslov::Queue< int > copyQueue(queue);
  std::ostringstream out;
  printQueue(out, copyQueue);
  BOOST_TEST(out.str() == "12");
  BOOST_TEST(queue.size() == 2);
}

BOOST_AUTO_TEST_CASE(moveConstructorQueue)
{
  maslov::Queue< int > queue;
  queue.push(1);
  queue.push(2);
  maslov::Queue< int > moveQueue(std::move(queue));
  std::ostringstream out;
  printQueue(out, moveQueue);
  BOOST_TEST(out.str() == "12");
  BOOST_TEST(queue.empty());
}

BOOST_AUTO_TEST_CASE(copyOperatorQueue)
{
  maslov::Queue< int > queue;
  queue.push(1);
  queue.push(2);
  maslov::Queue< int > copyQueue;
  copyQueue = queue;
  std::ostringstream out;
  printQueue(out, copyQueue);
  BOOST_TEST(out.str() == "12");
  BOOST_TEST(queue.size() == 2);
}

BOOST_AUTO_TEST_CASE(moveOperatorQueue)
{
  maslov::Queue< int > queue;
  queue.push(1);
  queue.push(2);
  maslov::Queue< int > moveQueue;
  moveQueue = std::move(queue);
  std::ostringstream out;
  printQueue(out, moveQueue);
  BOOST_TEST(out.str() == "12");
  BOOST_TEST(queue.empty());
}

BOOST_AUTO_TEST_CASE(pushFrontQueue)
{
  maslov::Queue< int > queue;
  queue.push(1);
  queue.push(2);
  BOOST_TEST(queue.front() == 1);
  BOOST_TEST(!queue.empty());
}

BOOST_AUTO_TEST_CASE(popQueue)
{
  maslov::Queue< int > queue;
  queue.push(1);
  queue.push(2);
  queue.pop();
  BOOST_TEST(queue.front() == 2);
}

BOOST_AUTO_TEST_CASE(swapQueue)
{
  maslov::Queue< int > queue;
  queue.push(1);
  queue.push(2);
  maslov::Queue< int > otherQueue;
  queue.swap(otherQueue);
  std::ostringstream out;
  printQueue(out, otherQueue);
  BOOST_TEST(queue.empty());
  BOOST_TEST(out.str() == "12");
}
