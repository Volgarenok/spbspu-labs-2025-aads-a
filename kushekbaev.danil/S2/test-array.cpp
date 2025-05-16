#include <boost/test/unit_test.hpp>
#include "array.hpp"

using namespace kushekbaev;

BOOST_AUTO_TEST_SUITE(arrayConstructors)

BOOST_AUTO_TEST_CASE(arrayDefaultConstructor)
{
  Array< int > array;
  BOOST_TEST(array.empty());
}

BOOST_AUTO_TEST_CASE(arrayCopyConstructor)
{
  Array< int > array;
  array.push_back(10);
  array.push_back(20);
  Array< int > arraycopy(array);
  BOOST_TEST(array.front() == arraycopy.front());
  BOOST_TEST(array.back() == arraycopy.back());
  BOOST_TEST(array.size() == arraycopy.size());
  BOOST_TEST(arraycopy.front() == 10);
  BOOST_TEST(arraycopy.back() == 20);
  BOOST_TEST(arraycopy.size() == 2);
}

BOOST_AUTO_TEST_CASE(arrayMoveConstructor)
{
  Array< int > array;
  array.push_back(10);
  array.push_back(20);
  Array< int > arraycopy(std::move(array));
  BOOST_TEST(array.size() == 0);
  BOOST_TEST(arraycopy.front() == 10);
  BOOST_TEST(arraycopy.back() == 20);
  BOOST_TEST(arraycopy.size() == 2);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_CASE(arrayAssignment)
{
  Array< int > array1;
  array1.push_back(10);
  Array< int > array2;
  array2 = array1;
  BOOST_TEST(array2.front() == 10);
}

BOOST_AUTO_TEST_SUITE(arrayElementAccess)

BOOST_AUTO_TEST_CASE(arrayFront)
{
  Array< int > array;
  array.push_back(10);
  BOOST_TEST(array.front() == 10);
  array.push_back(20);
  BOOST_TEST(array.front() == 10);
}

BOOST_AUTO_TEST_CASE(arrayBack)
{
  Array< int > array;
  array.push_back(10);
  BOOST_TEST(array.back() == 10);
  array.push_back(20);
  BOOST_TEST(array.back() == 20);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(arrayCapacity)

BOOST_AUTO_TEST_CASE(arrayEmpty)
{
  Array< int > array;
  BOOST_TEST(array.empty());
  array.push_back(10);
  BOOST_TEST(!array.empty());
  array.pop_back();
  BOOST_TEST(array.empty());
}

BOOST_AUTO_TEST_CASE(arraySize)
{
  Array< int > array;
  BOOST_TEST(array.size() == 0);
  array.push_back(10);
  array.push_back(10);
  BOOST_TEST(array.size() == 2);
  array.pop_back();
  BOOST_TEST(array.size() == 1);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_CASE(arrayPushPop)
{
  Array< int > array;
  array.push_back(1000);
  BOOST_TEST(array.back() == 1000);
  array.push_back(22222);
  BOOST_TEST(array.back() == 22222);
  array.push_back(1337);
  BOOST_TEST(array.back() == 1337);
  array.pop_back();
  BOOST_TEST(array.back() == 22222);
  array.pop_front();
  BOOST_TEST(array.back() == array.front());
}
