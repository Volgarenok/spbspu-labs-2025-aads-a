#include <boost/test/unit_test.hpp>
#include "queue.hpp"

BOOST_AUTO_TEST_CASE(queue_constructors_and_operators)
{
  mozhegova::Queue< int > queue1;
  queue1.push(1);
  queue1.push(2);
  mozhegova::Queue< int > queue2 = queue1;
  BOOST_TEST(queue1.front() == queue2.front());
  BOOST_TEST(queue1.back() == queue2.back());
  BOOST_TEST(queue1.size() == queue2.size());

  mozhegova::Queue< int > queue3;
  queue3 = queue1;
  BOOST_TEST(queue1.front() == queue3.front());
  BOOST_TEST(queue1.back() == queue3.back());
  BOOST_TEST(queue1.size() == queue3.size());
}

BOOST_AUTO_TEST_CASE(queue_metods)
{
  mozhegova::Queue< int > queue1;
  BOOST_TEST(queue1.empty());
  queue1.push(1);
  queue1.push(2);
  BOOST_TEST(!queue1.empty());
  BOOST_TEST(queue1.size() == 2);
  BOOST_TEST(queue1.front() == 1);
  BOOST_TEST(queue1.back() == 2);
  queue1.pop();
  BOOST_TEST(queue1.front() == 2);
  queue1.pop();
  BOOST_TEST(queue1.empty());
}

BOOST_AUTO_TEST_CASE(queue_resize)
{
  mozhegova::Queue< int > queue1;
  BOOST_TEST(queue1.size() == 0);
  for (int i = 0; i < 60; i++)
  {
    queue1.push(i);
  }
  BOOST_TEST(queue1.size() == 60);
}
