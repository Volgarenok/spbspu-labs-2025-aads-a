#include <boost/test/unit_test.hpp>
#include <sstream>
#include "list.hpp"

using namespace tkach;

namespace
{
  template< typename T >
  void outputList(std::ostream& out, const List< T >& list)
  {
    if (list.empty())
    {
      return;
    }
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
  BOOST_TEST(list.size() == 1);
  BOOST_TEST(list.front() == 1);
  a = 2;
  list.pushFront(a);
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
}

BOOST_AUTO_TEST_CASE(list_clear_test)
{
  List< int > list;
  list.pushBack(1);
  list.pushBack(2);
  list.clear();
  BOOST_TEST(list.empty());
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
  List< int > list;
  for (size_t i = 1; i <= 3; ++i)
  {
    list.pushBack(i);
  }
  List< int > list2(list);
  auto it = list.begin();
  std::ostringstream out;
  outputList(out, list2);
  out << "  ";
  outputList(out, list2);
  BOOST_TEST(out.str() == "1 2 3  1 2 3");
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
  List<int> list(count, 54);
  BOOST_TEST(list.size() == 5);
  auto it = list.begin();
  for (size_t i = 0; i < 5; ++i)
  {
    BOOST_TEST(*(it++) == 54);
  }
}

BOOST_AUTO_TEST_CASE(list_erase_after_test)
{
  List< size_t > list;
  for (size_t i = 1; i <= 2; ++i)
  {
    list.pushBack(i);
  }
  auto it = ++list.cbegin();
  auto it3 = list.eraseAfter(it);
  BOOST_TEST(*(it3) == 2);
  BOOST_TEST(list.size() == 1);
}

BOOST_AUTO_TEST_CASE(list_erase_after_first_last_test)
{
  List< size_t > list;
  for (size_t i = 1; i <= 5; ++i)
  {
    list.pushBack(i);
  }
  auto it = list.eraseAfter(list.cbegin(), std::next(list.cbegin(), 3));
  BOOST_TEST(*it == 4);
  BOOST_TEST(list.size() == 3);
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
  list.removeIf(
    [](size_t n)
    {
      return n < 10;
    }
  );
  BOOST_TEST(list.size() == 4);
  std::ostringstream out;
  outputList(out, list);
  BOOST_TEST(out.str() == "11 11 11 12");
}

BOOST_AUTO_TEST_CASE(list_splice_after_range_cbegin_cend_test)
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
  BOOST_TEST(list2.front() == 4);
}

BOOST_AUTO_TEST_CASE(list_splice_after_random_range_test)
{
  List< size_t > list;
  for (size_t i = 10; i <= 15; ++i)
  {
    list.pushBack(i);
  }
  List< size_t > list2;
  for (size_t i = 20; i <= 25; ++i)
  {
    list2.pushBack(i);
  }
  list.spliceAfter(std::next(list.cbegin()), list2, list2.cbegin(), std::next(list2.cbegin(), 3));
  BOOST_TEST(list.size() == 8);
  BOOST_TEST(list2.size() == 4);
  std::ostringstream out;
  outputList(out, list);
  out << "  ";
  outputList(out, list2);
  BOOST_TEST(out.str() == "10 11 21 22 12 13 14 15  20 23 24 25");
}

BOOST_AUTO_TEST_CASE(list_splice_after_random_range_including_last_test)
{
  List< size_t > list;
  for (size_t i = 10; i <= 15; ++i)
  {
    list.pushBack(i);
  }
  List< size_t > list2;
  for (size_t i = 20; i <= 25; ++i)
  {
    list2.pushBack(i);
  }
  list.spliceAfter(std::next(list.cbegin()), list2, std::next(list2.cbegin(), 1), std::next(list2.cbegin(), 6));
  BOOST_TEST(list.size() == 10);
  BOOST_TEST(list2.size() == 2);
  std::ostringstream out;
  outputList(out, list);
  out << "  ";
  outputList(out, list2);
  BOOST_TEST(out.str() == "10 11 22 23 24 25 12 13 14 15  20 21");
}

BOOST_AUTO_TEST_CASE(list_splice_after_rvelue_random_range_including_last_test)
{
  List< size_t > list;
  for (size_t i = 10; i <= 15; ++i)
  {
    list.pushBack(i);
  }
  List< size_t > list2;
  for (size_t i = 20; i <= 25; ++i)
  {
    list2.pushBack(i);
  }
  list.spliceAfter(std::next(list.cbegin()), std::move(list2), std::next(list2.cbegin(), 1), std::next(list2.cbegin(), 6));
  BOOST_TEST(list.size() == 10);
  BOOST_TEST(list2.size() == 2);
  std::ostringstream out;
  outputList(out, list);
  out << "  ";
  outputList(out, list2);
  BOOST_TEST(out.str() == "10 11 22 23 24 25 12 13 14 15  20 21");
}

BOOST_AUTO_TEST_CASE(list_splice_after_all_test)
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
  BOOST_TEST(list2.empty());
}

BOOST_AUTO_TEST_CASE(list_splice_after_all_list_rvalue__test)
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
  BOOST_TEST(list2.empty());
}

BOOST_AUTO_TEST_CASE(list_splice_after_all_list_back_test)
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
  list.spliceAfter(std::next(list.cbegin(), 2), list2);
  std::ostringstream out;
  outputList(out, list);
  BOOST_TEST(out.str() == "1 2 3 4 5 6 7");
  BOOST_TEST(list.size() == 7);
  BOOST_TEST(list2.empty());
}


BOOST_AUTO_TEST_CASE(list_splice_after_one_rvalue_test)
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

BOOST_AUTO_TEST_CASE(list_initializer_constructor_test)
{
  List< int > list{1, 3, 5};
  std::ostringstream out;
  outputList(out, list);
  BOOST_TEST(out.str() == "1 3 5");
  BOOST_TEST(list.size() == 3);
}

BOOST_AUTO_TEST_CASE(list_initializer_constructor_empty_test)
{
  List< int > list{};
  std::ostringstream out;
  outputList(out, list);
  BOOST_TEST(out.str() == "");
  BOOST_TEST(list.empty());
}

BOOST_AUTO_TEST_CASE(list_range_constructor_test)
{
  List< int > list{1, 3, 5};
  List< int > list2(std::next(list.begin(), 1), list.end());
  std::ostringstream out;
  outputList(out, list2);
  BOOST_TEST(out.str() == "3 5");
  BOOST_TEST(list2.size() == 2);
}

BOOST_AUTO_TEST_CASE(list_insert_after_test)
{
  List< int > list{1, 3, 5};
  auto it = list.insertAfter(list.cbegin(), 9);
  std::ostringstream out;
  outputList(out, list);
  BOOST_TEST(out.str() == "1 9 3 5");
  BOOST_TEST(list.size() == 4);
  BOOST_TEST(*it == 9);
}

BOOST_AUTO_TEST_CASE(list_insert_after_tail_test)
{
  List< int > list{1, 3, 5};
  auto it = list.insertAfter(std::next(list.cbegin(), 2), 9);
  std::ostringstream out;
  outputList(out, list);
  BOOST_TEST(out.str() == "1 3 5 9");
  BOOST_TEST(list.size() == 4);
  BOOST_TEST(*it == 9);
}

BOOST_AUTO_TEST_CASE(list_insert_after_lvalue_test)
{
  List< int > list{1, 3, 5};
  int temp = 9;
  auto it = list.insertAfter(list.cbegin(), temp);
  std::ostringstream out;
  outputList(out, list);
  BOOST_TEST(out.str() == "1 9 3 5");
  BOOST_TEST(list.size() == 4);
  BOOST_TEST(temp == 9);
  BOOST_TEST(*it == 9);
}

BOOST_AUTO_TEST_CASE(list_range_constructor_first_equal_last_test)
{
  List< int > list{1, 2, 3};
  List< int > list2(list.begin(), list.end());
  std::ostringstream out;
  outputList(out, list2);
  BOOST_TEST(out.str() == "");
  BOOST_TEST(list2.empty());
}

BOOST_AUTO_TEST_CASE(list_insert_after_count_test)
{
  List< int > list{1, 3, 5};
  size_t temp = 3;
  auto it = list.insertAfter(list.cbegin(), temp, 9);
  std::ostringstream out;
  outputList(out, list);
  BOOST_TEST(out.str() == "1 9 9 9 3 5");
  BOOST_TEST(list.size() == 6);
  BOOST_TEST(*(it++) == 9);
  BOOST_TEST(*it == 3);
}

BOOST_AUTO_TEST_CASE(list_insert_after_first_last_test)
{
  List< int > list{1, 3, 5};
  List< int > list2{2, 4, 6, 8};
  auto it = list.insertAfter(list.cbegin(), list2.begin(), std::next(list2.begin(), 3));
  std::ostringstream out;
  outputList(out, list);
  BOOST_TEST(out.str() == "1 2 4 6 3 5");
  BOOST_TEST(list.size() == 6);
  BOOST_TEST(*(it++) == 6);
  BOOST_TEST(*it == 3);
}

BOOST_AUTO_TEST_CASE(list_insert_after_init_list_test)
{
  List< int > list{1, 3, 5};
  auto it = list.insertAfter(list.cbegin(), {2, 4, 6, 8});
  std::ostringstream out;
  outputList(out, list);
  BOOST_TEST(out.str() == "1 2 4 6 8 3 5");
  BOOST_TEST(list.size() == 7);
  BOOST_TEST(*(it++) == 8);
  BOOST_TEST(*it == 3);
}

BOOST_AUTO_TEST_CASE(list_assign_first_last_test)
{
  List< int > list{1, 3, 5};
  List< int > list2{2, 4, 6, 8};
  list.assign(list2.begin(), std::next(list2.begin(), 3));
  std::ostringstream out;
  outputList(out, list);
  BOOST_TEST(out.str() == "2 4 6");
  BOOST_TEST(list.size() == 3);
}

BOOST_AUTO_TEST_CASE(list_assign_init_list_test)
{
  List< int > list{1, 3, 5};
  list.assign({2, 4, 6, 8});
  std::ostringstream out;
  outputList(out, list);
  BOOST_TEST(out.str() == "2 4 6 8");
  BOOST_TEST(list.size() == 4);
}

BOOST_AUTO_TEST_CASE(list_equal_lvalue_operator_test)
{
  List< int > list{1, 3, 5};
  List< int > list2{2, 4, 6, 8};
  list = list2;
  std::ostringstream out;
  outputList(out, list);
  BOOST_TEST(out.str() == "2 4 6 8");
  BOOST_TEST(list.size() == 4);
}

BOOST_AUTO_TEST_CASE(list_equal_rvalue_operator_test)
{
  List< int > list{1, 3, 5};
  List< int > list2{2, 4, 6, 8};
  list = std::move(list2);
  std::ostringstream out;
  outputList(out, list);
  BOOST_TEST(out.str() == "2 4 6 8");
  BOOST_TEST(list.size() == 4);
  BOOST_TEST(list2.empty());
}

BOOST_AUTO_TEST_CASE(list_equal_rvalue_operator_init_list_test)
{
  List< int > list{1, 3, 5};
  list = {2, 4, 6, 8};
  std::ostringstream out;
  outputList(out, list);
  BOOST_TEST(out.str() == "2 4 6 8");
  BOOST_TEST(list.size() == 4);
}

BOOST_AUTO_TEST_CASE(list_begin_end_test)
{
  List< int > list1;
  list1.pushBack(1);
  list1.pushBack(2);
  BOOST_TEST(*(list1.begin()) == 1);
  BOOST_TEST(*(list1.end()) == 1);
}

