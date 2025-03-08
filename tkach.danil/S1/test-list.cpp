#include <boost/test/unit_test.hpp>
#include <sstream>
#include "list.hpp"

using namespace tkach;

namespace
{
  template< typename T >
  void outputList(std::ostream& out, const List< T >& list)
  {
    auto it = list.cbegin();
    out << *(it++);
    for (; it != list.cend(); it++)
    {
      out << " " << *it;
    }
  }
}

BOOST_AUTO_TEST_CASE(basic_contructor_test)
{
  List< int > list;
  BOOST_TEST(list.empty());
  BOOST_TEST(list.size() == 0);
}

BOOST_AUTO_TEST_CASE(list_move_push_front_test)
{
  List< int > list;
  list.pushFront(1);
  BOOST_TEST(!list.empty());
  BOOST_TEST(list.size() == 1);
  BOOST_TEST(list.front() == 1);
  list.pushFront(2);
  BOOST_TEST(list.front() == 2);
  BOOST_TEST(list.size() == 2);
  std::ostringstream out;
  outputList(out, list);
  BOOST_TEST(out.str() == "2 1");
}

BOOST_AUTO_TEST_CASE(list_move_push_back_test)
{
  List< int > list;
  list.pushBack(1);
  BOOST_TEST(list.size() == 1);
  BOOST_TEST(list.front() == 1);
  list.pushBack(2);
  BOOST_TEST(list.front() == 1);
  BOOST_TEST(list.size() == 2);
  std::ostringstream out;
  outputList(out, list);
  BOOST_TEST(out.str() == "1 2");
}

BOOST_AUTO_TEST_CASE(list_push_front_test)
{
  List< int > list;
  int a = 1;
  list.pushFront(a);
  BOOST_TEST(!list.empty());
  BOOST_TEST(list.size() == 1);
  BOOST_TEST(list.front() == 1);
  int b = 2;
  list.pushFront(b);
  std::ostringstream out;
  outputList(out, list);
  BOOST_TEST(out.str() == "2 1");
  BOOST_TEST(list.size() == 2);
}

BOOST_AUTO_TEST_CASE(list_push_back_test)
{
  List< int > list;
  int a = 1;
  list.pushBack(a);
  auto it = list.begin();
  BOOST_TEST(!list.empty());
  BOOST_TEST(list.size() == 1);
  BOOST_TEST(list.front() == 1);
  a = 2;
  list.pushBack(a);
  std::ostringstream out;
  outputList(out, list);
  BOOST_TEST(out.str() == "1 2");
}

BOOST_AUTO_TEST_CASE(list_pop_front_test)
{
  List< int > list;
  list.pushBack(1);
  list.pushBack(2);
  list.popFront();
  BOOST_TEST(list.size() == 1);
  BOOST_TEST(list.front() = 2);
  std::ostringstream out;
  outputList(out, list);
  BOOST_TEST(out.str() == "2");
}

BOOST_AUTO_TEST_CASE(list_clear_test)
{
  List< int > lst;
  lst.pushBack(1);
  lst.pushBack(2);
  lst.clear();
  BOOST_TEST(lst.empty());
  BOOST_TEST(lst.size() == 0);
}

BOOST_AUTO_TEST_CASE(list_move_constructor_test)
{
  List< int > list1;
  list1.pushBack(1);
  list1.pushBack(2);
  List< int > list2(std::move(list1));
  BOOST_TEST(list2.size() == 2);
  BOOST_TEST(list1.size() == 0);
  std::ostringstream out;
  outputList(out, list2);
  BOOST_TEST(out.str() == "1 2");
}

BOOST_AUTO_TEST_CASE(list_copy_constructor_test)
{
  List< int > list1;
  list1.pushBack(1);
  list1.pushBack(2);
  list1.pushBack(3);
  List< int > list2(list1);
  auto it = list1.begin();
  std::ostringstream out;
  outputList(out, list2);
  BOOST_TEST(out.str() == "1 2 3");
}

BOOST_AUTO_TEST_CASE(list_swap_test)
{
  List< int > list1;
  list1.pushBack(1);
  list1.pushBack(2);
  List< int > list2;
  list2.swap(list1);
  BOOST_TEST(list1.empty());
  std::ostringstream out;
  outputList(out, list2);
  BOOST_TEST(out.str() == "1 2");
}

BOOST_AUTO_TEST_CASE(list_fill_test)
{
  size_t count = 5;
  List< int > list(count, 54);
  BOOST_TEST(list.size() == 5);
  auto it = list.begin();
  for (size_t i = 0; i < count; ++i)
  {
    BOOST_TEST(*it == 54);
  }
}

BOOST_AUTO_TEST_CASE(list_erase_after_test)
{
  List< size_t > list;
  for (size_t i = 1; i <= 2; ++i)
  {
    list.pushBack(i);
  }
  auto it = list.cbegin();
  it++;
  auto it3 = list.erase_after(it);
  BOOST_TEST(*(it3) == 2);
  std::ostringstream out;
  outputList(out, list);
  BOOST_TEST(out.str() == "2");
}

BOOST_AUTO_TEST_CASE(list_erase_after_first_last_test)
{
  List< size_t > list;
  for (size_t i = 1; i <= 5; ++i)
  {
    list.pushBack(i);
  }
  auto it = list.erase_after(list.cbegin(), std::next(list.cbegin(), 3));
  BOOST_TEST(*it == 4);
  std::ostringstream out;
  outputList(out, list);
  BOOST_TEST(out.str() == "1 4 5");
}

BOOST_AUTO_TEST_CASE(list_remove_test)
{
  List< size_t > list;
  for (size_t i = 1; i <= 3; ++i)
  {
    list.pushBack(5);
    list.pushBack(5);
    list.pushBack(i);
  }
  list.remove(5);
  BOOST_TEST(list.size() == 3);
  std::ostringstream out;
  outputList(out, list);
  BOOST_TEST(out.str() == "1 2 3");
}

BOOST_AUTO_TEST_CASE(list_remove_if_test)
{
  List< size_t > list;
  for (size_t i = 1; i <= 3; ++i)
  {
    list.pushBack(11);
    list.pushBack(5);
  }
  list.pushBack(12);
  list.removeIf([](size_t n){return n < 10;});
  BOOST_TEST(list.size() == 4);
  std::ostringstream out;
  outputList(out, list);
  BOOST_TEST(out.str() == "11 11 11 12");
}

BOOST_AUTO_TEST_CASE(list_splice_after_range_test)
{
  List< size_t > list;
  for (size_t i = 1; i <= 3; ++i)
  {
    list.pushBack(i);
  }
  List< size_t > list2;
  for (size_t i = 4; i <= 7; ++i)
  {
    list2.pushBack(i);
  }
  list.spliceAfter(list.cbegin(), list2, list2.cbegin(), list2.cend());
  BOOST_TEST(list.size() == 6);
  std::ostringstream out;
  outputList(out, list);
  BOOST_TEST(out.str() == "1 5 6 7 2 3");
  BOOST_TEST(list2.size() == 1);
  BOOST_TEST(*(list2.begin()) == 4);

  List< size_t > list3;
  for (size_t i = 10; i <= 15; ++i)
  {
    list3.pushBack(i);
  }
  List< size_t > list4;
  for (size_t i = 20; i <= 25; ++i)
  {
    list4.pushBack(i);
  }
  list3.spliceAfter(std::next(list3.cbegin()), list4, list4.cbegin(), std::next(list4.cbegin(), 3));
  BOOST_TEST(list3.size() == 8);
  BOOST_TEST(list4.size() == 4);
  std::ostringstream out1;
  outputList(out1, list3);
  BOOST_TEST(out1.str() == "10 11 21 22 12 13 14 15");
  std::ostringstream out4;
  outputList(out4, list4);
  BOOST_TEST(out4.str() == "20 23 24 25");
  list3.spliceAfter(std::next(list3.cbegin()), list4, std::next(list4.cbegin()), std::next(list4.cbegin(), 4));
  BOOST_TEST(list3.size() == 10);
  BOOST_TEST(list4.size() == 2);
  std::ostringstream out5;
  outputList(out5, list4);
  BOOST_TEST(out5.str() == "20 23");
  std::ostringstream out3;
  outputList(out3, list3);
  BOOST_TEST(out3.str() == "10 11 24 25 21 22 12 13 14 15");
}

BOOST_AUTO_TEST_CASE(list_splice_after_range_move_test)
{
  List< size_t > list;
  for (size_t i = 1; i <= 3; ++i)
  {
    list.pushBack(i);
  }
  List< size_t > list2;
  for (size_t i = 4; i <= 7; ++i)
  {
    list2.pushBack(i);
  }
  list.spliceAfter(list.cbegin(), std::move(list2), list2.cbegin(), list2.cend());
  BOOST_TEST(list.size() == 6);
  std::ostringstream out;
  outputList(out, list);
  BOOST_TEST(out.str() == "1 5 6 7 2 3");
  BOOST_TEST(list2.size() == 1);
  BOOST_TEST(*(list2.begin()) == 4);

  List< size_t > list3;
  for (size_t i = 10; i <= 15; ++i)
  {
    list3.pushBack(i);
  }
  List< size_t > list4;
  for (size_t i = 20; i <= 25; ++i)
  {
    list4.pushBack(i);
  }
  list3.spliceAfter(std::next(list3.cbegin()), list4, list4.cbegin(), std::next(list4.cbegin(), 3));
  BOOST_TEST(list3.size() == 8);
  BOOST_TEST(list4.size() == 4);
  std::ostringstream out1;
  outputList(out1, list3);
  BOOST_TEST(out1.str() == "10 11 21 22 12 13 14 15");
  std::ostringstream out4;
  outputList(out4, list4);
  BOOST_TEST(out4.str() == "20 23 24 25");
  list3.spliceAfter(std::next(list3.cbegin()), std::move(list4), std::next(list4.cbegin()), std::next(list4.cbegin(), 4));
  BOOST_TEST(list3.size() == 10);
  BOOST_TEST(list4.size() == 2);
  std::ostringstream out5;
  outputList(out5, list4);
  BOOST_TEST(out5.str() == "20 23");
  std::ostringstream out3;
  outputList(out3, list3);
  BOOST_TEST(out3.str() == "10 11 24 25 21 22 12 13 14 15");
}

BOOST_AUTO_TEST_CASE(list_splice_after_all_list_test)
{
  List< size_t > list;
  for (size_t i = 1; i <= 3; ++i)
  {
    list.pushBack(i);
  }
  List< size_t > list2;
  for (size_t i = 4; i <= 7; ++i)
  {
    list2.pushBack(i);
  }
  list.spliceAfter(list.cbegin(), list2);
  std::ostringstream out;
  outputList(out, list);
  BOOST_TEST(out.str() == "1 4 5 6 7 2 3");
  BOOST_TEST(list.size() == 7);
  BOOST_TEST(list2.size() == 0);
  List< size_t > list3;
  for (size_t i = 8; i <= 10; ++i)
  {
    list3.pushBack(i);
  }
  list.spliceAfter(std::next(list.cbegin(), 6), list3);
  std::ostringstream out2;
  outputList(out2, list);
  BOOST_TEST(out2.str() == "1 4 5 6 7 2 3 8 9 10");
  BOOST_TEST(list.size() == 10);
  BOOST_TEST(list3.size() == 0);
}

BOOST_AUTO_TEST_CASE(list_splice_after_all_list_move_test)
{
  List< size_t > list;
  for (size_t i = 1; i <= 3; ++i)
  {
    list.pushBack(i);
  }
  List< size_t > list2;
  for (size_t i = 4; i <= 7; ++i)
  {
    list2.pushBack(i);
  }
  list.spliceAfter(list.cbegin(), std::move(list2));
  std::ostringstream out;
  outputList(out, list);
  BOOST_TEST(out.str() == "1 4 5 6 7 2 3");
  BOOST_TEST(list.size() == 7);
  BOOST_TEST(list2.size() == 0);
  List< size_t > list3;
  for (size_t i = 8; i <= 10; ++i)
  {
    list3.pushBack(i);
  }
  list.spliceAfter(std::next(list.cbegin(), 6), std::move(list3));
  std::ostringstream out2;
  outputList(out2, list);
  BOOST_TEST(out2.str() == "1 4 5 6 7 2 3 8 9 10");
  BOOST_TEST(list.size() == 10);
  BOOST_TEST(list3.size() == 0);
}

BOOST_AUTO_TEST_CASE(list_splice_after_one_move_test)
{
  List< size_t > list;
  for (size_t i = 1; i <= 3; ++i)
  {
    list.pushBack(i);
  }
  List< size_t > list2;
  for (size_t i = 4; i <= 7; ++i)
  {
    list2.pushBack(i);
  }
  list.spliceAfter(list.cbegin(), std::move(list2), list2.cbegin());
  std::ostringstream out;
  outputList(out, list);
  BOOST_TEST(out.str() == "1 5 2 3");
  BOOST_TEST(list.size() == 4);
  BOOST_TEST(list2.size() == 3);
}

BOOST_AUTO_TEST_CASE(list_splice_after_one_test)
{
  List< size_t > list;
  for (size_t i = 1; i <= 3; ++i)
  {
    list.pushBack(i);
  }
  List< size_t > list2;
  for (size_t i = 4; i <= 7; ++i)
  {
    list2.pushBack(i);
  }
  list.spliceAfter(list.cbegin(), list2, list2.cbegin());
  std::ostringstream out;
  outputList(out, list);
  BOOST_TEST(out.str() == "1 5 2 3");
  BOOST_TEST(list.size() == 4);
  BOOST_TEST(list2.size() == 3);
}

BOOST_AUTO_TEST_CASE(list_begin_end_test)
{
  List< int > list1;
  list1.pushBack(1);
  list1.pushBack(2);
  auto it1 = list1.begin();
  auto it2 = list1.end();
  BOOST_TEST(*it1 == 1);
  BOOST_TEST(*it2 == 1);
}

