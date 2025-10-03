#include <boost/test/unit_test.hpp>
#include <array.hpp>

using array_t = duhanina::DynamicArray< int >;

BOOST_AUTO_TEST_CASE(DefaultConstructor)
{
  array_t arr;
  BOOST_TEST(arr.empty());
}

BOOST_AUTO_TEST_CASE(PushBackAndSize)
{
  array_t arr;
  arr.push_back(1);
  BOOST_TEST(arr.size() == 1);
  BOOST_TEST(arr.back() == 1);

  arr.push_back(2);
  BOOST_TEST(arr.size() == 2);
  BOOST_TEST(arr.back() == 2);
}

BOOST_AUTO_TEST_CASE(FrontAndBack)
{
  array_t arr;
  arr.push_back(1);
  arr.push_back(2);
  arr.push_back(3);
  BOOST_TEST(arr.front() == 1);
  BOOST_TEST(arr.back() == 3);
}

BOOST_AUTO_TEST_CASE(PopBack)
{
  array_t arr;
  arr.push_back(1);
  arr.push_back(2);
  arr.pop_back();
  BOOST_TEST(arr.size() == 1);
  BOOST_TEST(arr.back() == 1);
}

BOOST_AUTO_TEST_CASE(PopFront)
{
  array_t arr;
  arr.push_back(1);
  arr.push_back(2);
  arr.push_back(3);
  arr.pop_front();
  BOOST_TEST(arr.size() == 2);
  BOOST_TEST(arr.front() == 2);
}

BOOST_AUTO_TEST_CASE(CopyConstructor)
{
  array_t orig;
  orig.push_back(1);
  orig.push_back(2);
  array_t copy(orig);
  BOOST_TEST(copy.size() == 2);
}

BOOST_AUTO_TEST_CASE(MoveConstructor)
{
  array_t orig;
  orig.push_back(1);
  orig.push_back(2);
  array_t moved(std::move(orig));
  BOOST_TEST(moved.size() == 2);
  BOOST_TEST(orig.empty());
}

BOOST_AUTO_TEST_CASE(CopyAssignment)
{
  array_t a1;
  a1.push_back(1);
  a1.push_back(2);
  array_t a2;
  a2 = a1;
  BOOST_TEST(a2.size() == 2);
}

BOOST_AUTO_TEST_CASE(MoveAssignment)
{
  array_t a1;
  a1.push_back(1);
  a1.push_back(2);
  array_t a2;
  a2 = std::move(a1);
  BOOST_TEST(a2.size() == 2);
  BOOST_TEST(a1.empty());
}

BOOST_AUTO_TEST_CASE(ResizeTest)
{
  array_t arr;
  const size_t initCapacity = 50;
  for (size_t i = 0; i < initCapacity; ++i)
  {
    arr.push_back(1);
  }
  BOOST_TEST(arr.size() == initCapacity);
  arr.push_back(2);
  BOOST_TEST(arr.size() == initCapacity + 1);
}

BOOST_AUTO_TEST_CASE(Empty)
{
  array_t arr;
  BOOST_TEST(arr.empty());
  BOOST_TEST(arr.size() == 0);
}
