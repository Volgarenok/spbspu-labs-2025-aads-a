#include <boost/test/unit_test.hpp>
#include <sstream>

#include <stack.hpp>

template < typename T >
std::string to_string(const zholobov::Stack< T >& stack)
{
  std::stringstream ss;
  zholobov::Stack< T > temp(stack);
  if (!temp.empty()) {
    ss << temp.top();
    temp.pop();
    while (!temp.empty()) {
      ss << " " << temp.top();
      temp.pop();
    }
  }
  return ss.str();
}

BOOST_AUTO_TEST_SUITE(Stack_Constructors)

BOOST_AUTO_TEST_CASE(default_ctor)
{
  zholobov::Stack< std::string > stack;
  BOOST_TEST(stack.empty());
  BOOST_TEST(to_string(stack) == "");
}

BOOST_AUTO_TEST_CASE(copy_ctor)
{
  zholobov::Stack< std::string > stack;
  stack.push("This");
  stack.push("is");
  stack.push("content");
  zholobov::Stack< std::string > new_stack(stack);
  BOOST_TEST(to_string(stack) == "content is This");
  BOOST_TEST(to_string(new_stack) == "content is This");
}

BOOST_AUTO_TEST_CASE(move_ctor)
{
  zholobov::Stack< std::string > stack;
  stack.push("This");
  stack.push("is");
  stack.push("content");
  zholobov::Stack< std::string > new_stack(std::move(stack));
  BOOST_TEST(to_string(stack) == "");
  BOOST_TEST(to_string(new_stack) == "content is This");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(Stack_Assign_Operators)

BOOST_AUTO_TEST_CASE(copy_assign_operator)
{
  zholobov::Stack< std::string > stack;
  stack.push("This");
  stack.push("is");
  stack.push("content");
  zholobov::Stack< std::string > new_stack = stack;
  BOOST_TEST(to_string(stack) == "content is This");
  BOOST_TEST(to_string(new_stack) == "content is This");
  stack.push("of");
  stack.push("a");
  stack.push("stack");
  new_stack = stack;
  BOOST_TEST(to_string(stack) == "stack a of content is This");
  BOOST_TEST(to_string(new_stack) == "stack a of content is This");
  new_stack = zholobov::Stack< std::string >();
  BOOST_TEST(to_string(new_stack) == "");
}

BOOST_AUTO_TEST_CASE(move_assign_operator)
{
  zholobov::Stack< std::string > stack;
  stack.push("This");
  stack.push("is");
  stack.push("content");
  zholobov::Stack< std::string > new_stack = std::move(stack);
  BOOST_TEST(to_string(stack) == "");
  BOOST_TEST(to_string(new_stack) == "content is This");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(Stack_Opeartions)

BOOST_AUTO_TEST_CASE(push_copy)
{
  zholobov::Stack< std::string > stack;
  std::string val = "Something";
  stack.push(val);
  BOOST_TEST(to_string(stack) == "Something");
  BOOST_TEST(val == "Something");
}

BOOST_AUTO_TEST_CASE(push_move)
{
  zholobov::Stack< std::string > stack;
  std::string val = "Something";
  stack.push(std::move(val));
  BOOST_TEST(to_string(stack) == "Something");
  BOOST_TEST(val == "");
}

BOOST_AUTO_TEST_CASE(top_pop)
{
  zholobov::Stack< std::string > stack;
  stack.push("This");
  BOOST_TEST(stack.top() == "This");
  stack.push("is");
  BOOST_TEST(stack.top() == "is");
  stack.push("content");
  BOOST_TEST(stack.top() == "content");
  stack.pop();
  BOOST_TEST(stack.top() == "is");
  stack.pop();
  BOOST_TEST(stack.top() == "This");
}

BOOST_AUTO_TEST_CASE(empty)
{
  zholobov::Stack< std::string > stack;
  BOOST_TEST(stack.empty());
  stack.push("Something");
  BOOST_TEST(!stack.empty());
  stack.pop();
  BOOST_TEST(stack.empty());
}

BOOST_AUTO_TEST_CASE(clear)
{
  zholobov::Stack< std::string > stack;
  stack.push("One");
  BOOST_TEST(!stack.empty());
  stack.clear();
  BOOST_TEST(stack.empty());
  stack.push("Two");
  BOOST_TEST(!stack.empty());
  stack.clear();
  BOOST_TEST(stack.empty());
}

BOOST_AUTO_TEST_SUITE_END()
