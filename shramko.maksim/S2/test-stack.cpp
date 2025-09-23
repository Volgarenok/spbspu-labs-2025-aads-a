#include <boost/test/unit_test.hpp>
#include <sstream>
#include "stack.hpp"

namespace shramko
{
  void outputStackString(std::ostream& out, Stack< std::string >& stack)
  {
    if (stack.empty())
    {
      return;
    }
    out << stack.top();
    stack.pop();
    while (!stack.empty())
    {
      out << " " << stack.top();
      stack.pop();
    }
  }
}

BOOST_AUTO_TEST_CASE(stack_basic_constructor_test)
{
  shramko::Stack< std::string > stack;
  BOOST_TEST(stack.empty());
  BOOST_TEST(stack.size() == 0);
}

BOOST_AUTO_TEST_CASE(stack_pop_test)
{
  shramko::Stack< std::string > stack;
  stack.push("1");
  stack.push("2");
  stack.push("3");
  stack.push("4");
  stack.push("5");
  BOOST_TEST(stack.size() == 5);
  stack.pop();
  BOOST_TEST(stack.size() == 4);
  std::ostringstream out;
  shramko::outputStackString(out, stack);
  BOOST_TEST(out.str() == "4 3 2 1");
}

BOOST_AUTO_TEST_CASE(stack_lvalue_push_test)
{
  shramko::Stack< std::string > stack;
  std::string temp = "3";
  stack.push(temp);
  std::ostringstream out;
  shramko::outputStackString(out, stack);
  BOOST_TEST(out.str() == "3");
  BOOST_TEST(stack.size() == 0);
}

BOOST_AUTO_TEST_CASE(stack_top_size_test)
{
  shramko::Stack< std::string > stack;
  stack.push("2");
  stack.push("3");
  BOOST_TEST(stack.top() == "3");
  BOOST_TEST(stack.size() == 2);
}

BOOST_AUTO_TEST_CASE(stack_move_push_test)
{
  shramko::Stack< std::string > stack;
  stack.push("1");
  BOOST_TEST(!stack.empty());
  BOOST_TEST(stack.size() == 1);
  BOOST_TEST(stack.top() == "1");
  stack.push("2");
  BOOST_TEST(stack.top() == "2");
  BOOST_TEST(stack.size() == 2);
}
