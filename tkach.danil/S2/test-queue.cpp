#include <boost/test/unit_test.hpp>
#include <sstream>
#include "queue.hpp"

using namespace tkach;

namespace
{
  template< typename T >
  void outputQueue(std::ostream& out, Queue< T > queue)
  {
    if (queue.empty())
    {
      return;
    }
    out << queue.front();
    queue.pop();
    while (!queue.empty())
    {
      out << " " << queue.front();
      queue.pop();
    }
  }
}

BOOST_AUTO_TEST_CASE(queue_basic_constructor_test)
{
  Queue< int > que;
  BOOST_TEST(que.empty());
  BOOST_TEST(que.size() == 0);
}

BOOST_AUTO_TEST_CASE(queue_copy_constructor_test)
{
  Queue< size_t > que;
  for (size_t i = 1; i <= 5; ++i)
  {
    que.push(i);
  }
  Queue< size_t > que2(que);
  std::ostringstream out;
  outputQueue(out, que2);
  BOOST_TEST(out.str() == "1 2 3 4 5");
}

BOOST_AUTO_TEST_CASE(queue_move_constructor_test)
{
  Queue< size_t > que;
  for (size_t i = 1; i <= 5; ++i)
  {
    que.push(i);
  }
  Queue< size_t > que2(std::move(que));
  std::ostringstream out;
  outputQueue(out, que2);
  BOOST_TEST(out.str() == "1 2 3 4 5");
}

BOOST_AUTO_TEST_CASE(queue_equal_operator_test)
{
  Queue< size_t > que;
  for (size_t i = 1; i <= 5; ++i)
  {
    que.push(i);
  }
  Queue< size_t > que2;
  que2 = que;
  std::ostringstream out;
  outputQueue(out, que2);
  BOOST_TEST(out.str() == "1 2 3 4 5");
}

BOOST_AUTO_TEST_CASE(queue_pop_test)
{
  Queue< size_t > que;
  for (size_t i = 1; i <= 5; ++i)
  {
    que.push(i);
  }
  BOOST_TEST(que.size() == 5);
  que.pop();
  BOOST_TEST(que.size() == 4);
  std::ostringstream out;
  outputQueue(out, que);
  BOOST_TEST(out.str() == "2 3 4 5");
}

BOOST_AUTO_TEST_CASE(queue_front_size_test)
{
  Queue< size_t > que;
  que.push(2);
  que.push(3);
  BOOST_TEST(que.front() == 2);
  BOOST_TEST(que.size() == 2);
}

BOOST_AUTO_TEST_CASE(queue_move_equal_operator_test)
{
  Queue< size_t > que;
  for (size_t i = 1; i <= 5; ++i)
  {
    que.push(i);
  }
  Queue< size_t > que2;
  que2 = std::move(que);
  std::ostringstream out;
  outputQueue(out, que2);
  BOOST_TEST(que.empty());
  BOOST_TEST(out.str() == "1 2 3 4 5");
}

BOOST_AUTO_TEST_CASE(queue_swap_test)
{
  Queue< size_t > que;
  for (size_t i = 1; i <= 5; ++i)
  {
    que.push(i);
  }
  Queue< size_t > que2;
  que.swap(que2);
  std::ostringstream out;
  outputQueue(out, que2);
  BOOST_TEST(que.empty());
  BOOST_TEST(out.str() == "1 2 3 4 5");
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
  BOOST_TEST(que.size() == 2);
}
