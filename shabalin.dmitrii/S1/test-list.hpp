#ifndef TEST_LIST_HPP
#define TEST_LIST_HPP

#include <boost/test/unit_test.hpp>
#include "forward_list.hpp"

using namespace shabalin;

BOOST_AUTO_TEST_CASE(push_front_test)
{
  List<size_t> list;
  list.push_front(10);
  BOOST_TEST(list.front() == 10);
  list.push_front(20);
  BOOST_TEST(list.front() == 20);

  auto it = list.begin();
  BOOST_TEST(*it == 20);
  ++it;
  BOOST_TEST(*it == 10);
}

BOOST_AUTO_TEST_CASE(copy_constructor_test)
{
  List<size_t> list1;
  list1.push_front(10);
  list1.push_front(20);

  List<size_t> list2(list1);
  BOOST_TEST(list2.front() == 20);

  auto it1 = list1.begin();
  auto it2 = list2.begin();
  while (it1 != list1.end() && it2 != list2.end())
  {
    BOOST_TEST(*it1 == *it2);
    ++it1;
    ++it2;
  }
}

BOOST_AUTO_TEST_CASE(copy_assignment_test)
{
  List<size_t> list1;
  list1.push_front(10);
  List<size_t> list2;
  list2.push_front(20);

  list2 = list1;
  BOOST_TEST(list2.front() == 10);
}

BOOST_AUTO_TEST_CASE(move_constructor_test)
{
  List<size_t> list1;
  list1.push_front(10);
  list1.push_front(5);

  List<size_t> list2(std::move(list1));
  BOOST_TEST(list2.front() == 5);
  BOOST_TEST(list1.empty());
}

BOOST_AUTO_TEST_CASE(move_assignment_test)
{
  List<size_t> list1;
  list1.push_front(10);
  List<size_t> list2;
  list2 = std::move(list1);

  BOOST_TEST(list2.front() == 10);
  BOOST_TEST(list1.empty());
}

BOOST_AUTO_TEST_CASE(clear_test)
{
  List<size_t> list;
  list.push_front(10);
  list.push_front(20);
  BOOST_TEST(!list.empty());
  list.clear();
  BOOST_TEST(list.empty());
}

BOOST_AUTO_TEST_CASE(iterator_traversal_test)
{
  List<size_t> list;
  list.push_front(3);
  list.push_front(2);
  list.push_front(1);

  auto it = list.begin();
  BOOST_TEST(*it == 1);
  ++it;
  BOOST_TEST(*it == 2);
  ++it;
  BOOST_TEST(*it == 3);
  ++it;
  BOOST_TEST(it == list.end());
}

BOOST_AUTO_TEST_CASE(initializer_list_test)
{
  List<size_t> list = {1, 2, 3};
  auto it = list.begin();
  BOOST_TEST(*it == 1);
  ++it;
  BOOST_TEST(*it == 2);
  ++it;
  BOOST_TEST(*it == 3);
}

BOOST_AUTO_TEST_CASE(empty_test)
{
  List<int> list;
  BOOST_TEST(list.empty());
  list.push_front(1);
  BOOST_TEST(!list.empty());
  list.clear();
  BOOST_TEST(list.empty());
}

#endif
