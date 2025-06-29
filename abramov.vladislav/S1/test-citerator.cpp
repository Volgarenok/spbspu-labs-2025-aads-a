#include <boost/test/unit_test.hpp>
#include "cIterator.hpp"

BOOST_AUTO_TEST_CASE(pre_increment_const)
{
  abramov::List< int > list;
  list.pushBack(0);
  list.pushBack(1);
  auto iter = list.cbegin();
  BOOST_TEST(*iter == 0);
  ++iter;
  BOOST_TEST(*iter == 1);
}

BOOST_AUTO_TEST_CASE(post_increment_const)
{
  abramov::List< int > list;
  list.pushBack(0);
  list.pushBack(1);
  auto iter = list.cbegin();
  BOOST_TEST(*(iter++) == 0);
  BOOST_TEST(*iter == 1);
}

BOOST_AUTO_TEST_CASE(pre_decrement_const)
{
  abramov::List< int > list;
  list.pushBack(0);
  list.pushBack(1);
  auto iter = list.cend();
  --iter;
  BOOST_TEST(*iter == 1);
  --iter;
  BOOST_TEST(*iter == 0);
}

BOOST_AUTO_TEST_CASE(post_decrement_const)
{
  abramov::List< int > list;
  list.pushBack(0);
  list.pushBack(1);
  auto iter = list.cend();
  iter--;
  BOOST_TEST(*(iter--) == 1);
  BOOST_TEST(*iter == 0);
}

BOOST_AUTO_TEST_CASE(equality_const)
{
  abramov::List< int > list;
  list.pushBack(0);
  list.pushBack(1);
  list.pushBack(2);
  auto iter1 = list.cbegin();
  auto iter2 = ++iter1;
  bool b = (iter1 == iter2);
  BOOST_TEST(b);
}

BOOST_AUTO_TEST_CASE(inequality_const)
{
  abramov::List< int > list;
  list.pushBack(0);
  list.pushBack(1);
  auto iter1 = list.cbegin();
  auto iter2 = list.cend();
  bool b = (iter1 != iter2);
  BOOST_TEST(b);
}

BOOST_AUTO_TEST_CASE(dereferencing_const)
{
  abramov::List< int > list;
  list.pushBack(0);
  auto iter = list.cbegin();
  BOOST_TEST(*iter == 0);
}

BOOST_AUTO_TEST_CASE(arrow_operator_const)
{
  struct point_t
  {
    double x;
    double y;
  };
  abramov::List< point_t > list;
  point_t p{ 1, 2 };
  list.pushBack(p);
  auto iter = list.begin();
  BOOST_TEST(iter->y == 2);
}
