
#include <boost/test/unit_test.hpp>
#include <sstream>

#include <array.hpp>

template < typename T >
std::string to_string(const zholobov::Array< T >& array)
{
  std::stringstream ss;
  zholobov::Array< T > temp(array);
  if (!temp.empty()) {
    ss << temp.front();
    temp.pop_front();
    while (!temp.empty()) {
      ss << " " << temp.front();
      temp.pop_front();
    }
  }
  return ss.str();
}

BOOST_AUTO_TEST_SUITE(Array_Constructors)

BOOST_AUTO_TEST_CASE(default_ctor)
{
  zholobov::Array< std::string > array;
  BOOST_TEST(array.empty());
  BOOST_TEST(to_string(array) == "");
}

BOOST_AUTO_TEST_CASE(copy_ctor)
{
  zholobov::Array< std::string > array;
  array.push_back("This");
  array.push_back("is");
  array.push_back("content");
  zholobov::Array< std::string > new_array(array);
  BOOST_TEST(to_string(array) == "This is content");
  BOOST_TEST(to_string(new_array) == "This is content");
}

BOOST_AUTO_TEST_CASE(move_ctor)
{
  zholobov::Array< std::string > array;
  array.push_back("This");
  array.push_back("is");
  array.push_back("content");
  zholobov::Array< std::string > new_array(std::move(array));
  BOOST_TEST(to_string(array) == "");
  BOOST_TEST(to_string(new_array) == "This is content");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(Array_Assign_Operators)

BOOST_AUTO_TEST_CASE(copy_assign_operator)
{
  zholobov::Array< std::string > array;
  array.push_back("This");
  array.push_back("is");
  array.push_back("content");
  zholobov::Array< std::string > new_array = array;
  BOOST_TEST(to_string(array) == "This is content");
  BOOST_TEST(to_string(new_array) == "This is content");
  array.push_back("of");
  array.push_back("a");
  array.push_back("array");
  new_array = array;
  BOOST_TEST(to_string(array) == "This is content of a array");
  BOOST_TEST(to_string(new_array) == "This is content of a array");
  new_array = zholobov::Array< std::string >();
  BOOST_TEST(to_string(new_array) == "");
}

BOOST_AUTO_TEST_CASE(move_assign_operator)
{
  zholobov::Array< std::string > array;
  array.push_back("This");
  array.push_back("is");
  array.push_back("content");
  zholobov::Array< std::string > new_array = std::move(array);
  BOOST_TEST(to_string(array) == "");
  BOOST_TEST(to_string(new_array) == "This is content");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(Array_Opeartions)

BOOST_AUTO_TEST_CASE(push_copy)
{
  zholobov::Array< std::string > array;
  std::string val = "Something";
  array.push_back(val);
  BOOST_TEST(to_string(array) == "Something");
  BOOST_TEST(val == "Something");
}

BOOST_AUTO_TEST_CASE(push_move)
{
  zholobov::Array< std::string > array;
  std::string val = "Something";
  array.push_back(std::move(val));
  BOOST_TEST(to_string(array) == "Something");
  BOOST_TEST(val == "");
}

BOOST_AUTO_TEST_CASE(front_pop_front)
{
  zholobov::Array< std::string > array;
  array.push_back("This");
  BOOST_TEST(array.front() == "This");
  array.push_back("is");
  BOOST_TEST(array.front() == "This");
  array.push_back("content");
  BOOST_TEST(array.front() == "This");
  array.pop_front();
  BOOST_TEST(array.front() == "is");
  array.pop_front();
  BOOST_TEST(array.front() == "content");
}

BOOST_AUTO_TEST_CASE(many_push_pop)
{
  zholobov::Array< int > array;
  for (int i = 0; i < 20; ++i) {
    array.push_back(i);
  }
  for (int i = 0; i < 10; ++i) {
    array.pop_front();
  }
  for (int i = 0; i < 40; ++i) {
    array.push_back(i);
  }
  for (int i = 0; i < 48; ++i) {
    array.pop_front();
  }
  BOOST_TEST(to_string(array) == "38 39");
}

BOOST_AUTO_TEST_CASE(empty)
{
  zholobov::Array< std::string > array;
  BOOST_TEST(array.empty());
  array.push_back("Something");
  BOOST_TEST(!array.empty());
  array.pop_front();
  BOOST_TEST(array.empty());
  array.push_back("Other");
  BOOST_TEST(!array.empty());
  array.pop_back();
  BOOST_TEST(array.empty());
}

BOOST_AUTO_TEST_CASE(clear)
{
  zholobov::Array< std::string > stack;
  stack.push_back("One");
  BOOST_TEST(!stack.empty());
  stack.clear();
  BOOST_TEST(stack.empty());
  stack.push_back("Two");
  BOOST_TEST(!stack.empty());
  stack.clear();
  BOOST_TEST(stack.empty());
}

BOOST_AUTO_TEST_SUITE_END()
