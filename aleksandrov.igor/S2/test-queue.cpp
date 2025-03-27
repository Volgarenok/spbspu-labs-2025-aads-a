#include <boost/test/unit_test.hpp>
#include "queue.hpp"

BOOST_AUTO_TEST_CASE(queue_construction)
{
  aleksandrov::Queue< int > queue;
  BOOST_TEST(queue.empty());
}

BOOST_AUTO_TEST_CASE(queue_push)
{
  aleksandrov::Queue< char > letters;
  letters.push('a');
  letters.push('b');
  letters.push('c');
  BOOST_TEST(letters.size() == 3);
  BOOST_TEST(letters.drop() == 'a');
  BOOST_TEST(letters.size() == 2);
}

BOOST_AUTO_TEST_CASE(queue_front_drop)
{
  aleksandrov::Queue< double > queue;
  queue.push(1.0);
  queue.push(2.0);
  queue.push(3.0);
  BOOST_TEST(queue.front() == 1.0);
  queue.drop();
  BOOST_TEST(queue.size() == 2);
  BOOST_TEST(queue.drop() == 2.0);
}

BOOST_AUTO_TEST_CASE(queue_size_empty)
{
  aleksandrov::Queue< float > queue;
  BOOST_TEST(queue.empty());
  BOOST_TEST(queue.size() == 0);
  queue.push(9.9f);
  queue.push(8.8f);
  queue.push(7.7f);
  BOOST_TEST(queue.empty() == 0);
  BOOST_TEST(queue.size() == 3);
}

