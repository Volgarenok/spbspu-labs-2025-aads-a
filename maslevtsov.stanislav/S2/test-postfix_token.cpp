#include <boost/test/unit_test.hpp>
#include "postfix_token.hpp"

BOOST_AUTO_TEST_SUITE(postfix_token_constructor_tests)
BOOST_AUTO_TEST_CASE(convert_constructor_test)
{
  maslevtsov::PostfixToken exp1("1 + 1");
  BOOST_TEST(exp1() == 2);
  try {
    maslevtsov::PostfixToken exp2("1 + 1 )");
  } catch (const std::logic_error&) {
    BOOST_TEST(true);
  }
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(postfix_token_math_operators_tests)
BOOST_AUTO_TEST_CASE(postfix_token_addition_operator_test)
{
  maslevtsov::PostfixToken exp1("1 + 1");
  maslevtsov::PostfixToken exp2("( 2 + 1 )");
  maslevtsov::PostfixToken exp(exp1 + exp2);
  BOOST_TEST(exp() == 5);
}

BOOST_AUTO_TEST_CASE(postfix_token_subtraction_operator_test)
{
  maslevtsov::PostfixToken exp1("1 + 1");
  maslevtsov::PostfixToken exp2("( 2 + 1 )");
  maslevtsov::PostfixToken exp(exp1 - exp2);
  BOOST_TEST(exp() == -1);
}

BOOST_AUTO_TEST_CASE(postfix_token_multiplication_operator_test)
{
  maslevtsov::PostfixToken exp1("1 + 1");
  maslevtsov::PostfixToken exp2("( 2 + 1 )");
  maslevtsov::PostfixToken exp(exp1 * exp2);
  BOOST_TEST(exp() == 6);
}

BOOST_AUTO_TEST_CASE(postfix_token_division_operator_test)
{
  maslevtsov::PostfixToken exp1("5 + 1");
  maslevtsov::PostfixToken exp2("( 2 + 1 )");
  maslevtsov::PostfixToken exp(exp1 / exp2);
  BOOST_TEST(exp() == 2);
}

BOOST_AUTO_TEST_CASE(postfix_token_remainder_operator_test)
{
  maslevtsov::PostfixToken exp1("1 + 1");
  maslevtsov::PostfixToken exp2("( 2 + 1 )");
  maslevtsov::PostfixToken exp(exp1 % exp2);
  BOOST_TEST(exp() == 2);
}

BOOST_AUTO_TEST_CASE(postfix_token_parenthesis_operator_test)
{
  maslevtsov::PostfixToken exp("( 1 + 1 ) / ( 3 - 1 )");
  long long res = exp();
  BOOST_TEST(res == 1);
}
BOOST_AUTO_TEST_SUITE_END()
