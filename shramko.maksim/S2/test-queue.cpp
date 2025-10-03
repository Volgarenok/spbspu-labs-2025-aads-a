#include <boost/test/unit_test.hpp>
#include <sstream>
#include "queue.hpp"

namespace shramko
{
  void outputQueue(std::ostream& out, Queue< std::string >& queue)
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
  shramko::Queue< std::string > que;
  BOOST_TEST(que.empty());
  BOOST_TEST(que.size() == 0);
}

BOOST_AUTO_TEST_CASE(queue_pop_test)
{
  shramko::Queue< std::string > que;
  que.push("1");
  que.push("2");
  que.push("3");
  que.push("4");
  que.push("5");
  BOOST_TEST(que.size() == 5);
  que.pop();
  BOOST_TEST(que.size() == 4);
  std::ostringstream out;
  shramko::outputQueue(out, que);
  BOOST_TEST(out.str() == "2 3 4 5");
}

BOOST_AUTO_TEST_CASE(queue_front_size_test)
{
  shramko::Queue< std::string > que;
  que.push("2");
  que.push("3");
  BOOST_TEST(que.front() == "2");
  BOOST_TEST(que.size() == 2);
}

BOOST_AUTO_TEST_CASE(queue_move_push_front_test)
{
  shramko::Queue< std::string > que;
  que.push("1");
  BOOST_TEST(!que.empty());
  BOOST_TEST(que.size() == 1);
  BOOST_TEST(que.front() == "1");
  que.push("2");
  BOOST_TEST(que.front() == "1");
  BOOST_TEST(que.size() == 2);
}
