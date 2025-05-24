#include <boost/test/unit_test.hpp>
#include <string>
#include <iostream>
#include <array.hpp>

BOOST_AUTO_TEST_CASE(test_basic)
{
  using array_t = alymova::Array< std::string >;
  array_t array1;
  array_t array2(array1);
  BOOST_TEST(array1.empty());
  BOOST_TEST(array2.empty());

  for (int i = 0; i < 5; i++)
  {
    std::string s(1ull, '1' + i);
    array1.push_back(s);
  }
  array_t array3(array1);
  BOOST_TEST(array1.size() == 5);
  BOOST_TEST(array1.front() == "1");
  BOOST_TEST(array1.back() == "5");
  BOOST_TEST(array3.size() == 5);
  BOOST_TEST(array3.front() == "1");
  BOOST_TEST(array3.back() == "5");

  for (size_t i = 5; i < 9; i++)
  {
    std::string s(1ull, '1' + i);
    array1.push_back(s);
  }
  array1.push_back("10");
  BOOST_TEST(array1.back() == "10");

  array1.pop_front();
  array1.push_back("11");
  array_t array4(array1);
  BOOST_TEST(array4.size() == 10);
  BOOST_TEST(array4.front() == "2");
  BOOST_TEST(array4.back() == "11");

  array1.pop_back();
  BOOST_TEST(array1.size() == 9);
  BOOST_TEST(array1.back() == "10");

  array_t array5(array_t{});
  BOOST_TEST(array5.empty());

  array5 = array1;
  BOOST_TEST(array5.size() == 9);
  BOOST_TEST(array5.front() == "2");
  BOOST_TEST(array5.back() == "10");

  array5 = array_t();
  BOOST_TEST(array5.empty());

  array1.push_back("11");
  BOOST_TEST(array1.back() == "11");

  array1.pop_back();
  array1.pop_back();
  BOOST_TEST(array1.back() == "9");
}
BOOST_AUTO_TEST_CASE(test_emplace)
{
  using array_t = alymova::Array< std::string >;
  array_t array1;
  array1.push_back("1");
  BOOST_TEST(array1.size() == 1);
  BOOST_TEST(array1.front() == "1");
  BOOST_TEST(array1.back() == "1");

  for (size_t i = 1; i < 9; i++)
  {
    std::string s(1ull, '1' + i);
    array1.push_back(s);
  }
  array1.push_back("10");
  array_t array2 = array1;
  array_t array3 = array1;

  array1.push_back("11");
  BOOST_TEST(array1.back() == "11");

  while (array2.size() != 1)
  {
    array2.pop_front();
  }
  while (array2.size() != 10)
  {
    array2.push_back("1");
  }
  array2.push_back("2");
  BOOST_TEST(array2.size() == 11);
  BOOST_TEST(array2.back() == "2");

  array3.pop_front();
  array3.push_back("11");
  array3.push_back("12");
  BOOST_TEST(array3.size() == 11);
  BOOST_TEST(array3.back() == "12");
}
