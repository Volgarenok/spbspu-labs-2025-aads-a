#include <boost/test/unit_test.hpp>
#include "array.hpp"

using array_t = duhanina::DynamicArray< int >;

BOOST_AUTO_TEST_CASE(array)
{
  array_t array;
  BOOST_TEST(array.size() == 0);
  BOOST_TEST(array.empty());
  array.push_back(1);
  array.push_back(4);
  BOOST_TEST(array.back() == 4);
  BOOST_TEST(array.size() == 2);
  array.pop_back();
  BOOST_TEST(array.front() == 1);
  array.push_back(3);
  array.push_back(6);
  array.pop_front();
  BOOST_TEST(array.front() == 3);
  BOOST_TEST(array.back() == 6);

  array_t array1;
  array1.push_back(2);
  array1.push_back(5);
  array_t copyArray(array1);
  BOOST_TEST(copyArray.size() == 2);
  BOOST_TEST(copyArray.back() == 5);
  BOOST_TEST(array1.size() == 2);

  array_t array2;
  array2.push_back(88);
  array2.push_back(3);
  array_t moveArray(std::move(array2));
  BOOST_TEST(moveArray.size() == 2);
  BOOST_TEST(moveArray.back() == 3);
  BOOST_TEST(array2.size() == 0);

  array_t array3;
  array3.push_back(11);
  array3.push_back(33);
  array_t moveArray1 = std::move(array3);
  BOOST_TEST(moveArray1.size() == 2);
  BOOST_TEST(moveArray1.back() == 33);
  BOOST_TEST(array3.size() == 0);
}
