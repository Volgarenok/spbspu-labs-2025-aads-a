#include <boost/test/unit_test.hpp>
#include "queue.hpp"
BOOST_AUTO_TEST_CASE(queueDefaultConstrunct_test)
{
  nikonov::Queue< std::string > que;
  BOOST_TEST(que.empty());
}
BOOST_AUTO_TEST_CASE(queueLCopyConstrunct_test)
{
  nikonov::Queue< std::string > que;
  que.push("a");
  que.push("b");
  nikonov::Queue< std::string > copy(que);
  BOOST_TEST(copy.size() == 2);
  BOOST_TEST(que.front() == "a");
  BOOST_TEST(que.back() == "b");
}
BOOST_AUTO_TEST_CASE(queueRCopyConstrunct_test)
{
  nikonov::Queue< std::string > que;
  que.push("a");
  que.push("b");
  nikonov::Queue< std::string > copy(std::move(que));
  BOOST_TEST(que.size() == 0);
  BOOST_TEST(copy.size() == 2);
  BOOST_TEST(copy.front() == "a");
  BOOST_TEST(copy.back() == "b");
}
BOOST_AUTO_TEST_CASE(queueEmpty_test)
{
  nikonov::Queue< std::string > que;
  BOOST_TEST(que.empty());
  que.push("a");
  BOOST_TEST(!que.empty());
}
BOOST_AUTO_TEST_CASE(queueSize_test)
{
  nikonov::Queue< std::string > que;
  que.push("a");
  BOOST_TEST(que.size() == 1);
  que.push("b");
  BOOST_TEST(que.size() == 2);
  que.pop();
  BOOST_TEST(que.size() == 1);
}
BOOST_AUTO_TEST_CASE(queueFront_test)
{
  nikonov::Queue< std::string > que;
  que.push("a");
  BOOST_TEST(que.front() == "a");
  que.push("b");
  BOOST_TEST(que.front() == "a");
  que.pop();
  BOOST_TEST(que.front() == "b");
}
BOOST_AUTO_TEST_CASE(queueBack_test)
{
  nikonov::Queue< std::string > que;
  que.push("a");
  BOOST_TEST(que.back() == "a");
  que.push("b");
  BOOST_TEST(que.back() == "b");
}
BOOST_AUTO_TEST_CASE(queuePush_test)
{
  nikonov::Queue< std::string > que;
  que.push("a");
  BOOST_TEST(que.size() == 1);
  BOOST_TEST(que.front() == "a");
  que.push("a");
  que.push("a");
  que.push("a");
  que.push("a");
  que.push("a");
  que.push("a");
  que.push("a");
  que.push("a");
  que.push("a");
  que.push("a");
  BOOST_TEST(que.size() == 11);
}
BOOST_AUTO_TEST_CASE(queuePop_test)
{
  nikonov::Queue< std::string > que;
  que.push("a");
  que.push("b");
  que.push("c");
  BOOST_TEST(que.size() == 3);
  que.pop();
  BOOST_TEST(que.size() == 2);
  BOOST_TEST(que.front() == "b");
  BOOST_TEST(que.back() == "c");
}
BOOST_AUTO_TEST_CASE(queueSwap_test)
{
  nikonov::Queue< std::string > que;
  que.push("a");
  que.push("b");
  que.push("c");
  nikonov::Queue< std::string > copy;
  copy.swap(que);
  BOOST_TEST(que.size() == 0);
  BOOST_TEST(copy.size() == 3);
}
