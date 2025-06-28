#include <boost/test/unit_test.hpp>
#include "list.hpp"

BOOST_AUTO_TEST_CASE(fill_constructor)
{
  abramov::List< int > list(10, 1);
  auto it = list.begin();
  for (size_t i = 0; i < 10; ++i)
  {
    BOOST_TEST(*(it) == 1);
  }
}

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

BOOST_AUTO_TEST_CASE(operator_il_test)
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

BOOST_AUTO_TEST_CASE(cbegin)
{
  abramov::List< int > list{ 1, 2 };
  auto iter = list.cbegin();
  BOOST_TEST(*iter == 1);
}

BOOST_AUTO_TEST_CASE(cend)
{
  abramov::List< int > list{ 1, 2 };
  auto iter = list.cend();
  --iter;
  BOOST_TEST(*iter == 2);
}

BOOST_AUTO_TEST_CASE(find)
{
  abramov::List< int > list{ 0, 1, 2, 1};
  auto it = list.find(list.begin(), list.end(), 1);
  BOOST_TEST(*it == 1);
}

BOOST_AUTO_TEST_CASE(find_if)
{
  abramov::List< int > list{ 1, 2, 3, 4 };
  auto it = list.find_if(list.begin(), list.end(), [](int a) { return a % 2 == 0; });
  BOOST_TEST(*it == 2);
}

BOOST_AUTO_TEST_CASE(remove_test)
{
  abramov::List< int > list{ 0, 1, 1, 2 };
  auto it = list.remove(list.begin(), list.end(), 1);
  BOOST_TEST(*it == 1);
}

BOOST_AUTO_TEST_CASE(remove_if_test)
{
  abramov::List< int > list{ 1, 2, 3, 4, 5 };
  auto it = list.remove_if(list.begin(), list.end(), [](int a) { return a % 2 == 0; });
  BOOST_TEST(*it == 4);
}

BOOST_AUTO_TEST_CASE(splice_other_list)
{
  abramov::List< int > list_tmp{ 2, 3, 4 };
  abramov::List< int > list{ 1, 5 };
  list.splice(list.cbegin(), list_tmp);
  auto it = list.begin();
  for (size_t i = 2; i < 5; ++i)
  {
    BOOST_TEST(*(it++) == i);
  }
  BOOST_TEST(*(it++) == 1);
  BOOST_TEST(*it == 5);
  list.clear();
  list_tmp = { 6, 7 };
  list = { 4, 5 };
  list.splice(list.cend(), list_tmp);
  it = list.begin();
  for (size_t i = 4; i < 8; ++i)
  {
    BOOST_TEST(*(it++) == i);
  }
  list.clear();
  list = { 1, 5, 6 };
  list_tmp = { 2, 3, 4 };
  list.splice(++list.cbegin(), list_tmp);
  it = list.begin();
  for (size_t i = 1; i < 7; ++i)
  {
    BOOST_TEST(*(it++) == i);
  }
}

BOOST_AUTO_TEST_CASE(splice_iter)
{
  abramov::List< int > tmp_list{ 1, 0, -1, 3, 4, 5 };
  abramov::List< int > list{ 1, 2 };
  list.splice(list.cbegin(), tmp_list, ++tmp_list.cbegin());
  auto it = list.begin();
  for (size_t i = 0; i < 3; ++i)
  {
    BOOST_TEST(*(it++) == i);
  }
  BOOST_TEST(*(++tmp_list.cbegin()) == -1);
  list.splice(++list.cbegin(), tmp_list, ++tmp_list.cbegin());
  BOOST_TEST(*(++list.cbegin()) == -1);
  list.splice(list.cbegin(), tmp_list, tmp_list.cbegin());
  list.splice(++list.cbegin(), tmp_list, --tmp_list.cend());
  BOOST_TEST(*(list.begin()) == 1);
  BOOST_TEST(*(++list.begin()) == 5);
}

BOOST_AUTO_TEST_CASE(insert)
{
  abramov::List< int > list{ 1, 3 };
  list.insert(++list.cbegin(), 2);
  list.insert(list.cbegin(), 0);
  list.insert(list.cend(), 4);
  auto it = list.begin();
  for (size_t i = 0; i < 5; ++i)
  {
    BOOST_TEST(*(it++) == i);
  }
}

BOOST_AUTO_TEST_CASE(insert_range)
{
  abramov::List< int > list{ 1 };
  list.insert(++list.cbegin(), 5, 2);
  auto it = list.begin();
  BOOST_TEST(*(it++) == 1);
  for (size_t i = 0; i < 5; ++i)
  {
    BOOST_TEST(*(it++) == 2);
  }
}

BOOST_AUTO_TEST_CASE(erase)
{
  abramov::List< int > list{ 1, 3, 2, 3, 4 };
  list.erase(++list.begin());
  list.erase(list.begin());
  list.erase(--list.end());
  auto it = list.begin();
  for (size_t i = 2; i < 4; ++i)
  {
    BOOST_TEST(*(it++) == i);
  }
}

BOOST_AUTO_TEST_CASE(assign_fill)
{
  abramov::List< int > list;
  list.assign(5, 1);
  auto it = list.begin();
  for (size_t i = 0; i < 5; ++i)
  {
    BOOST_TEST(*(it++) == 1);
  }
}

BOOST_AUTO_TEST_CASE(assign_il)
{
  abramov::List< int > list;
  list.assign({ 1, 2, 3 });
  auto it = list.begin();
  for (size_t i = 1; i < 4; ++i)
  {
    BOOST_TEST(*(it++) == i);
  }
}

BOOST_AUTO_TEST_CASE(assign_iters)
{
  abramov::List< int > tmp_list{ 2, 3, 4 };
  abramov::List< int > list;
  list.assign(tmp_list.begin(), tmp_list.end());
  auto it = list.begin();
  for (size_t i = 2; i < 5; ++i)
  {
    BOOST_TEST(*(it++) == i);
  }
}
