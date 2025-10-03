#include <boost/test/unit_test.hpp>
#include "queue.hpp"

BOOST_AUTO_TEST_CASE(copy_constructor_queue)
{
  abramov::Queue< int > queue1;
  queue1.push(1);
  queue1.push(2);
  queue1.push(3);
  abramov::Queue< int > queue2(queue1);
  BOOST_TEST(queue2.size() == 3);
  BOOST_TEST(queue2.front() == 1);
}

BOOST_AUTO_TEST_CASE(operator_test_queue)
{
  abramov::Queue< int > queue1;
  queue1.push(1);
  queue1.push(2);
  queue1.push(3);
  abramov::Queue< int > queue2;
  queue2 = queue1;
  BOOST_TEST(queue2.size() == 3);
  BOOST_TEST(queue2.front() == 1);
}

BOOST_AUTO_TEST_CASE(push_queue)
{
  abramov::Queue< int > queue;
  queue.push(1);
  queue.push(2);
  BOOST_TEST(queue.front() == 1);
}

BOOST_AUTO_TEST_CASE(front_queue)
{
  abramov::Queue< std::string > queue;
  queue.push("b");
  queue.push("a");
  BOOST_TEST(queue.front() == "b");
}

BOOST_AUTO_TEST_CASE(pop_queue)
{
  abramov::Queue< int > queue;
  queue.push(3);
  queue.push(1);
  queue.push(2);
  queue.pop();
  BOOST_TEST(queue.front() == 1);
}

BOOST_AUTO_TEST_CASE(size_queue)
{
  abramov::Queue< int > queue;
  for (size_t i = 0; i < 202; ++i)
  {
    queue.push(i);
  }
  BOOST_TEST(queue.size() == 202);
}

BOOST_AUTO_TEST_CASE(empty_queue)
{
  abramov::Queue< int > queue;
  BOOST_TEST(queue.empty());
  queue.push(1);
  BOOST_TEST(!queue.empty());
}

BOOST_AUTO_TEST_CASE(swap_queue)
{
  abramov::Queue< int > queue1;
  queue1.push(2);
  queue1.push(1);
  abramov::Queue< int > queue2;
  queue2.push(1);
  queue2.push(2);
  queue1.swap(queue2);
  BOOST_TEST(queue1.front() == 1);
  BOOST_TEST(queue2.front() == 2);
}
