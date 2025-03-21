#include <boost/test/unit_test.hpp>
#include "iterator.hpp"

BOOST_AUTO_TEST_CASE(pre_increment)
{
  abramov::List< int > list;
  list.pushBack(0);
  list.pushBack(1);
  auto iter = list.begin();
  BOOST_TEST(*iter == 0);
  ++iter;
  BOOST_TEST(*iter == 1);
}

BOOST_AUTO_TEST_CASE(post_increment)
{
  abramov::List< int > list;
  list.pushBack(0);
  list.pushBack(1);
  auto iter = list.begin();
  BOOST_TEST(*(iter++) == 0);
  BOOST_TEST(*iter == 1);
}

BOOST_AUTO_TEST_CASE(pre_decrement)
{
  abramov::List< int > list;
  list.pushBack(0);
  list.pushBack(1);
  auto iter = list.end();
  --iter;
  BOOST_TEST(*iter == 1);
  --iter;
  BOOST_TEST(*iter == 0);
}

BOOST_AUTO_TEST_CASE(post_decrement)
{
  abramov::List< int > list;
  list.pushBack(0);
  list.pushBack(1);
  auto iter = list.end();
  iter--;
  BOOST_TEST(*(iter--) == 1);
  BOOST_TEST(*iter == 0);
}

BOOST_AUTO_TEST_CASE(equality)
{
  abramov::List< int > list;
  list.pushBack(0);
  list.pushBack(1);
  list.pushBack(2);
  auto iter1 = list.begin();
  auto iter2 = ++iter1;
  bool b = (iter1 == iter2);
  BOOST_TEST(b);
}

BOOST_AUTO_TEST_CASE(inequality)
{
  abramov::List< int > list;
  list.pushBack(0);
  list.pushBack(1);
  auto iter1 = list.end();
  auto iter2 = list.begin();
  bool b = (iter1 != iter2);
  BOOST_TEST(b);
}

BOOST_AUTO_TEST_CASE(dereferencing)
{
  abramov::List< int > list;
  list.pushBack(0);
  auto iter = list.begin();
  BOOST_TEST(*iter == 0);
}

BOOST_AUTO_TEST_CASE(arrow_operator)
{
  struct point_t
  {
    double x;
    double y;
  };
  point_t p{ 1, 2 };
  abramov::List< point_t > list;
  list.pushBack(p);
  auto iter = list.begin();
  BOOST_TEST(iter->x == 1);
}

