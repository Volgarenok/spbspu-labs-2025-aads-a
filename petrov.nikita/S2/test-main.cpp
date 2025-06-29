#define BOOST_TEST_MODULE S2
#include <boost/test/included/unit_test.hpp>
#include <sstream>
#include "parse_text_and_push_it_to_queue.hpp"
#include "calculate_arithmetic_expressions.hpp"

BOOST_AUTO_TEST_SUITE(pow_operation)

BOOST_AUTO_TEST_CASE(raise_five_to_first_power)
{
  std::ostringstream out;
  std::queue< std::string > queue;
  std::string text = "5 ** 1";
  petrov::parseTextAndPushItToQueue(text, queue);
  petrov::Stack< long long int > output_stack;
  output_stack = petrov::calculateArithmeticExpressions(queue);
  out << output_stack.top();
  output_stack.pop();
  size_t stack_size = output_stack.size();
  for (size_t i = 0; i < stack_size; i++)
  {
    out << " " << output_stack.top();
    output_stack.pop();
  }
  BOOST_TEST(out.str() == "5");
}

BOOST_AUTO_TEST_CASE(raise_five_to_second_and_third_power)
{
  std::ostringstream out;
  std::queue< std::string > queue;
  std::string text = "5 ** 2";
  petrov::parseTextAndPushItToQueue(text, queue);
  text = "5 ** 3";
  petrov::parseTextAndPushItToQueue(text, queue);
  petrov::Stack< long long int > output_stack;
  output_stack = petrov::calculateArithmeticExpressions(queue);
  out << output_stack.top();
  output_stack.pop();
  size_t stack_size = output_stack.size();
  for (size_t i = 0; i < stack_size; i++)
  {
    out << " " << output_stack.top();
    output_stack.pop();
  }
  BOOST_TEST(out.str() == "125 25");
}

BOOST_AUTO_TEST_CASE(raise_three_to_ten_and_five_power)
{
  std::ostringstream out;
  std::queue< std::string > queue;
  std::string text = "3 ** 10";
  petrov::parseTextAndPushItToQueue(text, queue);
  text = "3 ** 5";
  petrov::parseTextAndPushItToQueue(text, queue);
  petrov::Stack< long long int > output_stack;
  output_stack = petrov::calculateArithmeticExpressions(queue);
  out << output_stack.top();
  output_stack.pop();
  size_t stack_size = output_stack.size();
  for (size_t i = 0; i < stack_size; i++)
  {
    out << " " << output_stack.top();
    output_stack.pop();
  }
  BOOST_TEST(out.str() == "243 59049");
}

BOOST_AUTO_TEST_CASE(raise_five_to_zero_power)
{
  std::ostringstream out;
  std::queue< std::string > queue;
  std::string text = "5 ** 0";
  petrov::parseTextAndPushItToQueue(text, queue);
  petrov::Stack< long long int > output_stack;
  output_stack = petrov::calculateArithmeticExpressions(queue);
  out << output_stack.top();
  output_stack.pop();
  size_t stack_size = output_stack.size();
  for (size_t i = 0; i < stack_size; i++)
  {
    out << " " << output_stack.top();
    output_stack.pop();
  }
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_CASE(arithmetic_expression)
{
  std::ostringstream out;
  std::queue< std::string > queue;
  std::string text = "( ( 5 + 2 ) ** 2 ) + 1";
  petrov::parseTextAndPushItToQueue(text, queue);
  petrov::Stack< long long int > output_stack;
  output_stack = petrov::calculateArithmeticExpressions(queue);
  out << output_stack.top();
  output_stack.pop();
  size_t stack_size = output_stack.size();
  for (size_t i = 0; i < stack_size; i++)
  {
    out << " " << output_stack.top();
    output_stack.pop();
  }
  BOOST_TEST(out.str() == "50");
}

BOOST_AUTO_TEST_SUITE_END()
