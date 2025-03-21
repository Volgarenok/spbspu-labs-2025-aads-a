#include <boost/test/unit_test.hpp>
#include <sstream>
#include "queue.hpp"

using namespace tkach;
BOOST_AUTO_TEST_CASE(basic_constructor_test)
{
  Queue< int > que;
  BOOST_TEST(que.empty());
  BOOST_TEST(que.size() == 0);
}

BOOST_AUTO_TEST_CASE(queue_move_push_front_test)
{
  Queue< int > que;
  que.push(1);
  BOOST_TEST(!que.empty());
  BOOST_TEST(que.size() == 1);
  BOOST_TEST(que.front() == 1);
  que.push(2);
  BOOST_TEST(que.front() == 1);
  BOOST_TEST(que.back() == 2);
  BOOST_TEST(que.size() == 2);
}
