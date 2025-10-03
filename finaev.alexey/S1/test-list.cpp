#include <boost/test/unit_test.hpp>
#include "list.hpp"

using namespace finaev;

BOOST_AUTO_TEST_CASE(push_back_test)
{
  List< size_t > list;
  list.pushBack(10);
  BOOST_TEST(list.front() == 10);
  BOOST_TEST(list.back() == 10);
  list.pushBack(100);
  BOOST_TEST(list.front() == 10);
  BOOST_TEST(list.back() == 100);
}

BOOST_AUTO_TEST_CASE(copy_test)
{
  List< size_t > list1;
  list1.pushBack(10);
  List< size_t > list2;
  list2.pushBack(20);
  list2 = list1;
  BOOST_TEST(list2.front() == 10);
}

BOOST_AUTO_TEST_CASE(move_test)
{
  List< size_t > list1;
  list1.pushBack(10);
  List< size_t > list2;
  list2 = std::move(list1);
  BOOST_TEST(list2.front() == 10);
  BOOST_TEST(list1.isEmpty());
}

BOOST_AUTO_TEST_CASE(clear_test)
{
  List< size_t > list;
  list.pushBack(10);
  list.pushBack(20);
  BOOST_TEST(!list.isEmpty());
  list.clear();
  BOOST_TEST(list.isEmpty());
}

BOOST_AUTO_TEST_CASE(pushFront_test)
{
  List< size_t > list;
  list.pushFront(10);
  BOOST_TEST(list.front() == 10);
  BOOST_TEST(list.back() == 10);
  list.pushFront(100);
  BOOST_TEST(list.front() == 100);
  BOOST_TEST(list.back() == 10);
}

BOOST_AUTO_TEST_CASE(size_test)
{
  List< size_t > list;
  BOOST_TEST(list.isEmpty());
  list.pushBack(10);
  list.pushBack(20);
  BOOST_TEST(list.size() == 2);
}

BOOST_AUTO_TEST_CASE(popBack_test)
{
  List< size_t > list;
  list.pushBack(10);
  list.pushBack(20);
  list.popBack();
  BOOST_TEST(list.back() == 10);
}

BOOST_AUTO_TEST_CASE(popFront_test)
{
  List< size_t > list;
  list.pushBack(10);
  list.pushBack(20);
  list.popFront();
  BOOST_TEST(list.back() == 20);
}

BOOST_AUTO_TEST_CASE(iterator_test)
{
  List< size_t > list;
  list.pushBack(10);
  list.pushBack(20);
  list.pushBack(30);
  auto i = list.begin();
  BOOST_TEST(*i == 10);
  ++i;
  BOOST_TEST(*i == 20);
  --i;
  BOOST_TEST(*i == 10);
}

BOOST_AUTO_TEST_CASE(constIterator_test)
{
  List< size_t > list;
  list.pushBack(10);
  list.pushBack(20);
  list.pushBack(30);
  auto i = list.constBegin();
  BOOST_TEST(*i == 10);
  ++i;
  BOOST_TEST(*i == 20);
  --i;
  BOOST_TEST(*i == 10);
}
