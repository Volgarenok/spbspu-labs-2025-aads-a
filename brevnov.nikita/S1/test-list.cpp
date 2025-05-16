#include <sstream>
#include <boost/test/tools/interface.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include "iterator.hpp"
#include "list.hpp"

using namespace brevnov;

void createList(List< int >& list, int count)
{
  for (int i = 0; i < count; ++i)
  {
    list.push_back(i);
  }
}

std::ostream& outputList(std::ostream& out, List< int >& list)
{
  for (List< int >::ConstIter it = list.cbegin(); it != list.cend(); ++it)
  {
    out << *it;
  }
  return out;
}
BOOST_AUTO_TEST_SUITE(list)

BOOST_AUTO_TEST_CASE(default_and_copy_and_move_constructor)
{
  List< int > list;
  BOOST_TEST(list.empty());
  createList(list, 4);
  List< int > copyList(list);
  BOOST_TEST((copyList == list));
  List< int > list2(std::move(list));
  BOOST_TEST((list2 == copyList));
}

BOOST_AUTO_TEST_CASE(fill_constructor)
{
  size_t size = 0;
  List< int > list(size, 2);
  BOOST_TEST(list.empty());
  size = 10;
  List< int > list2(size, 5);
  BOOST_TEST((list2.size() == size));
}

BOOST_AUTO_TEST_CASE(range_constructor)
{
  List< int > list;
  List< int > list2(list.cbegin(), list.cend());
  BOOST_TEST(list2.empty());
  createList(list, 5);
  List< int > list3(list.cbegin(), list.cend());
  BOOST_TEST(list3.size() == 5);
}

BOOST_AUTO_TEST_CASE(initializer_list_constructor)
{
  std::initializer_list< int > il = {1, 2, 3, 4 };
  List< int > list(il);
  BOOST_TEST(list.size() == il.size());
}

BOOST_AUTO_TEST_CASE(copy_assignment_operator)
{
  List< int > list;
  createList(list, 5);
  List< int > copyList;
  copyList = list;
  BOOST_TEST((list == copyList));
  List< int > list2;
  list2 = std::move(list);
  BOOST_TEST((list2 == copyList));
}

BOOST_AUTO_TEST_CASE(initializer_list_assignment_operator)
{
  std::initializer_list< int > il = { 1, 2, 3 ,4 };
  List< int > listIl(il);
  List< int > list;
  list = il;
  BOOST_TEST((list == listIl));
}

BOOST_AUTO_TEST_CASE(empty)
{
  List< int > list;
  BOOST_TEST(list.size() == 0);
}

BOOST_AUTO_TEST_CASE(size)
{
  List< int > list;
  const int i = 1;
  list.push_back(i);
  BOOST_TEST(list.size() == 1);
}

BOOST_AUTO_TEST_CASE(front_and_push_front)
{
  List< int > list;
  int i = 1;
  list.push_front(i);
  list.push_front(++i);
  BOOST_TEST(list.front() == i);
}

BOOST_AUTO_TEST_CASE(back_and_push_back)
{
  List< int > list;
  int i = 1;
  list.push_back(i);
  list.push_back(++i);
  BOOST_TEST(list.back() == i);
}

BOOST_AUTO_TEST_CASE(pop_back)
{
  List< int > list;
  int i = 1;
  list.push_back(i);
  list.push_back(++i);
  list.pop_back();
  BOOST_TEST(list.back() == --i);
  list.pop_back();
  BOOST_TEST(list.size() == 0);
}

BOOST_AUTO_TEST_CASE(pop_front)
{
  List< int > list;
  int i = 1;
  list.push_front(i);
  list.push_back(++i);
  list.pop_front();
  BOOST_TEST(list.front() == i);
  list.pop_front();
  BOOST_TEST(list.size() == 0);
}

BOOST_AUTO_TEST_CASE(clear)
{
  List< int > list;
  createList(list, 4);
  list.clear();
  BOOST_TEST(list.empty());
}

BOOST_AUTO_TEST_CASE(swap)
{
  List< int > list;
  int i = 0;
  list.push_back(i);
  List< int > copyList(list);
  List< int > list2;
  list2.push_back(++i);
  list2.push_front(++i);
  list2.swap(list);
  BOOST_TEST(list2.size() == copyList.size());
}

BOOST_AUTO_TEST_SUITE_END()
