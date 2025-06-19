#include <boost/test/tools/interface.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include "dynamic_array.hpp"

BOOST_AUTO_TEST_SUITE(dynamic_array)

BOOST_AUTO_TEST_CASE(constructors)
{
  brevnov::Dynamic_array< int > test_array;
  BOOST_TEST(test_array.empty());
  test_array.push(6);
  brevnov::Dynamic_array< int > test_copy_array(test_array);
  BOOST_TEST(test_copy_array.front() == 6);
  brevnov::Dynamic_array< int > test_move_array(std::move(test_copy_array));
  BOOST_TEST(test_move_array.front() == 6);
}

BOOST_AUTO_TEST_CASE(front_back)
{
  brevnov::Dynamic_array< int > test_array;
  test_array.push(5);
  test_array.push(2);
  const int i = test_array.front();
  BOOST_TEST(i == 5);
  BOOST_TEST(test_array.front() == 5);
  const int a = test_array.back();
  BOOST_TEST(a == 2);
  BOOST_TEST(test_array.back() == 2);
}

BOOST_AUTO_TEST_CASE(assignment_operators)
{
  brevnov::Dynamic_array< int > test_array;
  brevnov::Dynamic_array< int > test_array2;
  test_array2.push(9);
  test_array = test_array2;
  BOOST_TEST(test_array.front() == 9);
  test_array.push(8);
  test_array2 = test_array;
  BOOST_TEST(test_array2.back() == 8);
}

BOOST_AUTO_TEST_CASE(push_and_pop)
{
  brevnov::Dynamic_array< int > test_array;
  int i = 2;
  test_array.push(i);
  BOOST_TEST(test_array.front() == 2);
  test_array.push(4);
  test_array.push(8);
  BOOST_TEST(test_array.back() == 8);
  test_array.popFront();
  BOOST_TEST(test_array.front() == 4);
  test_array.popBack();
  BOOST_TEST(test_array.back() == 4);
}

BOOST_AUTO_TEST_CASE(empty_size_clear)
{
  brevnov::Dynamic_array< int > test_array;
  for (int i = 0; i < 15; ++i)
  {
    test_array.push(i);
  }
  BOOST_TEST(test_array.size() == 15);
  test_array.clear();
  BOOST_TEST(test_array.empty());
}

BOOST_AUTO_TEST_SUITE_END()
