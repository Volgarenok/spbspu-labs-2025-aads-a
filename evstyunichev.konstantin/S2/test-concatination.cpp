#include <boost/test/unit_test.hpp>
#include <sstream>
#include <string>
#include <queue>
#include "expression_transform.hpp"

namespace
{
  template< class T >
  std::stringstream & out_stack(evstyunichev::Stack< T > &s, std::stringstream &ss)
  {
    ss << s.top();
    while (s.size() > 1)
    {
      s.pop();
      ss << ' ' << s.top();
    }
    return ss;
  }

  template< class T >
  std::stringstream & out_queue(evstyunichev::Queue< T > &q, std::stringstream &ss)
  {
    ss << q.front();
    while (q.size() > 1)
    {
      q.pop();
      ss << ' ' << q.front();
    }
    return ss;
  }
}

BOOST_AUTO_TEST_SUITE(concatination_test)

BOOST_AUTO_TEST_CASE(simple_test)
{
  std::string s = "12 | 34";
  BOOST_TEST(evstyunichev::result(s) == 1234);
  s = "2 | 0";
  BOOST_TEST(evstyunichev::result(s) == 20);
  s = "2 | 10";
  BOOST_TEST(evstyunichev::result(s) == 210);
}

BOOST_AUTO_TEST_CASE(overflow_test)
{
  try
  {
    std::string s = "10000000000 | 100000000000";
    evstyunichev::result(s);
    BOOST_TEST(0);
  }
  catch (const std::overflow_error &e)
  {
    BOOST_TEST(1);
  }
}

BOOST_AUTO_TEST_CASE(strong_expr_test)
{
  std::string s = "10 + ( 2 | 2 * 2 ) / 6 | 52";
  BOOST_TEST(evstyunichev::result(s) == 1452);
}

BOOST_AUTO_TEST_CASE(bad_data_for_conc_1)
{
  try
  {
    std::string s = "0 | 100000000000";
    evstyunichev::result(s);
    BOOST_TEST(0);
  }
  catch (const std::logic_error &e)
  {
    BOOST_TEST(1);
  }
}

BOOST_AUTO_TEST_CASE(bad_data_for_conc_2)
{
  try
  {
    std::string s = "10 | ( 0 - 100000 )";
    evstyunichev::result(s);
    BOOST_TEST(0);
  }
  catch (const std::logic_error &e)
  {
    BOOST_TEST(1);
  }
}

BOOST_AUTO_TEST_SUITE_END()
