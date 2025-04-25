#include <boost/test/unit_test.hpp>
#include <sstream>
#include "../common/stack.hpp"

BOOST_AUTO_TEST_SUITE(stack_tests)

BOOST_AUTO_TEST_CASE(construct_test)
{
  evstyunichev::Stack< int > s1{};
}

BOOST_AUTO_TEST_CASE(push_pop_tests)
{
  evstyunichev::Stack< int > s1{};
  s1.push(34);
  s1.push(52);
  s1.pop();
}

BOOST_AUTO_TEST_CASE(top_test)
{
  std::stringstream ss;
  evstyunichev::Stack< int > s1{};
  for (int i = 1; i <= 6; i++)
  {
    s1.push(i * 2 + 1);
    s1.push(i * 2);
    s1.pop();
  }
  BOOST_CHECK(s1.top() == 13);
  s1.push(52);
  BOOST_CHECK(s1.size() == 7);
  ss << s1.top();
  while (s1.size() > 1)
  {
    s1.pop();
    ss << ' ' << s1.top();
  }
  BOOST_CHECK(ss.str() == "52 13 11 9 7 5 3");
}

BOOST_AUTO_TEST_SUITE_END()
