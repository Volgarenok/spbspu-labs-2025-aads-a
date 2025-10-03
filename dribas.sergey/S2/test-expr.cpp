#include <boost/test/unit_test.hpp>

#include "expr.hpp"

BOOST_AUTO_TEST_CASE(infixtopostfixTest)
{
  dribas::Queue< std::string > queue1;
  queue1.push("10");
  queue1.push("+");
  queue1.push("10");

  dribas::Queue< std::string > queue2 = dribas::infixToPostfix(queue1);
  BOOST_CHECK(queue2.back() == "10");
  queue2.pop();
  BOOST_TEST(queue2.back() == "10");
  queue2.pop();
  BOOST_TEST(queue2.back() == "+");
}

BOOST_AUTO_TEST_CASE(calcPostfix)
{
  dribas::Queue< std::string > queue1;
  queue1.push("10");
  queue1.push("|");
  queue1.push("10");

  dribas::Queue< std::string > queue2 = dribas::infixToPostfix(queue1);
  long long res = dribas::evaluatePostfix(queue2);
  BOOST_TEST(res == 1010);
}

BOOST_AUTO_TEST_CASE(calcPostfix2)
{
  dribas::Queue< std::string > queue1;
  queue1.push("10");
  queue1.push("*");
  queue1.push("10");
  queue1.push("|");
  queue1.push("10");

  dribas::Queue< std::string > queue2 = dribas::infixToPostfix(queue1);
  long long res = dribas::evaluatePostfix(queue2);
  BOOST_TEST(res == 10100);
}

BOOST_AUTO_TEST_CASE(calcPostfix3)
{
  dribas::Queue< std::string > queue1;
  queue1.push("(");
  queue1.push("10");
  queue1.push("*");
  queue1.push("10");
  queue1.push(")");
  queue1.push("|");
  queue1.push("10");

  dribas::Queue< std::string > queue2 = dribas::infixToPostfix(queue1);
  long long res = dribas::evaluatePostfix(queue2);
  BOOST_TEST(res == 10010);
}
