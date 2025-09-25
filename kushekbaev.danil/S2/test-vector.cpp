#include <boost/test/unit_test.hpp>
#include <vector.hpp>

using namespace kushekbaev;

BOOST_AUTO_TEST_SUITE(Constructors)

BOOST_AUTO_TEST_CASE(DefaultConstructor)
{
  Vector< int > v;
  BOOST_TEST(v.empty());
  BOOST_TEST(v.size() == 0);
}

BOOST_AUTO_TEST_CASE(CopyConstructor)
{
  Vector< int > original;
  original.pushBack(10);
  original.pushBack(20);
  Vector< int > copy(original);
  BOOST_TEST(original.size() == 2);
  BOOST_TEST(copy.size() == 2);
  BOOST_TEST(copy.front() == 10);
  BOOST_TEST(copy.back() == 20);
  copy.popBack();
  BOOST_TEST(original.size() == 2);
  BOOST_TEST(copy.size() == 1);
}

BOOST_AUTO_TEST_CASE(MoveConstructor)
{
  Vector< int > original;
  original.pushBack(10);
  original.pushBack(20);
  Vector< int > moved(std::move(original));
  BOOST_TEST(moved.size() == 2);
  BOOST_TEST(moved.front() == 10);
  BOOST_TEST(moved.back() == 20);
  BOOST_TEST(original.size() == 0);
  BOOST_TEST(original.empty());
  moved.pushBack(30);
  BOOST_TEST(moved.size() == 3);
  BOOST_TEST(moved.back() == 30);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_CASE(AssignmentOperator)
{
  Vector< int > v1;
  v1.pushBack(10);
  v1.pushBack(20);
  Vector< int > v2;
  v2 = v1;
  BOOST_TEST(v2.size() == 2);
  BOOST_TEST(v2.front() == 10);
  BOOST_TEST(v2.back() == 20);
  Vector< int > v3;
  v3 = std::move(v1);
  BOOST_TEST(v3.size() == 2);
  BOOST_TEST(v1.size() == 0);
}

BOOST_AUTO_TEST_SUITE(ElementAccess)

BOOST_AUTO_TEST_CASE(FrontAndBack)
{
  Vector< int > v;
  v.pushBack(10);
  BOOST_TEST(v.front() == 10);
  BOOST_TEST(v.back() == 10);
  v.pushBack(20);
  BOOST_TEST(v.front() == 10);
  BOOST_TEST(v.back() == 20);
  v.pushBack(30);
  BOOST_TEST(v.front() == 10);
  BOOST_TEST(v.back() == 30);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(Capacity)

BOOST_AUTO_TEST_CASE(EmptyAndSize)
{
  Vector< int > v;
  BOOST_TEST(v.empty());
  BOOST_TEST(v.size() == 0);
  v.pushBack(10);
  BOOST_TEST(!v.empty());
  BOOST_TEST(v.size() == 1);
  v.popBack();
  BOOST_TEST(v.empty());
  BOOST_TEST(v.size() == 0);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(Modifiers)

BOOST_AUTO_TEST_CASE(PushBackAndPop)
{
  Vector< int > v;
  v.pushBack(100);
  BOOST_TEST(v.back() == 100);
  v.pushBack(200);
  BOOST_TEST(v.back() == 200);
  v.pushBack(300);
  BOOST_TEST(v.back() == 300);
  BOOST_TEST(v.size() == 3);
  v.popBack();
  BOOST_TEST(v.back() == 200);
  BOOST_TEST(v.size() == 2);
  v.popFront();
  BOOST_TEST(v.front() == 200);
  BOOST_TEST(v.back() == 200);
  BOOST_TEST(v.size() == 1);
}

BOOST_AUTO_TEST_CASE(ExtendVectorCapacity)
{
  Vector< int > v;
  for (int i = 0; i < 20; ++i)
  {
    v.pushBack(i);
  }
  BOOST_TEST(v.size() == 20);
  BOOST_TEST(v.front() == 0);
  BOOST_TEST(v.back() == 19);
}

BOOST_AUTO_TEST_SUITE_END()
