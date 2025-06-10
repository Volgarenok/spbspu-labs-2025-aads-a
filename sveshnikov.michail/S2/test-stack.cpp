#include <boost/test/unit_test.hpp>
#include <string>
#include "stack.hpp"

BOOST_AUTO_TEST_SUITE(special_methods_stack)
BOOST_AUTO_TEST_CASE(constructors)
{
  sveshnikov::Stack< char > stack;
  BOOST_TEST(stack.empty());

  stack.push('p');
  stack.push('r');
  sveshnikov::Stack< char > stack2(stack);
  BOOST_TEST(stack2.getSize() == 2);
  BOOST_TEST(stack2.top() == 'r');

  sveshnikov::Stack< char > stack3(std::move(stack));
  BOOST_TEST(stack.empty());
  BOOST_TEST(stack3.getSize() == 2);
  BOOST_TEST(stack3.top() == 'r');
}

BOOST_AUTO_TEST_CASE(operators)
{
  sveshnikov::Stack< int > stack1;
  stack1.push(1);
  stack1.push(2);
  sveshnikov::Stack< int > stack2;
  stack2 = stack1;
  BOOST_TEST(!stack1.empty());
  BOOST_TEST(stack2.getSize() == 2);
  BOOST_TEST(stack2.top() == 2);

  sveshnikov::Stack< int > stack3;
  stack3 = std::move(stack1);
  BOOST_TEST(stack1.empty());
  BOOST_TEST(stack3.getSize() == 2);
  BOOST_TEST(stack2.top() == 2);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(other_methods_stack)
BOOST_AUTO_TEST_CASE(push)
{
  sveshnikov::Stack< std::string > stack;
  stack.push("pobeda");
  BOOST_TEST(stack.top() == "pobeda");
  std::string str = "ura ura";
  stack.push(str);
  BOOST_TEST(stack.getSize() == 2);
  BOOST_TEST(stack.top() == "ura ura");
}

BOOST_AUTO_TEST_CASE(pop)
{
  sveshnikov::Stack< std::string > stack;
  stack.push("slovo1");
  stack.push("slovo2");
  stack.pop();
  BOOST_TEST(stack.top() == "slovo1");
  BOOST_TEST(stack.getSize() == 1);
}

BOOST_AUTO_TEST_CASE(swap)
{
  sveshnikov::Stack< std::string > stack1;
  stack1.push("slovo1");
  stack1.push("slovo2");
  sveshnikov::Stack< std::string > stack2;
  stack2.push("drugoe_slovo1");
  stack1.swap(stack2);
  BOOST_TEST(stack1.getSize() == 1);
  BOOST_TEST(stack2.getSize() == 2);
  BOOST_TEST(stack1.top() == "drugoe_slovo1");
  BOOST_TEST(stack2.top() == "slovo2");
}
BOOST_AUTO_TEST_SUITE_END()
