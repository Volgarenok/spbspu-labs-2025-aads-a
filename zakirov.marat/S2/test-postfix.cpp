#include <boost/test/unit_test.hpp>
#include "postfix.hpp"

BOOST_AUTO_TEST_CASE(string_constructor_postfix)
{
  zakirov::Postfix postfix_t("1 + 1");
  BOOST_TEST(postfix_t() == 2);
}

BOOST_AUTO_TEST_CASE(operator_plus_postfix)
{
  zakirov::Postfix postfix_1("1 - 2");
  zakirov::Postfix postfix_2("5 - 6");
  zakirov::Postfix postfix_t(postfix_1 + postfix_2);
  BOOST_TEST(postfix_1() == -1);
  BOOST_TEST(postfix_2() == -1);
  BOOST_TEST(postfix_t() == -2);
  BOOST_TEST(postfix_t() = postfix_1() + postfix_2());
}


BOOST_AUTO_TEST_CASE(operator_minus_postfix)
{
  zakirov::Postfix postfix_1("1 - 2");
  zakirov::Postfix postfix_2("5 - 6");
  zakirov::Postfix postfix_t(postfix_1 - postfix_2);
  BOOST_TEST(postfix_1() == -1);
  BOOST_TEST(postfix_2() == -1);
  BOOST_TEST(postfix_t() == 0);
  BOOST_TEST(postfix_t() == postfix_1() - postfix_2());
}

BOOST_AUTO_TEST_CASE(operator_multiply_postfix)
{
  zakirov::Postfix postfix_1("1 - 2");
  zakirov::Postfix postfix_2("5 - 6");
  zakirov::Postfix postfix_t(postfix_1 * postfix_2);
  BOOST_TEST(postfix_1() == -1);
  BOOST_TEST(postfix_2() == -1);
  BOOST_TEST(postfix_t() == 1);
  BOOST_TEST(postfix_t() == postfix_1() * postfix_2());
}

BOOST_AUTO_TEST_CASE(operator_remainder_of_postfix)
{
  zakirov::Postfix postfix_1("1 + 4");
  zakirov::Postfix postfix_2("6 - 3");
  zakirov::Postfix postfix_t(postfix_1 % postfix_2);
  BOOST_TEST(postfix_1() == 5);
  BOOST_TEST(postfix_2() == 3);
  BOOST_TEST(postfix_t() == 2);
  BOOST_TEST(postfix_t() == postfix_1() % postfix_2());
}
