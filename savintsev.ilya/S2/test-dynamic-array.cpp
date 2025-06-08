#include <boost/test/unit_test.hpp>
#include "dynamic-array.hpp"

using namespace savintsev;

BOOST_AUTO_TEST_CASE(test_constr)
{
  Array< int > m(2);

  BOOST_TEST(m.size() == 0);
  m.push_back(123);
  BOOST_TEST(m.back() == 123);
  BOOST_TEST(m.size() == 1);
  m.push_back(456);
  BOOST_TEST(m.back() == 456);
  BOOST_TEST(m.front() == 123);
  m.push_back(789);
  BOOST_TEST(m.back() == 789);
  BOOST_TEST(m.front() == 123);
  BOOST_TEST(m.size() == 3);
  m.pop_front();
  BOOST_TEST(m.front() == 456);
  BOOST_TEST(m.size() == 2);
  m.push_back(999);
  BOOST_TEST(m.back() == 999);
  m.pop_back();
  BOOST_TEST(m.back() == 789);

  Array< int > n(m);

  BOOST_TEST(n.front() == 456);
  BOOST_TEST(n.back() == 789);
}
