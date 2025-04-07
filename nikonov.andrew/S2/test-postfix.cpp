#include <boost/test/unit_test.hpp>
#include "postfix.hpp"
#include "sstream"
#include <iostream>
BOOST_AUTO_TEST_CASE(postfixConstructor_test)
{
  nikonov::Postfix postfix("2 + 3", 0);
  std::stringstream str;
  str << postfix;
  BOOST_TEST(str.str() == "2 3 +");
}
BOOST_AUTO_TEST_CASE(postfixMathOperators_test)
{
  nikonov::Postfix postfix1("2 + 3", 0);
  nikonov::Postfix postfix2("4 + 5", 0);
  std::stringstream str1;
  str1 << (postfix1 + postfix2);
  BOOST_TEST(str1.str() == "2 3 + 4 5 + +");
  std::stringstream str2;
  str2 << (postfix1 - postfix2);
  BOOST_TEST(str2.str() == "2 3 + 4 5 + -");
  std::stringstream str3;
  str3 << (postfix1 * postfix2);
  BOOST_TEST(str3.str() == "2 3 + 4 5 + *");
  std::stringstream str4;
  str4 << (postfix1 / postfix2);
  BOOST_TEST(str4.str() == "2 3 + 4 5 + /");
  std::stringstream str5;
  str5 << (postfix1 % postfix2);
  BOOST_TEST(str5.str() == "2 3 + 4 5 + %");
}
