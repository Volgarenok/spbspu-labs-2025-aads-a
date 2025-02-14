#include <boost/test/unit_test.hpp>
#include <string>
#include "list.hpp"

BOOST_AUTO_TEST_CASE(list_test)
{
  std::string out;
  savintsev::List<std::string> list;
  list.push_back("test1234");
  BOOST_TEST(!list.empty());
  BOOST_TEST(list.size() == 1);
  list.pop_front();
  BOOST_TEST(list.size() == 0);
  list.push_back("test1234");
  list.push_front("test8765");
  list.clear();
  BOOST_TEST(list.empty());
  list.push_front("test");
  BOOST_TEST(list.front() == "test");
  list.push_back("hello");
  BOOST_TEST(list.back() == "hello");
}
