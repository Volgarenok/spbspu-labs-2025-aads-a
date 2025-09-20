#include <boost/test/unit_test.hpp>
#include <string>
#include "postfix.hpp"
#include "io-infix.hpp"

namespace
{
  void calcExpr(const std::string &expr);

  void calcExpr(const std::string &expr)
  {
    using namespace sveshnikov;
    Queue< std::string > infix2 = splitIntoTokens(expr);
    Postfix postf2(infix2);
    postf2.calculate();
  }
}

BOOST_AUTO_TEST_SUITE(special_methods_postfix)
BOOST_AUTO_TEST_CASE(constructors)
{
  using namespace sveshnikov;
  Postfix postf_default;
  BOOST_TEST(postf_default.calculate() == 0);

  std::string expr = "";
  Queue< std::string > infix1 = splitIntoTokens(expr);
  Postfix postf1(infix1);
  BOOST_TEST(postf1.calculate() == 0);

  expr = "( 1 + 2 ) * 4 + 3 % 5 / ( 1 + 0 )";
  Queue< std::string > infix2 = splitIntoTokens(expr);
  Postfix postf2(infix2);
  BOOST_TEST(postf2.calculate() == 15);

  expr = "1 + 2 ( 1 + 3 )";
  BOOST_CHECK_THROW(calcExpr(expr), std::exception);

  expr = "1 + 2 ( 1 + 3 4 + )";
  BOOST_CHECK_THROW(calcExpr(expr), std::exception);

  expr = "1 + 2 )";
  BOOST_CHECK_THROW(calcExpr(expr), std::exception);

  expr = "( )";
  BOOST_CHECK_THROW(calcExpr(expr), std::exception);

  expr = "( 234 + ) 5";
  BOOST_CHECK_THROW(calcExpr(expr), std::exception);

  expr = "( 1 + 2";
  BOOST_CHECK_THROW(calcExpr(expr), std::exception);

  expr = "1 + 2 - + 3";
  BOOST_CHECK_THROW(calcExpr(expr), std::exception);

  expr = "1 + 2 - 3 -";
  BOOST_CHECK_THROW(calcExpr(expr), std::exception);

  expr = "1 + 2 - d";
  BOOST_CHECK_THROW(calcExpr(expr), std::exception);

  Postfix postf_move(postf2);
  BOOST_TEST(postf2.calculate() == 15);
  BOOST_TEST(postf_move.calculate() == 15);

  Postfix postf_copy(std::move(postf2));
  BOOST_TEST(postf2.calculate() == 0);
  BOOST_TEST(postf_copy.calculate() == 15);
}

BOOST_AUTO_TEST_CASE(equal_operators)
{
  using namespace sveshnikov;
  std::string expr = "4 / 2 * 3 % 4";
  Queue< std::string > infix = splitIntoTokens(expr);
  Postfix postf1(infix);

  Postfix postf2;
  postf2 = postf1;
  BOOST_TEST(postf1.calculate() == 2);
  BOOST_TEST(postf2.calculate() == 2);

  Postfix postf3;
  postf3 = std::move(postf1);
  BOOST_TEST(postf1.calculate() == 0);
  BOOST_TEST(postf3.calculate() == 2);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(operators_postfix)
BOOST_AUTO_TEST_CASE(plus)
{
  using namespace sveshnikov;
  std::string expr1 = "( 1 - 4 * 3 )";
  Queue< std::string > infix1 = splitIntoTokens(expr1);
  Postfix postf1(infix1);

  std::string expr2 = " 9 / 4";
  Queue< std::string > infix2 = splitIntoTokens(expr2);
  Postfix postf2(infix2);

  Postfix postf3 = postf1 + postf2;
  BOOST_TEST(postf3.calculate() == -9);
}

BOOST_AUTO_TEST_CASE(minus)
{
  using namespace sveshnikov;
  std::string expr1 = "( 1 % 4 + 3 ) - 4";
  Queue< std::string > infix1 = splitIntoTokens(expr1);
  Postfix postf1(infix1);

  std::string expr2 = " 9 * 0 - 4 * -7";
  Queue< std::string > infix2 = splitIntoTokens(expr2);
  Postfix postf2(infix2);

  Postfix postf3 = postf1 - postf2;
  BOOST_TEST(postf3.calculate() == -28);
}

BOOST_AUTO_TEST_CASE(multiply)
{
  using namespace sveshnikov;
  std::string expr1 = "8 / 4 + 3 * 2";
  Queue< std::string > infix1 = splitIntoTokens(expr1);
  Postfix postf1(infix1);

  std::string expr2 = "0 * 8 - 5 + 7";
  Queue< std::string > infix2 = splitIntoTokens(expr2);
  Postfix postf2(infix2);

  Postfix postf3 = postf1 * postf2;
  BOOST_TEST(postf3.calculate() == 16);
}

BOOST_AUTO_TEST_CASE(divide)
{
  using namespace sveshnikov;
  std::string expr1 = "4 * ( 9 - 5 )";
  Queue< std::string > infix1 = splitIntoTokens(expr1);
  Postfix postf1(infix1);

  std::string expr2 = "4 - 5 + 7 + 2";
  Queue< std::string > infix2 = splitIntoTokens(expr2);
  Postfix postf2(infix2);

  Postfix postf3 = postf1 / postf2;
  BOOST_TEST(postf3.calculate() == 2);
}

BOOST_AUTO_TEST_CASE(modulo_division)
{
  using namespace sveshnikov;
  std::string expr1 = "5";
  Queue< std::string > infix1 = splitIntoTokens(expr1);
  Postfix postf1(infix1);

  std::string expr2 = "3";
  Queue< std::string > infix2 = splitIntoTokens(expr2);
  Postfix postf2(infix2);

  Postfix postf3 = postf1 % postf2;
  BOOST_TEST(postf3.calculate() == 2);
}

BOOST_AUTO_TEST_CASE(double_power)
{
  using namespace sveshnikov;
  std::string expr = "0 ^^ 0";
  BOOST_CHECK_THROW(calcExpr(expr), std::exception);

  expr = "0 ^^ -4";
  BOOST_CHECK_THROW(calcExpr(expr), std::exception);

  expr = "3 ^^ -4";
  BOOST_CHECK_THROW(calcExpr(expr), std::exception);

  expr = "( 1 + 2 ) ^^ ( 7 - 5 )";
  Queue< std::string > infix = splitIntoTokens(expr);
  Postfix postf(infix);
  BOOST_TEST(postf.calculate() == 81);

  expr = "0 ^^ ( 7 - 2 )";
  infix = splitIntoTokens(expr);
  postf = Postfix(infix);
  BOOST_TEST(postf.calculate() == 0);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_CASE(calculate)
{
  std::string expr = "9223372036854775805 + 10";
  BOOST_CHECK_THROW(calcExpr(expr), std::exception);

  expr = "-9223372036854775807 + -10";
  BOOST_CHECK_THROW(calcExpr(expr), std::exception);

  expr = "-9223372036854775808 - 10";
  BOOST_CHECK_THROW(calcExpr(expr), std::exception);

  expr = "9223372036854775806 - -10";
  BOOST_CHECK_THROW(calcExpr(expr), std::exception);

  expr = "4 / 0";
  BOOST_CHECK_THROW(calcExpr(expr), std::exception);

  expr = "8 % 0";
  BOOST_CHECK_THROW(calcExpr(expr), std::exception);

  expr = "9223372036854775808 * -1";
  BOOST_CHECK_THROW(calcExpr(expr), std::exception);

  expr = "-1 * 9223372036854775808";
  BOOST_CHECK_THROW(calcExpr(expr), std::exception);

  expr = "9223372036854000000 * 2";
  BOOST_CHECK_THROW(calcExpr(expr), std::exception);

  expr = "-8223372036854000000 * -3";
  BOOST_CHECK_THROW(calcExpr(expr), std::exception);

  expr = "-8223372036854000000 * 3";
  BOOST_CHECK_THROW(calcExpr(expr), std::exception);

  expr = "8223372036854000000 * -3";
  BOOST_CHECK_THROW(calcExpr(expr), std::exception);
}
