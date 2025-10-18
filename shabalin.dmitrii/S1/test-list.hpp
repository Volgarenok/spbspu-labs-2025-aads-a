#ifndef TEST_LIST_HPP
#define TEST_LIST_HPP

#include <boost/test/unit_test.hpp>
#include "forward_list.hpp"

using namespace shabalin;

BOOST_AUTO_TEST_CASE(push_back_test)
{
  List< size_t > list;
  list.push_back(10);
  BOOST_TEST(list.front() == 10);
  list.push_back(100);
  BOOST_TEST(list.front() == 10);
  auto it = list.begin();
  ++it;
  BOOST_TEST(*it == 100);
}

BOOST_AUTO_TEST_CASE(copy_test)
{
  List< size_t > list1;
  list1.push_back(10);
  List< size_t > list2;
  list2.push_back(20);
  list2 = list1;
  BOOST_TEST(list2.front() == 10);
}

BOOST_AUTO_TEST_CASE(move_test)
{
  List< size_t > list1;
  list1.push_back(10);
  List< size_t > list2;
  list2 = std::move(list1);
  BOOST_TEST(list2.front() == 10);
  BOOST_TEST(list1.empty());
}

BOOST_AUTO_TEST_CASE(clear_test)
{
  List< size_t > list;
  list.push_back(10);
  list.push_back(20);
  BOOST_TEST(!list.empty());
  list.clear();
  BOOST_TEST(list.empty());
}

BOOST_AUTO_TEST_CASE(pushFront_test)
{
  List< size_t > list;
  list.push_front(10);
  BOOST_TEST(list.front() == 10);
  list.push_front(100);
  BOOST_TEST(list.front() == 100);
  auto it = list.begin();
  ++it;
  BOOST_TEST(*it == 10);
}

BOOST_AUTO_TEST_CASE(empty_test)
{
  List< size_t > list;
  BOOST_TEST(list.empty());
  list.push_back(10);
  list.push_back(20);
  BOOST_TEST(!list.empty());
}

BOOST_AUTO_TEST_CASE(popFront_test)
{
  List< size_t > list;
  list.push_back(10);
  list.push_back(20);
  list.pop_front();
  BOOST_TEST(list.front() == 20);
}

BOOST_AUTO_TEST_CASE(iterator_test)
{
  List< size_t > list;
  list.push_back(10);
  list.push_back(20);
  list.push_back(30);
  auto i = list.begin();
  BOOST_TEST(*i == 10);
  ++i;
  BOOST_TEST(*i == 20);
}

BOOST_AUTO_TEST_CASE(constIterator_test)
{
  List< size_t > list;
  list.push_back(10);
  list.push_back(20);
  list.push_back(30);
  auto i = list.cbegin();
  BOOST_TEST(*i == 10);
  ++i;
  BOOST_TEST(*i == 20);
}

BOOST_AUTO_TEST_CASE(reverse_test)
{
  List< size_t > list;
  list.push_back(10);
  list.push_back(20);
  list.push_back(30);
  list.reverse();
  auto it = list.begin();
  BOOST_TEST(*it == 30);
  ++it;
  BOOST_TEST(*it == 20);
  ++it;
  BOOST_TEST(*it == 10);
}

BOOST_AUTO_TEST_CASE(remove_test)
{
  List< size_t > list;
  list.push_back(10);
  list.push_back(20);
  list.push_back(10);
  list.push_back(30);
  list.remove(10);
  auto it = list.begin();
  BOOST_TEST(*it == 20);
  ++it;
  BOOST_TEST(*it == 30);
  BOOST_TEST(++it == list.end());
}

BOOST_AUTO_TEST_CASE(remove_if_test)
{
  List< size_t > list;
  list.push_back(10);
  list.push_back(25);
  list.push_back(5);
  list.push_back(30);
  list.remove_if([](size_t val) { return val < 20; });
  auto it = list.begin();
  BOOST_TEST(*it == 25);
  ++it;
  BOOST_TEST(*it == 30);
  BOOST_TEST(++it == list.end());
}

BOOST_AUTO_TEST_CASE(assign_test)
{
  List< size_t > list;
  list.push_back(1);
  list.push_back(2);
  list.assign(3, 100);
  BOOST_TEST(list.front() == 100);
  auto it = list.begin();
  BOOST_TEST(*it == 100);
  ++it;
  BOOST_TEST(*it == 100);
  ++it;
  BOOST_TEST(*it == 100);
  BOOST_TEST(++it == list.end());
}

BOOST_AUTO_TEST_CASE(initializer_list_test)
{
  List< size_t > list = {10, 20, 30};
  auto it = list.begin();
  BOOST_TEST(*it == 10);
  ++it;
  BOOST_TEST(*it == 20);
  ++it;
  BOOST_TEST(*it == 30);
}

BOOST_AUTO_TEST_CASE(swap_test)
{
  List< size_t > list1;
  list1.push_back(10);
  list1.push_back(20);
  List< size_t > list2;
  list2.push_back(30);
  list2.push_back(40);
  list2.push_back(50);
  list1.swap(list2);
  BOOST_TEST(list1.front() == 30);
  BOOST_TEST(list2.front() == 10);
  size_t count1 = 0;
  for (auto it = list1.begin(); it != list1.end(); ++it) count1++;
  BOOST_TEST(count1 == 3);
  size_t count2 = 0;
  for (auto it = list2.begin(); it != list2.end(); ++it) count2++;
  BOOST_TEST(count2 == 2);
}

#endif
