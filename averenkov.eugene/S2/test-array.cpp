#include <boost/test/unit_test.hpp>
#include <cstddef>
#include "array.hpp"

using array_int = averenkov::Array< int >;

bool isSame(const array_int lhs, const array_int rhs)
{
  if (!(lhs.front() == rhs.front() && lhs.back() == rhs.back()))
  {
    return false;
  }
  return true;
}

BOOST_AUTO_TEST_SUITE()

BOOST_AUTO_TEST_CASE(EmptyAndSize)
{
  array_int array;
  BOOST_TEST(array.empty() == (array.size() == 0));
}

BOOST_AUTO_TEST_CASE(Constructors)
{
  array_int array1;
  BOOST_TEST(array1.empty());

  array1.push_back(3);
  array_int array2(array1);
  BOOST_TEST(isSame(array2, array1));

  array_int array3;
  array3 = array1;
  BOOST_TEST(isSame(array3, array1));

  array_int array4(std::move(array1));
  BOOST_TEST(array4.front() == 3);
  BOOST_TEST(array1.empty());
}

BOOST_AUTO_TEST_CASE(Push_back)
{
  array_int array1;
  array1.push_back(4);
  BOOST_TEST(array1.back() == 4);

  array1.push_back(2);
  array1.push_back(3);
  BOOST_TEST(array1.front() == 4);
  BOOST_TEST(array1.back() == 3);
}

BOOST_AUTO_TEST_CASE(Pop)
{
  array_int array1;
  array1.push_back(3);
  array1.push_back(4);
  array1.push_back(5);
  BOOST_TEST(array1.front() == 3);
  BOOST_TEST(array1.back() == 5);
  array1.pop_front();
  BOOST_TEST(array1.front() == 4);
  array1.pop_back();
  BOOST_TEST(array1.back() == 4);

}

BOOST_AUTO_TEST_SUITE_END()
