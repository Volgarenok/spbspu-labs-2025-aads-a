#include <boost/test/unit_test.hpp>
#include "postfix-expression.h"

BOOST_AUTO_TEST_CASE(test_operator_plus)
{
  savintsev::PostfixExpr expr1 = savintsev::convert("2 * 2");
  savintsev::PostfixExpr expr2 = savintsev::convert("6 / 3");

  BOOST_TEST(expr1() == 4);
  BOOST_TEST(expr2() == 2);

  savintsev::PostfixExpr expr_sum(expr1 + expr2);

  BOOST_TEST(expr1() + expr2() == 6);

  BOOST_TEST(expr1() == 4);
  BOOST_TEST(expr2() == 2);

  BOOST_TEST(expr1() + expr2() == expr_sum());
  BOOST_TEST(expr1() + expr2() == (expr1 + expr2)());
}

BOOST_AUTO_TEST_CASE(test_operator_minus)
{
  savintsev::PostfixExpr expr1 = savintsev::convert("4 * 2");
  savintsev::PostfixExpr expr2 = savintsev::convert("12 / 3");

  BOOST_TEST(expr1() == 8);
  BOOST_TEST(expr2() == 4);

  savintsev::PostfixExpr expr_min(expr1 - expr2);

  BOOST_TEST(expr1() - expr2() == 4);

  BOOST_TEST(expr1() == 8);
  BOOST_TEST(expr2() == 4);

  BOOST_TEST(expr1() - expr2() == expr_min());
  BOOST_TEST(expr1() - expr2() == (expr1 - expr2)());
}

BOOST_AUTO_TEST_CASE(test_operator_mult)
{
  using namespace savintsev;

  PostfixExpr expr1 = convert("4 + 2");
  PostfixExpr expr2 = convert("7 - 3");
  PostfixExpr expr3 = convert("10 / 5");

  BOOST_TEST(expr1() == 6);
  BOOST_TEST(expr2() == 4);
  BOOST_TEST(expr3() == 2);

  savintsev::PostfixExpr expr_full(expr1 * expr2 + expr3);

  BOOST_TEST(expr1() * expr2() == 24);
  BOOST_TEST(expr_full() == 26);

  BOOST_TEST(expr1() == 6);
  BOOST_TEST(expr2() == 4);
  BOOST_TEST(expr3() == 2);

  PostfixExpr expr4 = convert("11 * 2");

  BOOST_TEST(expr1() * expr2() + expr3() == expr_full());
  BOOST_TEST((expr3 * expr2 * expr1 - expr4)() == (expr1 * expr2 + expr3)());
}

BOOST_AUTO_TEST_CASE(test_operator_div)
{
  savintsev::PostfixExpr expr1 = savintsev::convert("4 * 2");
  savintsev::PostfixExpr expr2 = savintsev::convert("12 / 3");

  BOOST_TEST(expr1() == 8);
  BOOST_TEST(expr2() == 4);

  savintsev::PostfixExpr expr_div(expr1 / expr2);

  BOOST_TEST(expr1() / expr2() == 2);

  BOOST_TEST(expr1() == 8);
  BOOST_TEST(expr2() == 4);

  BOOST_TEST(expr1() / expr2() == expr_div());
  BOOST_TEST(expr1() / expr2() == (expr1 / expr2)());
}

BOOST_AUTO_TEST_CASE(test_operator_rem)
{
  savintsev::PostfixExpr expr1 = savintsev::convert("3 * 3");
  savintsev::PostfixExpr expr2 = savintsev::convert("12 / 3");

  BOOST_TEST(expr1() == 9);
  BOOST_TEST(expr2() == 4);

  savintsev::PostfixExpr expr_div(expr1 % expr2);

  BOOST_TEST(expr1() % expr2() == 1);

  BOOST_TEST(expr1() == 9);
  BOOST_TEST(expr2() == 4);

  BOOST_TEST(expr1() % expr2() == expr_div());
  BOOST_TEST(expr1() % expr2() == (expr1 % expr2)());
}
