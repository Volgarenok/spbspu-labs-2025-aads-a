#include <boost/test/unit_test.hpp>
#include <string>
#include <stdexcept>
#include "postfix.hpp"
#include "postfixProcess.hpp"

BOOST_AUTO_TEST_CASE(test_operators)
{
  using namespace alymova;
  Postfix p1("2 + 2");
  Postfix p2("3 * 3");
  Postfix p3(p1 + p2);
  BOOST_TEST(p1() == 4);
  BOOST_TEST(p2() == 9);
  BOOST_TEST(p3() == p1() + p2());

  Postfix p4;
  p3 = p1 + p4;
  try
  {
    p3();
  }
  catch (const std::exception& e)
  {
    BOOST_TEST(p1() == 4);
  }

  p3 = p1 - p2;
  BOOST_TEST(p3() == -5);
  BOOST_TEST(p3() == p1() - p2());

  p3 = p1 * p2;
  BOOST_TEST(p3() == 36);
  BOOST_TEST(p3() == p1() * p2());

  Postfix p5("5 * 7");
  p3 = p5 / p1;
  BOOST_TEST(p3() == 8);
  BOOST_TEST(p3() == p5() / p1());

  p3 = p5 % p1;
  BOOST_TEST(p3() == 3);
  BOOST_TEST(p3() == mod(p5(), p1()));
}
