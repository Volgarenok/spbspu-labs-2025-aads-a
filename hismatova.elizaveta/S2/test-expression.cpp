#include <boost/test/unit_test.hpp>
#include "PostfixValue.hpp"
using namespace hismatova;
BOOST_AUTO_TEST_CASE(default_constructor)
{
  PostfixValue pv;
  BOOST_TEST(pv.getValue() == 0);
}
BOOST_AUTO_TEST_CASE(constructor_with_number)
{
  PostfixValue pv(42);
  BOOST_TEST(pv.getValue() == 42);
}
BOOST_AUTO_TEST_CASE(constructor_with_operator)
{
  PostfixValue pv('+');
  BOOST_TEST_EXCEPTION(pv.getValue(), std::exception);
}
BOOST_AUTO_TEST_CASE(addition_operator)
{
  PostfixValue pv1(10);
  PostfixValue pv2(5);
  PostfixValue result = pv1 + pv2;
  BOOST_TEST(result.getValue() == 15);
}
BOOST_AUTO_TEST_CASE(subtraction_operator)
{
  PostfixValue pv1(10);
  PostfixValue pv2(4);
  PostfixValue result = pv1 - pv2;
  BOOST_TEST(result.getValue() == 6);
}
BOOST_AUTO_TEST_CASE(multiplication_operator)
{
  PostfixValue pv1(6);
  PostfixValue pv2(7);
  PostfixValue result = pv1 * pv2;
  BOOST_TEST(result.getValue() == 42);
}
BOOST_AUTO_TEST_CASE(division_operator)
{
  PostfixValue pv1(20);
  PostfixValue pv2(5);
  PostfixValue result = pv1 / pv2;
  BOOST_TEST(result.getValue() == 4);
}
BOOST_AUTO_TEST_CASE(division_by_zero_throws)
{
  PostfixValue pv1(10);
  PostfixValue pv2(0);
  BOOST_TEST_EXCEPTION(pv1 / pv2, std::exception);
}
BOOST_AUTO_TEST_CASE(modulo_operator)
{
  PostfixValue pv1(10);
  PostfixValue pv2(3);
  PostfixValue result = pv1 % pv2;
  BOOST_TEST(result.getValue() == 1);
}
BOOST_AUTO_TEST_CASE(modulo_by_zero_throws)
{
  PostfixValue pv1(10);
  PostfixValue pv2(0);
  BOOST_TEST_EXCEPTION(pv1 % pv2, std::exception);
}
BOOST_AUTO_TEST_SUITE_END()
