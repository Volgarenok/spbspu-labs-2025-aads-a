#include <boost/test/unit_test.hpp>
#include "calculationProcess.hpp"

using namespace kushekbaev;

BOOST_AUTO_TEST_SUITE(convertion)

BOOST_AUTO_TEST_CASE(nothingSpecial)
{
  Queue< std::string > Q;
  Q.push("10");
  Q.push("+");
  Q.push("20");
  BOOST_TEST(Q.front() == "10");
  BOOST_TEST(Q.back() == "20");
  Queue< std::string > postfixQ = convertToPostfix(Q);
  BOOST_TEST(postfixQ.front() == "10");
  BOOST_TEST(postfixQ.back() == "+");
}

BOOST_AUTO_TEST_CASE(parenthesis)
{
  Queue< std::string > Q;
  Q.push("(");
  Q.push("10");
  Q.push("+");
  Q.push("20");
  Q.push(")");
  Q.push("-");
  Q.push("10");
  BOOST_TEST(Q.front() == "(");
  BOOST_TEST(Q.back() == "10");
  Queue< std::string > postfixQ = convertToPostfix(Q);
  BOOST_TEST(postfixQ.front() == "10");
  BOOST_TEST(postfixQ.back() == "-");
}

BOOST_AUTO_TEST_CASE(diffPriority)
{
  Queue< std::string > Q;
  Q.push("1");
  Q.push("+");
  Q.push("2");
  Q.push("**");
  Q.push("3");
  Q.push("*");
  Q.push("4");
  BOOST_TEST(Q.front() == "1");
  BOOST_TEST(Q.back() == "4");
  Queue< std::string > postfixQ = convertToPostfix(Q);
  BOOST_TEST(postfixQ.front() == "1");
  BOOST_TEST(postfixQ.back() == "+");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(calculation)

BOOST_AUTO_TEST_CASE(defaultAddition)
{
  Queue< std::string > Q;
  Q.push("1");
  Q.push("+");
  Q.push("828");
  Queue< std::string > postfixQ = convertToPostfix(Q);
  long long int result = calculatePostfix(postfixQ);
  BOOST_TEST(result == 829);
}

BOOST_AUTO_TEST_CASE(additionToNegative)
{
  Queue< std::string > Q;
  Q.push("-10");
  Q.push("+");
  Q.push("1994");
  Queue< std::string > postfixQ = convertToPostfix(Q);
  long long int result = calculatePostfix(postfixQ);
  BOOST_TEST(result == 1984);
}

BOOST_AUTO_TEST_CASE(negativeAddition)
{
  Queue< std::string > Q;
  Q.push("10");
  Q.push("+");
  Q.push("-224");
  Queue< std::string > postfixQ = convertToPostfix(Q);
  long long int result = calculatePostfix(postfixQ);
  BOOST_TEST(result == -214);
}

BOOST_AUTO_TEST_CASE(negativePlusNegative)
{
  Queue< std::string > Q;
  Q.push("-10");
  Q.push("+");
  Q.push("-1994");
  Queue< std::string > postfixQ = convertToPostfix(Q);
  long long int result = calculatePostfix(postfixQ);
  BOOST_TEST(result == -2004);
}

BOOST_AUTO_TEST_CASE(zeroPlusZero)
{
  Queue< std::string > Q;
  Q.push("0");
  Q.push("+");
  Q.push("0");
  Queue< std::string > postfixQ = convertToPostfix(Q);
  long long int result = calculatePostfix(postfixQ);
  BOOST_TEST(result == 0);
}

BOOST_AUTO_TEST_CASE(defaultSubstraction)
{
  Queue< std::string > Q;
  Q.push("1000");
  Q.push("-");
  Q.push("7");
  Queue< std::string > postfixQ = convertToPostfix(Q);
  long long int result = calculatePostfix(postfixQ);
  BOOST_TEST(result == 993);
}

BOOST_AUTO_TEST_CASE(negativeSubstraction)
{
  Queue< std::string > Q;
  Q.push("-10");
  Q.push("-");
  Q.push("3");
  Queue< std::string > postfixQ = convertToPostfix(Q);
  long long int result = calculatePostfix(postfixQ);
  BOOST_TEST(result == -13);
}

BOOST_AUTO_TEST_CASE(substractionOfNegative)
{
  Queue< std::string > Q;
  Q.push("40");
  Q.push("-");
  Q.push("-2");
  Queue< std::string > postfixQ = convertToPostfix(Q);
  long long int result = calculatePostfix(postfixQ);
  BOOST_TEST(result == 42);
}

BOOST_AUTO_TEST_CASE(negativeMinusNegative)
{
  Queue< std::string > Q;
  Q.push("-1984");
  Q.push("-");
  Q.push("-1337");
  Queue< std::string > postfixQ = convertToPostfix(Q);
  long long int result = calculatePostfix(postfixQ);
  BOOST_TEST(result == -647);
}

BOOST_AUTO_TEST_CASE(zeroMinusZero)
{
  Queue< std::string > Q;
  Q.push("0");
  Q.push("-");
  Q.push("0");
  Queue< std::string > postfixQ = convertToPostfix(Q);
  long long int result = calculatePostfix(postfixQ);
  BOOST_TEST(result == 0);
}

BOOST_AUTO_TEST_CASE(defaultDivision)
{
  Queue< std::string > Q;
  Q.push("12");
  Q.push("/");
  Q.push("3");
  Queue< std::string > postfixQ = convertToPostfix(Q);
  long long int result = calculatePostfix(postfixQ);
  BOOST_TEST(result == 4);
}

BOOST_AUTO_TEST_CASE(divisionOfNegative)
{
  Queue< std::string > Q;
  Q.push("-50");
  Q.push("/");
  Q.push("5");
  Queue< std::string > postfixQ = convertToPostfix(Q);
  long long int result = calculatePostfix(postfixQ);
  BOOST_TEST(result == -10);
}

BOOST_AUTO_TEST_CASE(negativeDivision)
{
  Queue< std::string > Q;
  Q.push("228");
  Q.push("/");
  Q.push("-2");
  Queue< std::string > postfixQ = convertToPostfix(Q);
  long long int result = calculatePostfix(postfixQ);
  BOOST_TEST(result == -114);
}

BOOST_AUTO_TEST_CASE(negativeDividedByNegative)
{
  Queue< std::string > Q;
  Q.push("-2222");
  Q.push("/");
  Q.push("-22");
  Queue< std::string > postfixQ = convertToPostfix(Q);
  long long int result = calculatePostfix(postfixQ);
  BOOST_TEST(result == 101);
}

BOOST_AUTO_TEST_CASE(zeroDivided)
{
  Queue< std::string > Q;
  Q.push("0");
  Q.push("/");
  Q.push("1234");
  Queue< std::string > postfixQ = convertToPostfix(Q);
  long long int result = calculatePostfix(postfixQ);
  BOOST_TEST(result == 0);
}

BOOST_AUTO_TEST_CASE(defaultModulation)
{
  Queue< std::string > Q;
  Q.push("12");
  Q.push("%");
  Q.push("5");
  Queue< std::string > postfixQ = convertToPostfix(Q);
  long long int result = calculatePostfix(postfixQ);
  BOOST_TEST(result == 2);
}

BOOST_AUTO_TEST_CASE(modulationOfNegative)
{
  Queue< std::string > Q;
  Q.push("-123");
  Q.push("%");
  Q.push("120");
  Queue< std::string > postfixQ = convertToPostfix(Q);
  long long int result = calculatePostfix(postfixQ);
  BOOST_TEST(result == 117);
}

BOOST_AUTO_TEST_CASE(negativeModulation)
{
  Queue< std::string > Q;
  Q.push("12");
  Q.push("%");
  Q.push("-5");
  Queue< std::string > postfixQ = convertToPostfix(Q);
  long long int result = calculatePostfix(postfixQ);
  BOOST_TEST(result == -3);
}

BOOST_AUTO_TEST_CASE(negativeModulatedByNegative)
{
  Queue< std::string > Q;
  Q.push("-12");
  Q.push("%");
  Q.push("-5");
  Queue< std::string > postfixQ = convertToPostfix(Q);
  long long int result = calculatePostfix(postfixQ);
  BOOST_TEST(result == -2);
}

BOOST_AUTO_TEST_CASE(zeroModulated)
{
  Queue< std::string > Q;
  Q.push("0");
  Q.push("%");
  Q.push("5");
  Queue< std::string > postfixQ = convertToPostfix(Q);
  long long int result = calculatePostfix(postfixQ);
  BOOST_TEST(result == 0);
}

BOOST_AUTO_TEST_CASE(secondOperandIsBigger)
{
  Queue< std::string > Q;
  Q.push("5");
  Q.push("%");
  Q.push("12");
  Queue< std::string > postfixQ = convertToPostfix(Q);
  long long int result = calculatePostfix(postfixQ);
  BOOST_TEST(result == 5);
}

BOOST_AUTO_TEST_CASE(defaultMultiplication)
{
  Queue< std::string > Q;
  Q.push("5");
  Q.push("*");
  Q.push("12");
  Queue< std::string > postfixQ = convertToPostfix(Q);
  long long int result = calculatePostfix(postfixQ);
  BOOST_TEST(result == 60);
}

BOOST_AUTO_TEST_CASE(multiplicationOfNegative)
{
  Queue< std::string > Q;
  Q.push("-5");
  Q.push("*");
  Q.push("12");
  Queue< std::string > postfixQ = convertToPostfix(Q);
  long long int result = calculatePostfix(postfixQ);
  BOOST_TEST(result == -60);
}

BOOST_AUTO_TEST_CASE(negativeMultiplication)
{
  Queue< std::string > Q;
  Q.push("5");
  Q.push("*");
  Q.push("-12");
  Queue< std::string > postfixQ = convertToPostfix(Q);
  long long int result = calculatePostfix(postfixQ);
  BOOST_TEST(result == -60);
}

BOOST_AUTO_TEST_CASE(negativeMultiplicatedByNegative)
{
  Queue< std::string > Q;
  Q.push("-5");
  Q.push("*");
  Q.push("-12");
  Queue< std::string > postfixQ = convertToPostfix(Q);
  long long int result = calculatePostfix(postfixQ);
  BOOST_TEST(result == 60);
}

BOOST_AUTO_TEST_CASE(multiplicationByZero)
{
  Queue< std::string > Q;
  Q.push("5");
  Q.push("*");
  Q.push("0");
  Queue< std::string > postfixQ = convertToPostfix(Q);
  long long int result = calculatePostfix(postfixQ);
  BOOST_TEST(result == 0);
}

BOOST_AUTO_TEST_CASE(zeroMultiplicatedByZero)
{
  Queue< std::string > Q;
  Q.push("0");
  Q.push("*");
  Q.push("0");
  Queue< std::string > postfixQ = convertToPostfix(Q);
  long long int result = calculatePostfix(postfixQ);
  BOOST_TEST(result == 0);
}

BOOST_AUTO_TEST_CASE(defaultPower)
{
  Queue< std::string > Q;
  Q.push("4");
  Q.push("**");
  Q.push("3");
  Queue< std::string > postfixQ = convertToPostfix(Q);
  long long int result = calculatePostfix(postfixQ);
  BOOST_TEST(result == 64);
}

BOOST_AUTO_TEST_CASE(negativeInThePowerOfOdd)
{
  Queue< std::string > Q;
  Q.push("-2");
  Q.push("**");
  Q.push("1");
  Queue< std::string > postfixQ = convertToPostfix(Q);
  long long int result = calculatePostfix(postfixQ);
  BOOST_TEST(result == -2);
}

BOOST_AUTO_TEST_CASE(negativeInThePowerOfEven)
{
  Queue< std::string > Q;
  Q.push("-2");
  Q.push("**");
  Q.push("4");
  Queue< std::string > postfixQ = convertToPostfix(Q);
  long long int result = calculatePostfix(postfixQ);
  BOOST_TEST(result == 16);
}

BOOST_AUTO_TEST_CASE(powerOfZero)
{
  Queue< std::string > Q;
  Q.push("0");
  Q.push("**");
  Q.push("2");
  Queue< std::string > postfixQ = convertToPostfix(Q);
  long long int result = calculatePostfix(postfixQ);
  BOOST_TEST(result == 0);
}

BOOST_AUTO_TEST_CASE(powerIsZero)
{
  Queue< std::string > Q;
  Q.push("2");
  Q.push("**");
  Q.push("0");
  Queue< std::string > postfixQ = convertToPostfix(Q);
  long long int result = calculatePostfix(postfixQ);
  BOOST_TEST(result == 1);
}

BOOST_AUTO_TEST_SUITE_END()
