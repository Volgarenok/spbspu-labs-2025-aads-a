#include <boost/test/unit_test.hpp>
#include "queue.hpp"
#include "mem_checker.hpp"

BOOST_AUTO_TEST_SUITE(S2_queue_test)

BOOST_AUTO_TEST_CASE(empty_constructor_test)
{
  rychkov::Queue< int > queue;
  BOOST_TEST(queue.empty());
  BOOST_TEST(queue.size() == 0);
}
BOOST_AUTO_TEST_CASE(push_test)
{
  rychkov::Queue< int > queue;
  queue.push(3);
  queue.push(9);
  BOOST_TEST(!queue.empty());
  BOOST_TEST(queue.size() == 2);
  BOOST_TEST(queue.capacity() >= 2);
  BOOST_TEST(queue.front() == 3);
  BOOST_TEST(queue.back() == 9);
}
BOOST_AUTO_TEST_CASE(copy_test)
{
  rychkov::Queue< int > queue1;
  queue1.push(3);
  queue1.push(9);
  rychkov::Queue< int > queue2 = queue1;
  BOOST_TEST(queue2.size() == queue1.size());
  BOOST_TEST(queue2.capacity() >= queue1.size());
  BOOST_TEST(queue2.front() == queue1.front());
  BOOST_TEST(queue2.back() == queue1.back());
}
BOOST_AUTO_TEST_CASE(move_test)
{
  rychkov::Queue< int > queue1;
  queue1.push(3);
  queue1.push(9);
  rychkov::Queue< int > queue2 = std::move(queue1);
  BOOST_TEST(queue2.size() == 2);
  BOOST_TEST(queue2.capacity() >= queue2.size());
  BOOST_TEST(queue2.front() == 3);
  BOOST_TEST(queue2.back() == 9);
}
BOOST_AUTO_TEST_CASE(pop_test)
{
  rychkov::Queue< int > queue;
  queue.push(3);
  queue.push(9);
  queue.push(27);
  queue.pop();
  BOOST_TEST(queue.front() == 9);
  BOOST_TEST(queue.back() == 27);
  BOOST_TEST(queue.size() == 2);
  queue.pop();
  BOOST_TEST(queue.front() == 27);
  BOOST_TEST(queue.back() == 27);
  BOOST_TEST(queue.size() == 1);
  queue.pop();
  BOOST_TEST(queue.empty());
}
BOOST_AUTO_TEST_CASE(destructors_leak_test)
{
  struct A
  {};
  rychkov::MemTrack< A > observer{};
  rychkov::Queue< rychkov::MemChecker< A > > queue;
  queue.emplace();
  queue.emplace();
  queue.pop();
  queue.emplace();
  queue.emplace();
  queue.emplace();
  queue.pop();
  queue.pop();
}

BOOST_AUTO_TEST_SUITE_END()
