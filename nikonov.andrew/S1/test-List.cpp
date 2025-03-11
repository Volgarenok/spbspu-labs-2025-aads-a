#include <boost/test/unit_test.hpp>
#include <sstream>
#include "List.hpp"
BOOST_AUTO_TEST_CASE(ListStandardConstructor_test)
{
  nikonov::List< int > list;
  BOOST_TEST(list.size() == 0);
}
BOOST_AUTO_TEST_CASE(ListVariableConstructor_test)
{
  constexpr size_t size = 10;
  constexpr int data = 100;
  nikonov::List< int > list(size, data);
  BOOST_TEST(list.size() == size);
  for (auto el : list)
  {
   BOOST_TEST(el == data);
  }
}
BOOST_AUTO_TEST_CASE(ListCopyConstructor_test)
{
  constexpr size_t size = 10;
  constexpr int data = 100;
  nikonov::List< int > list(size, data);
  nikonov::List< int > copyList(list);
  BOOST_TEST(list.size() == copyList.size());
  auto it = list.begin();
  auto copyIt = copyList.begin();
  for (; it != list.end(); ++it, ++copyIt)
  {
    BOOST_TEST(*it == *copyIt);
  }
}
BOOST_AUTO_TEST_CASE(ListMoveConstructor_test)
{
  constexpr size_t size = 10;
  constexpr int data = 100;
  nikonov::List< int > list(10, 100);
  nikonov::List< int > copyList(std::move(list));
  BOOST_TEST(copyList.size() == size);
  for (auto el : copyList)
  {
    BOOST_TEST(el == data);
  }
}
BOOST_AUTO_TEST_CASE(ListInitLConstructor_test)
{
  constexpr int data1 = 10;
  constexpr int data2 = 100;
  nikonov::List< int > list{ data1, data2 };
  BOOST_TEST(list.front() == data1);
  BOOST_TEST(list.back() == data2);
}
BOOST_AUTO_TEST_CASE(ListRangeConstructor_test)
{
  using iter = nikonov::ListIterator< int >;
  constexpr size_t size = 10;
  constexpr int data = 100;
  nikonov::List< int > list(size, data);
  iter begin = list.begin();
  iter end = list.end();
  nikonov::List< int > newList(begin, end);
  iter newBegin = newList.begin();
  for (; newBegin != newList.end(); ++newBegin, ++begin)
  {
    BOOST_TEST(*begin == *newBegin);
  }
}
BOOST_AUTO_TEST_CASE(ListAssignmentOp_test)
{
  constexpr size_t size = 10;
  constexpr int data = 100;
  nikonov::List< int > list(size, data);
  list = { };
  BOOST_TEST(list.empty());
}
BOOST_AUTO_TEST_CASE(ListBegin_test)
{
  constexpr int data1 = 10;
  constexpr int data2 = 100;
  nikonov::List< int > list{ data1, data2 };
  BOOST_TEST(*list.begin() == data1);
}
/*
BOOST_AUTO_TEST_CASE(ListEnd_test)
{
  constexpr int data1 = 10;
  constexpr int data2 = 100;
  nikonov::List< int > list{ data1, data2 };
  BOOST_TEST(list.end() != list.begin());
}
*/
BOOST_AUTO_TEST_CASE(ListFront_test)
{
  constexpr int data1 = 10;
  constexpr int data2 = 100;
  nikonov::List< int > list{ data1, data2 };
  BOOST_TEST(list.front() == data1);
}
BOOST_AUTO_TEST_CASE(ListBack_test)
{
  constexpr int data1 = 10;
  constexpr int data2 = 100;
  nikonov::List< int > list{ data1, data2 };
  BOOST_TEST(list.back() == data2);
}
BOOST_AUTO_TEST_CASE(ListEmpty_test)
{
  nikonov::List< int > list{ };
  BOOST_TEST(list.empty());
}
BOOST_AUTO_TEST_CASE(ListSize_test)
{
  constexpr size_t size = 10;
  constexpr int data = 100;
  nikonov::List< int > list(size, data);
  BOOST_TEST(list.size() == size);
}
BOOST_AUTO_TEST_CASE(ListPushFront_test)
{
  constexpr int data1 = 1;
  constexpr int data2 = 2;
  constexpr int data3 = 3;
  nikonov::List< int > list{ data2, data3 };
  list.push_front(data1);
  BOOST_TEST(list.front() == data1);
}
BOOST_AUTO_TEST_CASE(ListPushBack_test)
{
  constexpr int data = 100;
  nikonov::List< int > list{ };
  list.push_back(data);
  BOOST_TEST(list.back() == data);
}
BOOST_AUTO_TEST_CASE(ListPopFront_test)
{
  constexpr int data1 = 10;
  constexpr int data2 = 20;
  constexpr int data3 = 30;
  nikonov::List< int > list{ data1, data2, data3 };
  list.pop_front();
  BOOST_TEST(list.front() == data2);
}
BOOST_AUTO_TEST_CASE(ListPopBack_test)
{
  constexpr int data1 = 10;
  constexpr int data2 = 20;
  constexpr int data3 = 30;
  nikonov::List< int > list{ data1, data2, data3 };
  list.pop_back();
  BOOST_TEST(list.back() == data2);
}
BOOST_AUTO_TEST_CASE(ListSwap_test)
{
  constexpr int data1 = 10;
  constexpr int data2 = 20;
  nikonov::List< int > list1{ data1 };
  nikonov::List< int > list2{ data2 };
  list1.swap(list2);
  BOOST_TEST(list1.back() == data2);
  BOOST_TEST(list2.back() == data1);
}
BOOST_AUTO_TEST_CASE(ListClear_test)
{
  constexpr int data1 = 10;
  constexpr int data2 = 20;
  constexpr int data3 = 30;
  nikonov::List< int > list{ data1, data2, data3 };
  list.clear();
  BOOST_TEST(list.empty());
}
BOOST_AUTO_TEST_CASE(ListRemove_test)
{
  constexpr int data1 = 10;
  constexpr int data2 = 20;
  constexpr int data3 = 30;
  nikonov::List< int > list{ data3, data1, data2, data3 };
  list.remove(data3);
  BOOST_TEST(list.front() == data1);
  BOOST_TEST(list.back() == data2);
}
BOOST_AUTO_TEST_CASE(ListRemoveIf_test)
{
  struct moreThen100
  {
    bool operator()(int val)
    {
      return val > 100;
    }
  };
  constexpr int data1 = 99;
  constexpr int data2 = 200;
  constexpr int data3 = 300;
  moreThen100 pred;
  nikonov::List< int > list{ data3, data1, data2, data3 };
  list.remove_if(pred);
  BOOST_TEST(list.front() == data1);
  BOOST_TEST(list.back() == data1);
}
BOOST_AUTO_TEST_CASE(ListAssignFill_test)
{
  constexpr int data1 = 10;
  constexpr int data2 = 20;
  constexpr int data3 = 30;
  nikonov::List< int > list{ data1, data2, data3 };
  list.assign(data2, data3);
  BOOST_TEST(list.size() == data2);
  for (auto el : list)
  {
    BOOST_TEST(el == data3);
  }
}
BOOST_AUTO_TEST_CASE(ListAssignRange_test)
{
  constexpr int data1 = 10;
  constexpr int data2 = 20;
  nikonov::List< int > list{ data1, data2 };
  nikonov::List< int > diffList{ data2, data2, data2 };
  list.assign(diffList.begin(), diffList.end());
  BOOST_TEST(list.size() == 3);
  for (auto el : list)
  {
    BOOST_TEST(el == data2);
  }
}
BOOST_AUTO_TEST_CASE(ListAssignInitL_test)
{
  constexpr int data1 = 10;
  constexpr int data2 = 20;
  constexpr int data3 = 30;
  constexpr int data4 = 40;
  nikonov::List< int > list{ data1, data2, data3 };
  list.assign({ data4, data4 });
  BOOST_TEST(list.size() == 2);
  for (auto el : list)
  {
    BOOST_TEST(el == data4);
  }
}
BOOST_AUTO_TEST_CASE(ListIsert_test)
{
  constexpr int data1 = 10;
  constexpr int data2 = 20;
  constexpr int data3 = 30;
  constexpr int newVal = 144;
  nikonov::List< int > list{ data1, data2, data3 };
  list.insert(++list.cbegin(), newVal);
  BOOST_TEST(*(++list.cbegin()) == newVal);
}
BOOST_AUTO_TEST_CASE(ListErase_test)
{
  constexpr int data1 = 10;
  constexpr int data2 = 20;
  constexpr int data3 = 30;
  nikonov::List< int > list{ data1, data2, data3 };
  nikonov::ConstListIterator< int > iter = list.cbegin();
  ++iter;
  nikonov::ConstListIterator< int > prev = iter++;
  list.erase(iter);
  BOOST_TEST(*prev == data2);
}
