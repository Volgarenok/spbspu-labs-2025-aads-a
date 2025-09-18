#include <boost/test/unit_test.hpp>
#include <dynamic-array.hpp>

using namespace savintsev;

BOOST_AUTO_TEST_CASE(da_test_all)
{
  Array< int > m(2);

  BOOST_TEST(m.size() == 0);
  m.push_back(123);
  BOOST_TEST(m.back() == 123);
  BOOST_TEST(m.size() == 1);
  m.push_back(456);
  BOOST_TEST(m.back() == 456);
  BOOST_TEST(m.front() == 123);
  m.push_back(789);
  BOOST_TEST(m.back() == 789);
  BOOST_TEST(m.front() == 123);
  BOOST_TEST(m.size() == 3);
  m.pop_front();
  BOOST_TEST(m.front() == 456);
  BOOST_TEST(m.size() == 2);
  m.push_back(999);
  BOOST_TEST(m.back() == 999);
  m.pop_back();
  BOOST_TEST(m.back() == 789);

  Array< int > n(m);

  BOOST_TEST(n.front() == 456);
  BOOST_TEST(n.back() == 789);
}

BOOST_AUTO_TEST_CASE(da_test_operator_sq)
{
  Array< int > m(2);

  m.push_back(10);
  m.push_back(-20);
  m.push_back(30);

  BOOST_TEST(m[0] == 10);
  BOOST_TEST(m[1] == -20);
  BOOST_TEST(m[2] == 30);

  m.push_back(40);

  m.pop_front();
  m.pop_front();

  BOOST_TEST(m[0] == 30);
  BOOST_TEST(m[1] == 40);
}

BOOST_AUTO_TEST_CASE(array_basic_operations)
{
  Array< int > arr(2);
  arr.push_back(1);
  arr.push_back(2);

  BOOST_TEST(arr.size() == 2);
  BOOST_TEST(arr[0] == 1);

  arr.pop_front();
  BOOST_TEST(arr.size() == 1);
  BOOST_TEST(arr[0] == 2);

  arr.push_back(3);
  BOOST_TEST(arr.size() == 2);
  BOOST_TEST(arr[1] == 3);
}

BOOST_AUTO_TEST_CASE(array_copy_move)
{
  Array< int > arr1(3);
  arr1.push_back(1);
  arr1.push_back(2);

  Array< int > arr2(arr1);
  BOOST_TEST(arr2.size() == 2);
  BOOST_TEST(arr2[1] == 2);

  Array< int > arr3(std::move(arr1));
  BOOST_TEST(arr3.size() == 2);
  BOOST_TEST(arr1.size() == 0);
}

BOOST_AUTO_TEST_CASE(stress_test)
{
  Array< int > arr1(2);

  for (int i = 1; i < 1001; ++i)
  {
    arr1.push_back(i);
  }

  for (int i = 0; i < 1000; ++i)
  {
    BOOST_TEST(arr1[i] == i + 1);
  }
}
