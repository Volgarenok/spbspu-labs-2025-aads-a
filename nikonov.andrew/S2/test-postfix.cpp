#include <boost/test/unit_test.hpp>
#include "postfix.hpp"
#include "sstream"
#include <iostream>
BOOST_AUTO_TEST_CASE(plusOperator_test)
{
  nikonov::Postfix expr1("2 * 2");
  nikonov::Postfix expr2("6 / 3");

  BOOST_TEST(expr1() == 4);
  BOOST_TEST(expr2() == 2);

  nikonov::Postfix expr_sum(expr1 + expr2);

  BOOST_TEST(expr1() + expr2() == 6);

  BOOST_TEST(expr1() == 4);
  BOOST_TEST(expr2() == 2);

  BOOST_TEST(expr1() + expr2() == expr_sum());
  BOOST_TEST(expr1() + expr2() == (expr1 + expr2)());
}

BOOST_AUTO_TEST_CASE(substractionOperator_test)
{
  nikonov::Postfix expr1("4 * 2");
  nikonov::Postfix expr2("12 / 3");

  BOOST_TEST(expr1() == 8);
  BOOST_TEST(expr2() == 4);

  nikonov::Postfix expr_min(expr1 - expr2);

  BOOST_TEST(expr1() - expr2() == 4);

  BOOST_TEST(expr1() == 8);
  BOOST_TEST(expr2() == 4);

  BOOST_TEST(expr1() - expr2() == expr_min());
  BOOST_TEST(expr1() - expr2() == (expr1 - expr2)());
}

BOOST_AUTO_TEST_CASE(multOperator_test)
{
  using namespace nikonov;

  Postfix expr1("4 + 2");
  Postfix expr2("7 - 3");
  Postfix expr3("10 / 5");

  BOOST_TEST(expr1() == 6);
  BOOST_TEST(expr2() == 4);
  BOOST_TEST(expr3() == 2);

  nikonov::Postfix expr_full(expr1 * expr2 + expr3);

  BOOST_TEST(expr1() * expr2() == 24);
  BOOST_TEST(expr_full() == 26);

  BOOST_TEST(expr1() == 6);
  BOOST_TEST(expr2() == 4);
  BOOST_TEST(expr3() == 2);

  Postfix expr4("11 * 2");

  BOOST_TEST(expr1() * expr2() + expr3() == expr_full());
  BOOST_TEST((expr3 * expr2 * expr1 - expr4)() == (expr1 * expr2 + expr3)());
}

BOOST_AUTO_TEST_CASE(divOperator_test)
{
  nikonov::Postfix expr1("4 * 2");
  nikonov::Postfix expr2("12 / 3");

  BOOST_TEST(expr1() == 8);
  BOOST_TEST(expr2() == 4);

  nikonov::Postfix expr_div(expr1 / expr2);

  BOOST_TEST(expr1() / expr2() == 2);

  BOOST_TEST(expr1() == 8);
  BOOST_TEST(expr2() == 4);

  BOOST_TEST(expr1() / expr2() == expr_div());
  BOOST_TEST(expr1() / expr2() == (expr1 / expr2)());
}

BOOST_AUTO_TEST_CASE(removeOperator_test)
{
  nikonov::Postfix expr1("3 * 3");
  nikonov::Postfix expr2("12 / 3");

  BOOST_TEST(expr1() == 9);
  BOOST_TEST(expr2() == 4);

  nikonov::Postfix expr_div(expr1 % expr2);

  BOOST_TEST(expr1() % expr2() == 1);

  BOOST_TEST(expr1() == 9);
  BOOST_TEST(expr2() == 4);

  BOOST_TEST(expr1() % expr2() == expr_div());
  BOOST_TEST(expr1() % expr2() == (expr1 % expr2)());
}
