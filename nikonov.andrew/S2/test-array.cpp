#include <boost/test/unit_test.hpp>
#include "dynamicArray.hpp"
BOOST_AUTO_TEST_CASE(arrayDefaultConstrunct_test)
{
  nikonov::Array< std::string > arr;
  BOOST_TEST(arr.size() == 0);
  BOOST_TEST(arr.capacity() == 10);
}
BOOST_AUTO_TEST_CASE(arrayLCopyConstrunct_test)
{
  nikonov::Array< std::string > arr;
  arr.push_back("a");
  arr.push_back("b");
  nikonov::Array< std::string > copy(arr);
  BOOST_TEST(copy.size() == 2);
  BOOST_TEST(arr.front() == "a");
  BOOST_TEST(arr.back() == "b");
}
BOOST_AUTO_TEST_CASE(arrayRCopyConstrunct_test)
{
  nikonov::Array< std::string > arr;
  arr.push_back("a");
  arr.push_back("b");
  nikonov::Array< std::string > copy(std::move(arr));
  BOOST_TEST(arr.size() == 0);
  BOOST_TEST(copy.size() == 2);
  BOOST_TEST(copy.front() == "a");
  BOOST_TEST(copy.back() == "b");
}
BOOST_AUTO_TEST_CASE(arrayEmpty_test)
{
  nikonov::Array< std::string > arr;
  BOOST_TEST(arr.empty());
  arr.push_back("a");
  BOOST_TEST(!arr.empty());
}
BOOST_AUTO_TEST_CASE(arraySize_test)
{
  nikonov::Array< std::string > arr;
  arr.push_back("a");
  BOOST_TEST(arr.size() == 1);
  arr.push_back("b");
  BOOST_TEST(arr.size() == 2);
  arr.pop_back();
  BOOST_TEST(arr.size() == 1);
}
BOOST_AUTO_TEST_CASE(arrayFront_test)
{
  nikonov::Array< std::string > arr;
  arr.push_back("a");
  BOOST_TEST(arr.front() == "a");
  arr.push_back("b");
  BOOST_TEST(arr.front() == "a");
  arr.pop_front();
  BOOST_TEST(arr.front() == "b");
}
BOOST_AUTO_TEST_CASE(arrayBack_test)
{
  nikonov::Array< std::string > arr;
  arr.push_back("a");
  BOOST_TEST(arr.back() == "a");
  arr.push_back("b");
  BOOST_TEST(arr.front() == "a");
}
BOOST_AUTO_TEST_CASE(arrayPushBack_test)
{
  nikonov::Array< std::string > arr;
  arr.push_back("aboba");
  BOOST_TEST(arr.size() == 1);
  BOOST_TEST(arr.capacity() == 10);
}
BOOST_AUTO_TEST_CASE(arrayPushBackAndRealloc_test)
{
  std::string str = "aboba";
  nikonov::Array< std::string > arr;
  arr.push_back(str);
  arr.push_back(str);
  arr.push_back(str);
  arr.push_back(str);
  arr.push_back(str);
  arr.push_back(str);
  arr.push_back(str);
  arr.push_back(str);
  arr.push_back(str);
  arr.push_back(str);
  arr.push_back(str);
  BOOST_TEST(arr.size() == 11);
  BOOST_TEST(arr.capacity() == 20);
}
BOOST_AUTO_TEST_CASE(arrayEmplace_test)
{
  nikonov::Array< std::string > arr;
  arr.emplace("aboba");
  BOOST_TEST(arr.back() == "aboba");
  BOOST_TEST(arr.size() == 1);
  BOOST_TEST(arr.capacity() == 10);
}
BOOST_AUTO_TEST_CASE(arrayPopFront_test)
{
  nikonov::Array< std::string > arr;
  arr.push_back("a");
  arr.push_back("b");
  arr.push_back("c");
  BOOST_TEST(arr.size() == 3);
  arr.pop_front();
  BOOST_TEST(arr.size() == 2);
  BOOST_TEST(arr.front() == "b");
  BOOST_TEST(arr.back() == "c");
}
BOOST_AUTO_TEST_CASE(arrayPopBack_test)
{
  nikonov::Array< std::string > arr;
  arr.push_back("a");
  arr.push_back("b");
  arr.push_back("c");
  BOOST_TEST(arr.size() == 3);
  arr.pop_back();
  BOOST_TEST(arr.size() == 2);
  BOOST_TEST(arr.back() == "b");
  BOOST_TEST(arr.front() == "a");
}
BOOST_AUTO_TEST_CASE(arraySwap_test)
{
  nikonov::Array< std::string > arr;
  arr.push_back("a");
  arr.push_back("b");
  arr.push_back("c");
  nikonov::Array< std::string > copy;
  copy.swap(arr);
  BOOST_TEST(arr.size() == 0);
  BOOST_TEST(copy.size() == 3);
}
