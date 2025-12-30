#include <boost/test/unit_test.hpp>
#include "stack.hpp"
BOOST_AUTO_TEST_CASE(stackDefaultConstrunct_test)
{
  nikonov::Stack< std::string > st;
  BOOST_TEST(st.empty());
}
BOOST_AUTO_TEST_CASE(stackLCopyConstrunct_test)
{
  nikonov::Stack< std::string > st;
  st.push("a");
  st.push("b");
  nikonov::Stack< std::string > copy(st);
  BOOST_TEST(copy.size() == 2);
  BOOST_TEST(copy.top() == "b");
  copy.pop();
  BOOST_TEST(copy.top() == "a");
}
BOOST_AUTO_TEST_CASE(stackRCopyConstrunct_test)
{
  nikonov::Stack< std::string > st;
  st.push("a");
  st.push("b");
  nikonov::Stack< std::string > copy(std::move(st));
  BOOST_TEST(st.size() == 0);
  BOOST_TEST(copy.size() == 2);
  BOOST_TEST(copy.size() == 2);
  BOOST_TEST(copy.top() == "b");
  copy.pop();
  BOOST_TEST(copy.top() == "a");
}
BOOST_AUTO_TEST_CASE(stackEmpty_test)
{
  nikonov::Stack< std::string > st;
  BOOST_TEST(st.empty());
  st.push("a");
  BOOST_TEST(!st.empty());
}
BOOST_AUTO_TEST_CASE(stackSize_test)
{
  nikonov::Stack< std::string > st;
  st.push("a");
  BOOST_TEST(st.size() == 1);
  st.push("b");
  BOOST_TEST(st.size() == 2);
  st.pop();
  BOOST_TEST(st.size() == 1);
}
BOOST_AUTO_TEST_CASE(stackPush_test)
{
  nikonov::Stack< std::string > st;
  st.push("a");
  BOOST_TEST(st.size() == 1);
  BOOST_TEST(st.top() == "a");
  st.push("b");
  st.push("c");
  st.push("d");
  st.push("e");
  st.push("f");
  st.push("g");
  st.push("h");
  st.push("i");
  st.push("j");
  st.push("k");
  BOOST_TEST(st.top() == "k");
  BOOST_TEST(st.size() == 11);
}
BOOST_AUTO_TEST_CASE(stackPop_test)
{
  nikonov::Stack< std::string > st;
  st.push("a");
  st.push("b");
  st.push("c");
  BOOST_TEST(st.size() == 3);
  st.pop();
  BOOST_TEST(st.size() == 2);
  BOOST_TEST(st.top() == "b");
}
BOOST_AUTO_TEST_CASE(stackSwap_test)
{
  nikonov::Stack< std::string > st;
  st.push("a");
  st.push("b");
  st.push("c");
  nikonov::Stack< std::string > copy;
  copy.swap(st);
  BOOST_TEST(st.size() == 0);
  BOOST_TEST(copy.size() == 3);
  BOOST_TEST(copy.top() == "c");
  copy.pop();
  BOOST_TEST(copy.top() == "b");
  copy.pop();
  BOOST_TEST(copy.top() == "a");
}
