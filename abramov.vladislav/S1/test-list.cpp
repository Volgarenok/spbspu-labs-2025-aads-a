#include <boost/test/unit_test.hpp>
#include "list.hpp"

BOOST_AUTO_TEST_CASE(init_list)
{
  abramov::List< int > list{ 1, 2, 3 };
  BOOST_TEST(list.front() == 1);
  BOOST_TEST(list.back() == 3);
}

BOOST_AUTO_TEST_CASE(range_constructor)
{
  abramov::List< int > list1{ 1, 2, 3, 4 };
  abramov::List< int > list2{ list1.begin(), list1.end() };
  auto it = list2.begin();
  for (size_t i = 1; i < 5; ++i)
  {
    BOOST_TEST(*(it++) == i);
  }
}

BOOST_AUTO_TEST_CASE(operator_test)
{
  abramov::List< int > list1 = { 1, 2, 3 };
  auto it = list1.begin();
  for (size_t i = 1; i < 4; ++i)
  {
    BOOST_TEST(*(it++) == i);
  }
}

BOOST_AUTO_TEST_CASE(empty)
{
  abramov::List< int > list;
  BOOST_TEST(list.empty());
}

BOOST_AUTO_TEST_CASE(size)
{
  abramov::List< int > list;
  BOOST_TEST(list.size() == 0);
}

BOOST_AUTO_TEST_CASE(front_pushFront)
{
  abramov::List< int > list;
  list.pushFront(0);
  BOOST_TEST(list.front() == 0);
  list.pushFront(1);
  BOOST_TEST(list.front() == 1);
}

BOOST_AUTO_TEST_CASE(back_pushBack)
{
  abramov::List< int > list;
  list.pushBack(0);
  BOOST_TEST(list.back() == 0);
  list.pushBack(1);
  BOOST_TEST(list.back() == 1);
}

BOOST_AUTO_TEST_CASE(popFront)
{
  abramov::List< int > list;
  list.pushFront(0);
  list.popFront();
  BOOST_TEST(list.empty());
}

BOOST_AUTO_TEST_CASE(popBack)
{
  abramov::List< int > list;
  list.pushBack(0);
  list.popBack();
  BOOST_TEST(list.empty());
}

BOOST_AUTO_TEST_CASE(swap)
{
  abramov::List< int > list1{ 1, 2 };
  abramov::List< int > list2{ 3, 4 };
  list1.swap(list2);
  BOOST_TEST(list1.front() == 3);
  BOOST_TEST(list1.back() == 4);
  BOOST_TEST(list2.front() == 1);
  BOOST_TEST(list2.back() == 2);
}

BOOST_AUTO_TEST_CASE(clear)
{
  abramov::List< int > list{ 1, 2 };
  list.clear();
  BOOST_TEST(list.empty());
}

BOOST_AUTO_TEST_CASE(begin)
{
  abramov::List< int > list{ 1, 0 };
  list.pushFront(0);
  list.pushFront(1);
  auto iter = list.begin();
  BOOST_TEST(*iter == 1);
}

BOOST_AUTO_TEST_CASE(end)
{
  abramov::List< int > list{ 1, 2 };
  auto iter1 = list.begin();
  auto iter2 = list.end();
  ++iter1;
  ++iter1;
  bool b = (iter1 == iter2);
  BOOST_TEST(b);
}

BOOST_AUTO_TEST_CASE(fill)
{
  abramov::List< int > list{ 0, 1, 2 };
  auto iter1 = list.begin();
  auto iter2 = list.end();
  list.fill(iter1, iter2, 3);
  BOOST_TEST(list.front() == 3);
  BOOST_TEST(*(++iter1) == 3);
  BOOST_TEST(list.back() == 3);
}

BOOST_AUTO_TEST_CASE(find)
{
  abramov::List< int > list{ 0, 1, 2, 1};
  auto it = list.find(list.begin(), list.end(), 1);
  BOOST_TEST(*it == 1);
}

BOOST_AUTO_TEST_CASE(remove_test)
{
  abramov::List< int > list{ 0, 1, 1, 2 };
  auto it = list.remove(list.begin(), list.end(), 1);
  BOOST_TEST(*it == 1);
}

BOOST_AUTO_TEST_CASE(splice)
{
  abramov::List< int > list1{ 3, 4, 5 };
  abramov::List< int > list2{ 1, 2 };
  auto it = list1.cbegin();
  list1.splice(it, list2);
  it = list1.cbegin();
  for (size_t i = 1; i < 6; ++i)
  {
    BOOST_TEST(*(it++) == i);
  }
  abramov::List< int > list3{ 6, 7 };
  list1.splice(++list1.cbegin(), list3);
  it = ++list1.cbegin();
  for (size_t i = 6; i < 8; ++i)
  {
    BOOST_TEST(*(it++) == i);
  }
}
