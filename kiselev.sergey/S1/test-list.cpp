#include <sstream>
#include <boost/test/tools/interface.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include "iterator.hpp"
#include "list.hpp"

using namespace kiselev;
namespace
{
  void createList(List< int >& list, int count)
  {
    for (int i = 0; i < count; ++i)
    {
      list.pushBack(i);
    }
  }
  std::ostream& outputList(std::ostream& out, List< int >& list)
  {
    for (List< int >::ConstIterator it = list.cbegin(); it != list.cend(); ++it)
    {
      out << *it;
    }
    return out;
  }
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
  list.pushBack(i);
  BOOST_TEST(list.size() == 1);
}

BOOST_AUTO_TEST_CASE(front_and_push_front)
{
  List< int > list;
  int i = 1;
  list.pushFront(i);
  list.pushFront(++i);
  BOOST_TEST(list.front() == i);
}

BOOST_AUTO_TEST_CASE(back_and_push_back)
{
  List< int > list;
  int i = 1;
  list.pushBack(i);
  list.pushBack(++i);
  BOOST_TEST(list.back() == i);
}

BOOST_AUTO_TEST_CASE(pop_back)
{
  List< int > list;
  int i = 1;
  list.pushBack(i);
  list.pushBack(++i);
  list.popBack();
  BOOST_TEST(list.back() == --i);
  list.popBack();
  BOOST_TEST(list.size() == 0);
}

BOOST_AUTO_TEST_CASE(pop_front)
{
  List< int > list;
  int i = 1;
  list.pushFront(i);
  list.pushBack(++i);
  list.popFront();
  BOOST_TEST(list.front() == i);
  list.popFront();
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
  list.pushBack(i);
  List< int > copyList(list);
  List< int > list2;
  list2.pushBack(++i);
  list2.pushFront(++i);
  list2.swap(list);
  BOOST_TEST(list2.size() == copyList.size());
}

BOOST_AUTO_TEST_CASE(erase_pos)
{
  List< int > list;
  createList(list, 20);
  List< int >::ConstIterator begin = list.cbegin();
  List< int >::Iterator it = list.erase(begin);
  BOOST_CHECK(it == list.begin());
  List< int>::ConstIterator end = --list.cend();
  it = list.erase(end);
  BOOST_CHECK(it == list.end());
  List< int >::ConstIterator iter = ++list.cbegin();
  it = list.erase(iter);
  BOOST_CHECK(*it == 3);
  begin = ++list.cbegin();
  end = --list.cend();
  it = list.erase(begin, end);
  BOOST_CHECK(it == --list.end());
}

BOOST_AUTO_TEST_CASE(remove)
{
  List< int > list;
  int i = 0;
  list.pushBack(i);
  list.remove(i);
  BOOST_TEST(list.empty());
  createList(list, 10);
  list.pushBack(1);
  list.remove(1);
  BOOST_TEST(list.size() == 9);
}

BOOST_AUTO_TEST_CASE(splice_list)
{
  List< int > list1;
  createList(list1, 5);
  List< int > list2;
  createList(list2, 3);
  List< int >::ConstIterator it = ++list1.cbegin();
  list1.splice(it, list2);
  std::ostringstream out;
  outputList(out, list1);
  BOOST_TEST(out.str() == "00121234");
  List< int > list3;
  createList(list3, 5);
  List< int > list4;
  createList(list4, 3);
  it = ++list3.cbegin();
  list3.splice(it, std::move(list4));
  std::ostringstream out2;
  outputList(out2, list3);
  BOOST_TEST(out2.str() == "00121234");

}
BOOST_AUTO_TEST_CASE(splice_element)
{
  List< int > list;
  createList(list, 4);
  List< int > list2;
  createList(list2, 3);
  std::ostringstream out1;
  List< int >::ConstIterator it = list.cbegin();
  List< int >::ConstIterator element = ++list2.cbegin();
  list.splice(it, list2, element);
  std::ostringstream out;
  outputList(out, list);
  BOOST_TEST(out.str() == "10123");
  it = list.cbegin();
  element = list2.cbegin();
  list.splice(it, std::move(list2), element);
  std::ostringstream out2;
  outputList(out2, list);\
  BOOST_TEST(out2.str() == "010123");
}

BOOST_AUTO_TEST_CASE(splice_range)
{
  List< int > list;
  createList(list, 4);
  List< int > list2;
  createList(list2, 3);
  List< int >::ConstIterator pos = list.cend();
  List< int >::ConstIterator first = ++list2.cbegin();
  List< int >::ConstIterator last = list2.cend();
  list.splice(pos, list2, first, last);
  std::ostringstream out;
  outputList(out, list);
  BOOST_TEST(out.str() == "012312");
  List< int > list3;
  createList(list3, 3);
  pos = list.cbegin();
  first = list3.cbegin();
  last = --list3.cend();
  list.splice(pos, std::move(list3), first, last);
  std::ostringstream out2;
  outputList(out2, list);
  BOOST_TEST(out2.str() == "01012312");
}

BOOST_AUTO_TEST_CASE(insert_element)
{
  List< int > list;
  list.insert(list.cbegin(), 1);
  BOOST_TEST(list.front() == 1);
  list.insert(list.cbegin(), 3);
  BOOST_TEST(list.front() == 3);
  list.insert(++list.cbegin(), 5);
  BOOST_TEST(*(++list.cbegin()) == 5);
  int i = 7;
  list.insert(list.cbegin(), i);
  BOOST_TEST(list.front() == 7);
}

BOOST_AUTO_TEST_CASE(insert_fill)
{
  List< int > list;
  createList(list, 5);
  size_t i = 0;
  List< int >::Iterator res = list.insert(list.cbegin(), i, 3);
  BOOST_TEST((res == list.begin()));
  i = 2;
  res = list.insert(list.cbegin(), i, 7);
  BOOST_TEST(list.front() == 7);
}

BOOST_AUTO_TEST_CASE(insert_range)
{
  List< int > list;
  createList(list, 5);
  List< int > list2;
  List< int >::Iterator res = list.insert(list.cbegin(), list2.cbegin(), list2.cend());
  BOOST_TEST((res == list.begin()));
  createList(list2, 4);
  res = list.insert(list.cbegin(), ++list2.cbegin(), list2.cend());
  BOOST_TEST((res == list.begin()));
}

BOOST_AUTO_TEST_CASE(insert_initializer)
{
  List< int > list;
  createList(list, 5);
  std::initializer_list< int > il = { 1, 2, 3 };
  List< int >::Iterator it = list.insert(list.cbegin(), il.begin(), il.end());
  BOOST_TEST((it == list.begin()));
}

BOOST_AUTO_TEST_CASE(assign_fill)
{
  List< int > list;
  size_t size = 20;
  list.assign(size, 10);
  BOOST_TEST(list.size() == size);
  BOOST_TEST(list.front() == 10);
}

BOOST_AUTO_TEST_CASE(assign_range_initializer)
{
  List< int > list;
  List< int > list2;
  createList(list2, 5);
  list.assign(list2.begin(), list2.end());
  BOOST_TEST(list.size() == 5);
  std::initializer_list< int > il = { 1, 2, 3 };
  list.assign(il);
  BOOST_TEST(list.size() == 3);
}

BOOST_AUTO_TEST_CASE(operator_equal)
{
  List< int > list;
  createList(list, 5);
  List< int > list2;
  createList(list2, 5);
  bool isEqual = list == list2;
  BOOST_TEST(isEqual);
  list2.pushBack(1);
  isEqual = list == list2;
  BOOST_TEST(!isEqual);
  list.pushBack(4);
  isEqual = list == list2;
  BOOST_TEST(!isEqual);
}

BOOST_AUTO_TEST_CASE(operator_unequal)
{
  List< int > list;
  createList(list, 3);
  List< int > list2;
  bool isUnequal = list != list2;
  BOOST_TEST(isUnequal);
  createList(list2, 3);
  isUnequal = list != list2;
  BOOST_TEST(!isUnequal);
}

BOOST_AUTO_TEST_CASE(operator_less)
{
  List< int > list = { 0, 1, 2, 3 };
  List< int > list2 = { 0, 1, 2, 3 };
  bool isLess = list < list2;
  BOOST_TEST(!isLess);
  list.pushBack(1);
  list2.pushBack(3);
  isLess = list < list2;
  BOOST_TEST(isLess);
}

BOOST_AUTO_TEST_CASE(operator_more)
{
  List< int > list = { 1, 2, 3 };
  List< int > list2 = { 1, 2, 3 };
  bool isMore = list > list2;
  BOOST_TEST(!isMore);
  list.pushBack(6);
  list2.pushBack(4);
  isMore = list > list2;
  BOOST_TEST(isMore);
}

BOOST_AUTO_TEST_CASE(operator_less_or_equal)
{
  List< int > list = { 1, 2, 3 };
  List< int > list2 = { 1, 2, 3 };
  bool isLessOrEqual = list <= list2;
  BOOST_TEST(isLessOrEqual);
  list.pushBack(4);
  list2.pushBack(6);
  isLessOrEqual = list <= list2;
  BOOST_TEST(isLessOrEqual);
  list.popBack();
  list2.popBack();
  list.pushBack(7);
  list2.pushBack(2);
  isLessOrEqual = list <= list2;
  BOOST_TEST(!isLessOrEqual);
}

BOOST_AUTO_TEST_CASE(operator_more_or_equal)
{
  List< int > list = { 3, 4, 5 };
  List< int > list2 = { 3, 4, 5 };
  bool isMoreOrEqual = list >= list2;
  BOOST_TEST(isMoreOrEqual);
  list.pushFront(5);
  list2.pushFront(3);
  isMoreOrEqual = list >= list2;
  BOOST_TEST(isMoreOrEqual);
  list.popFront();
  list2.popFront();
  list.pushFront(3);
  list2.pushFront(6);
  isMoreOrEqual = list >= list2;
  BOOST_TEST(!isMoreOrEqual);
}

BOOST_AUTO_TEST_CASE(reverse)
{
  List< int > list;
  list.reverse();
  BOOST_TEST(list.empty());
  list.pushBack(1);
  createList(list, 4);
  list.reverse();
  std::ostringstream out;
  outputList(out, list);
  BOOST_TEST(out.str() == "32101");
}


BOOST_AUTO_TEST_SUITE_END()
