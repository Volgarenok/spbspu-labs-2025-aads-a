#include <boost/test/unit_test.hpp>
#include "queue.hpp"

BOOST_AUTO_TEST_CASE(queue_construction)
{
  aleksandrov::Queue< int > queue1;
  queue1.push(12);
  queue1.push(13);
  BOOST_TEST(queue1.size() == 2);
  aleksandrov::Queue< int > queue2(queue1);
  BOOST_TEST(queue2.size() == 2);
  BOOST_TEST(queue2.front() == 12);
  aleksandrov::Queue< int > queue3(std::move(queue1));
  BOOST_TEST(queue3.size() == 2);
  BOOST_TEST(queue3.front() == 12);
}

BOOST_AUTO_TEST_CASE(queue_copy_move)
{
  aleksandrov::Queue< int > queue1;
  queue1.push(1);
  queue1.push(2);
  aleksandrov::Queue< int > queue2;
  queue2 = queue1;
  BOOST_TEST(queue2.size() == 2);
  BOOST_TEST(queue2.front() == 1);
  queue2.push(3);
  aleksandrov::Queue< int > queue3;
  queue3 = std::move(queue2);
  BOOST_TEST(queue3.size() == 3);
  BOOST_TEST(queue3.front() == 1);
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

