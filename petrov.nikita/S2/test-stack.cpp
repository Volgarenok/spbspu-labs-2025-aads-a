#include <boost/test/unit_test.hpp>
#include <sstream>
#include <stack.hpp>

BOOST_AUTO_TEST_SUITE(copy_constructor)

BOOST_AUTO_TEST_CASE(make_stack_from_empty_stack)
{
  std::ostringstream out;
  petrov::Stack< int > first_stack;
  petrov::Stack< int > second_stack(first_stack);
  out << first_stack.empty() << " " << second_stack.empty();
  BOOST_TEST(out.str() == "1 1");
}

BOOST_AUTO_TEST_CASE(make_stack_from_one_element_stack)
{
  std::ostringstream out;
  petrov::Stack< int > first_stack;
  first_stack.push(1);
  petrov::Stack< int > second_stack(first_stack);
  out << first_stack.top() << " " << second_stack.top();
  BOOST_TEST(out.str() == "1 1");
}

BOOST_AUTO_TEST_CASE(make_stack_from_five_element_stack)
{
  std::ostringstream out;
  petrov::Stack< int > first_stack;
  for (size_t i = 1; i <= 5; i++)
  {
    first_stack.push(i);
  }
  petrov::Stack< int > second_stack(first_stack);
  out << second_stack.top();
  second_stack.pop();
  for (size_t i = 1; i < 5; i++)
  {
    out << " " << second_stack.top();
    second_stack.pop();
  }
  BOOST_TEST(out.str() == "5 4 3 2 1");
}

BOOST_AUTO_TEST_CASE(make_stack_from_six_element_stack)
{
  std::ostringstream out;
  petrov::Stack< int > first_stack;
  for (size_t i = 1; i <= 6; i++)
  {
    first_stack.push(i);
  }
  petrov::Stack< int > second_stack(first_stack);
  out << second_stack.top();
  second_stack.pop();
  for (size_t i = 1; i < 6; i++)
  {
    out << " " << second_stack.top();
    second_stack.pop();
  }
  BOOST_TEST(out.str() == "6 5 4 3 2 1");
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(assign)

BOOST_AUTO_TEST_CASE(assign_stack_from_empty_element_stack)
{
  std::ostringstream out;
  petrov::Stack< int > first_stack;
  petrov::Stack< int > second_stack;
  second_stack.push(1);
  second_stack = first_stack;
  out << second_stack.empty();
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_CASE(assign_stack_from_one_element_stack)
{
  std::ostringstream out;
  petrov::Stack< int > first_stack;
  petrov::Stack< int > second_stack;
  first_stack.push(1);
  second_stack = first_stack;
  out << second_stack.top();
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_CASE(assign_stack_from_five_element_stack)
{
  std::ostringstream out;
  petrov::Stack< int > first_stack;
  for (size_t i = 1; i <= 5; i++)
  {
    first_stack.push(i);
  }
  petrov::Stack< int > second_stack;
  for (size_t i = 1; i <= 3; i++)
  {
    second_stack.push(i);
  }
  second_stack = first_stack;
  out << second_stack.top();
  second_stack.pop();
  for (size_t i = 1; i < 5; i++)
  {
    out << " " << second_stack.top();
    second_stack.pop();
  }
  BOOST_TEST(out.str() == "5 4 3 2 1");
}

BOOST_AUTO_TEST_CASE(make_stack_from_six_element_stack)
{
  std::ostringstream out;
  petrov::Stack< int > first_stack;
  for (size_t i = 1; i <= 6; i++)
  {
    first_stack.push(i);
  }
  petrov::Stack< int > second_stack;
  for (size_t i = 1; i <= 5; i++)
  {
    second_stack.push(i);
  }
  second_stack = first_stack;
  out << second_stack.top();
  second_stack.pop();
  for (size_t i = 1; i < 6; i++)
  {
    out << " " << second_stack.top();
    second_stack.pop();
  }
  BOOST_TEST(out.str() == "6 5 4 3 2 1");
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(stack_push_method)

BOOST_AUTO_TEST_CASE(push_one_element)
{
  std::ostringstream out;
  petrov::Stack< int > stack;
  stack.push(1);
  out << stack.top();
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_CASE(push_three_elements)
{
  std::ostringstream out;
  petrov::Stack< int > stack;
  for (size_t i = 1; i <= 3; i++)
  {
    stack.push(i);
  }
  out << stack.top();
  stack.pop();
  for (size_t i = 1; i < 3; i++)
  {
    out << " " << stack.top();
    stack.pop();
  }
  BOOST_TEST(out.str() == "3 2 1");
}

BOOST_AUTO_TEST_CASE(push_five_elements)
{
  std::ostringstream out;
  petrov::Stack< int > stack;
  for (size_t i = 1; i <= 5; i++)
  {
    stack.push(i);
  }
  out << stack.top();
  stack.pop();
  for (size_t i = 1; i < 5; i++)
  {
    out << " " << stack.top();
    stack.pop();
  }
  BOOST_TEST(out.str() == "5 4 3 2 1");
}

BOOST_AUTO_TEST_CASE(push_sixth_elements)
{
  std::ostringstream out;
  petrov::Stack< int > stack;
  for (size_t i = 1; i <= 6; i++)
  {
    stack.push(i);
  }
  out << stack.top();
  stack.pop();
  for (size_t i = 1; i < 6; i++)
  {
    out << " " << stack.top();
    stack.pop();
  }
  BOOST_TEST(out.str() == "6 5 4 3 2 1");
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(stack_pop_method)

BOOST_AUTO_TEST_CASE(pop_one_element)
{
  std::ostringstream out;
  petrov::Stack< int > stack;
  stack.push(1);
  stack.push(2);
  stack.push(3);
  stack.pop();
  out << stack.top();
  BOOST_TEST(out.str() == "2");
}

BOOST_AUTO_TEST_CASE(pop_three_elements)
{
  std::ostringstream out;
  petrov::Stack< int > stack;
  for (size_t i = 1; i <= 5; i++)
  {
    stack.push(i);
  }
  for (size_t i = 1; i <= 3; i++)
  {
    stack.pop();
  }
  out << stack.top();
  stack.pop();
  out << " " << stack.top();
  BOOST_TEST(out.str() == "2 1");
}

BOOST_AUTO_TEST_CASE(pop_all_elements)
{
  std::ostringstream out;
  petrov::Stack< int > stack;
  for (size_t i = 1; i <= 6; i++)
  {
    stack.push(i);
  }
  for (size_t i = 1; i <= 6; i++)
  {
    stack.pop();
  }
  out << stack.empty();
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(empty_method)

BOOST_AUTO_TEST_CASE(empty_stack)
{
  std::ostringstream out;
  petrov::Stack< int > stack;
  out << stack.empty();
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_CASE(make_empty_stack)
{
  std::ostringstream out;
  petrov::Stack< int > stack;
  stack.push(1);
  stack.pop();
  out << stack.empty();
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_CASE(not_empty_stack)
{
  std::ostringstream out;
  petrov::Stack< int > stack;
  stack.push(0);
  out << stack.empty();
  BOOST_TEST(out.str() == "0");
}

BOOST_AUTO_TEST_SUITE_END()
