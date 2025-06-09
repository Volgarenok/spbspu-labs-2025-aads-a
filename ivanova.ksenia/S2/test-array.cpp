#include <algorithm>
#include <boost/test/unit_test.hpp>

#include "array.hpp"

BOOST_AUTO_TEST_CASE(ArrayDefaultConstructor)
{
  ivanova::Array<int> array;
  BOOST_CHECK_EQUAL(0, array.size());
}

template <typename Iter1, typename Iter2>
bool areEqual(Iter1 first1, Iter1 last1, Iter2 first2, Iter2 last2)
{
  while (first1 != last1 && first2 != last2 && *first1 == *first2)
  {
    ++first1;
    ++first2;
  }
  return first1 == last1 && first2 == last2;
}

BOOST_AUTO_TEST_CASE(ArrayCopyConstructor)
{
  ivanova::Array<int> array;
  array.push_back(1);
  ivanova::Array<int> copy = array;

  BOOST_CHECK(areEqual(array.begin(), array.end(), copy.begin(), copy.end()));
  array.push_back(2);
  BOOST_CHECK(!areEqual(array.begin(), array.end(), copy.begin(), copy.end()));
}

BOOST_AUTO_TEST_CASE(ArrayMoveConstructor)
{
  ivanova::Array<int> array;
  array.push_back(10);
  ivanova::Array<int> move = std::move(array);

  BOOST_CHECK_EQUAL(move.size(), 1);
  BOOST_CHECK_EQUAL(array.size(), 0);
  BOOST_CHECK_EQUAL(move.back(), 10);
}

BOOST_AUTO_TEST_CASE(ArrayAssignOperator)
{
  ivanova::Array<int> a;
  a.push_back(10);
  ivanova::Array<int> b;
  b.push_back(20);
  b.push_back(30);

  a = b;
  BOOST_CHECK(areEqual(a.begin(), a.end(), b.begin(), b.end()));
  b.push_back(40);
  BOOST_CHECK(!areEqual(a.begin(), a.end(), b.begin(), b.end()));
}

BOOST_AUTO_TEST_CASE(ArrayAssignSelfOperator)
{
  ivanova::Array<int> a;
  a.push_back(10);
  
  a = a;
  BOOST_CHECK_EQUAL(a.size(), 1);
  BOOST_CHECK_EQUAL(a[0], 10);
}

BOOST_AUTO_TEST_CASE(ArrayAssignMoveOperator)
{
  ivanova::Array<int> a;
  a.push_back(10);
  ivanova::Array<int> b;
  b.push_back(20);
  b.push_back(30);

  b = std::move(a);
  BOOST_CHECK_EQUAL(b.size(), 1);
  BOOST_CHECK_EQUAL(b[0], 10);
  BOOST_CHECK(a.empty());
}

BOOST_AUTO_TEST_CASE(ArrayPushBack)
{
  int array[10];
  ivanova::Array<int> a;

  for (int i = 0; i < 10; ++i)
  {
    a.push_back(i * 10);
    array[i] = i * 10;
    BOOST_CHECK(areEqual(a.begin(), a.end(), array, array + i + 1));
  }
}

BOOST_AUTO_TEST_CASE(ArrayPushPop)
{
  int array[10];
  ivanova::Array<int> a;

  for (int i = 0; i < 10; ++i)
  {
    a.push_back(i * 10);
    array[i] = i * 10;
  }
  for (int i = 0; i < 10; ++i)
  {
    a.pop_back();
    BOOST_CHECK(areEqual(a.begin(), a.end(), array, array + 9 - i));
  }
}

BOOST_AUTO_TEST_CASE(ArrayIndexer)
{
  int array[10];
  ivanova::Array<int> a;

  for (int i = 0; i < 10; ++i)
  {
    a.push_back(i * 10);
    array[i] = i * 10;
  }
  for (int i = 0; i < 10; ++i)
  {
    BOOST_CHECK_EQUAL(array[i], a[i]);
  }
}
