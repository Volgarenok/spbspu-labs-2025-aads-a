#include <boost/test/unit_test.hpp>
#include <string>
#include "postfix.hpp"
#include "io-infix.hpp"

namespace
{
  bool hasException(const std::string &expr);

  bool hasException(const std::string &expr)
  {
    bool hasExcept = false;
    sveshnikov::Queue< std::string > infix2 = sveshnikov::splitIntoTokens(expr);
    try
    {
      sveshnikov::Postfix postf2(infix2);
      postf2.calculate();
    }
    catch (const std::exception &e)
    {
      hasExcept = true;
    }
    return hasExcept;
  }
}

BOOST_AUTO_TEST_SUITE(special_methods_postfix)
BOOST_AUTO_TEST_CASE(constructors)
{
  sveshnikov::Postfix postf_default;
  BOOST_TEST(postf_default.calculate() == 0);

  std::string expr = "";
  sveshnikov::Queue< std::string > infix1 = sveshnikov::splitIntoTokens(expr);
  sveshnikov::Postfix postf1(infix1);
  BOOST_TEST(postf1.calculate() == 0);

  expr = "( 1 + 2 ) * 4 + 3 % 5 / ( 1 + 0 )";
  sveshnikov::Queue< std::string > infix2 = sveshnikov::splitIntoTokens(expr);
  sveshnikov::Postfix postf2(infix2);
  BOOST_TEST(postf2.calculate() == 15);

  expr = "1 + 2 ( 1 + 3 )";
  BOOST_TEST(hasException(expr));

  expr = "1 + 2 ( 1 + 3 )";
  BOOST_TEST(hasException(expr));

  expr = "1 + 2 ( 1 + 3 4 + )";
  BOOST_TEST(hasException(expr));

  expr = "1 + 2 )";
  BOOST_TEST(hasException(expr));

  expr = "( )";
  BOOST_TEST(hasException(expr));

  expr = "( 234 + ) 5";
  BOOST_TEST(hasException(expr));

  expr = "( 1 + 2";
  BOOST_TEST(hasException(expr));

  expr = "1 + 2 - + 3";
  BOOST_TEST(hasException(expr));

  expr = "1 + 2 - 3 -";
  BOOST_TEST(hasException(expr));

  expr = "1 + 2 - d";
  BOOST_TEST(hasException(expr));

  sveshnikov::Postfix postf_move(postf2);
  BOOST_TEST(postf2.calculate() == 15);
  BOOST_TEST(postf_move.calculate() == 15);

  sveshnikov::Postfix postf_copy(std::move(postf2));
  BOOST_TEST(postf2.calculate() == 0);
  BOOST_TEST(postf_copy.calculate() == 15);
}

BOOST_AUTO_TEST_CASE(equal_operators)
{
  std::string expr = "4 / 2 * 3 % 4";
  sveshnikov::Queue< std::string > infix = sveshnikov::splitIntoTokens(expr);
  sveshnikov::Postfix postf1(infix);

  sveshnikov::Postfix postf2;
  postf2 = postf1;
  BOOST_TEST(postf1.calculate() == 2);
  BOOST_TEST(postf2.calculate() == 2);

  sveshnikov::Postfix postf3;
  postf3 = std::move(postf1);
  BOOST_TEST(postf1.calculate() == 0);
  BOOST_TEST(postf3.calculate() == 2);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(operators_postfix)
BOOST_AUTO_TEST_CASE(plus)
{
  std::string expr1 = "( 1 - 4 * 3 )";
  sveshnikov::Queue< std::string > infix1 = sveshnikov::splitIntoTokens(expr1);
  sveshnikov::Postfix postf1(infix1);

  std::string expr2 = " 9 / 4";
  sveshnikov::Queue< std::string > infix2 = sveshnikov::splitIntoTokens(expr2);
  sveshnikov::Postfix postf2(infix2);

  sveshnikov::Postfix postf3 = postf1 + postf2;
  BOOST_TEST(postf3.calculate() == -9);
}

BOOST_AUTO_TEST_CASE(minus)
{
  std::string expr1 = "( 1 % 4 + 3 ) - 4";
  sveshnikov::Queue< std::string > infix1 = sveshnikov::splitIntoTokens(expr1);
  sveshnikov::Postfix postf1(infix1);

  std::string expr2 = " 9 * 0 - 4 * -7";
  sveshnikov::Queue< std::string > infix2 = sveshnikov::splitIntoTokens(expr2);
  sveshnikov::Postfix postf2(infix2);

  sveshnikov::Postfix postf3 = postf1 - postf2;
  BOOST_TEST(postf3.calculate() == -28);
}

BOOST_AUTO_TEST_CASE(multiply)
{
  std::string expr1 = "8 / 4 + 3 * 2";
  sveshnikov::Queue< std::string > infix1 = sveshnikov::splitIntoTokens(expr1);
  sveshnikov::Postfix postf1(infix1);

  std::string expr2 = "0 * 8 - 5 + 7";
  sveshnikov::Queue< std::string > infix2 = sveshnikov::splitIntoTokens(expr2);
  sveshnikov::Postfix postf2(infix2);

  sveshnikov::Postfix postf3 = postf1 * postf2;
  BOOST_TEST(postf3.calculate() == 16);
}

BOOST_AUTO_TEST_CASE(divide)
{
  std::string expr1 = "4 * ( 9 - 5 )";
  sveshnikov::Queue< std::string > infix1 = sveshnikov::splitIntoTokens(expr1);
  sveshnikov::Postfix postf1(infix1);

  std::string expr2 = "4 - 5 + 7 + 2";
  sveshnikov::Queue< std::string > infix2 = sveshnikov::splitIntoTokens(expr2);
  sveshnikov::Postfix postf2(infix2);

  sveshnikov::Postfix postf3 = postf1 / postf2;
  BOOST_TEST(postf3.calculate() == 2);
}

BOOST_AUTO_TEST_CASE(remainder_of_division)
{
  std::string expr1 = "5";
  sveshnikov::Queue< std::string > infix1 = sveshnikov::splitIntoTokens(expr1);
  sveshnikov::Postfix postf1(infix1);

  std::string expr2 = "3";
  sveshnikov::Queue< std::string > infix2 = sveshnikov::splitIntoTokens(expr2);
  sveshnikov::Postfix postf2(infix2);

  sveshnikov::Postfix postf3 = postf1 % postf2;
  BOOST_TEST(postf3.calculate() == 2);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_CASE(calculate)
{
  std::string expr = "9223372036854775805 + 10";
  BOOST_TEST(hasException(expr));

  expr = "-9223372036854775807 + -10";
  BOOST_TEST(hasException(expr));

  expr = "-9223372036854775808 - 10";
  BOOST_TEST(hasException(expr));

  expr = "9223372036854775806 - -10";
  BOOST_TEST(hasException(expr));

  expr = "4 / 0";
  BOOST_TEST(hasException(expr));

  expr = "8 % 0";
  BOOST_TEST(hasException(expr));

  expr = "9223372036854775808 * -1";
  BOOST_TEST(hasException(expr));

  expr = "-1 * 9223372036854775808";
  BOOST_TEST(hasException(expr));

  expr = "9223372036854000000 * 2";
  BOOST_TEST(hasException(expr));

  expr = "-8223372036854000000 * -3";
  BOOST_TEST(hasException(expr));

  expr = "-8223372036854000000 * 3";
  BOOST_TEST(hasException(expr));

  expr = "8223372036854000000 * -3";
  BOOST_TEST(hasException(expr));
}
