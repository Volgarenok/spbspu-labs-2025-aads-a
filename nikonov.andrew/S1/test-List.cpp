#include <boost/test/unit_test.hpp>
#include "List.hpp"
BOOST_AUTO_TEST_CASE(ListDefaultConstructor_test)
{
  nikonov::List< int > list;
  BOOST_TEST(list.size() == 0);
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
  nikonov::List< int > list(size, data);
  nikonov::List< int > copyList(std::move(list));
  BOOST_TEST(copyList.size() == size);
  for (auto el: copyList)
  {
    BOOST_TEST(el == data);
  }
}
BOOST_AUTO_TEST_CASE(ListFillConstructor_test)
{
  constexpr size_t size = 10;
  constexpr int data = 100;
  nikonov::List< int > list(size, data);
  BOOST_TEST(list.size() == size);
  for (auto el: list)
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
  constexpr size_t size = 10;
  constexpr int data = 100;
  nikonov::List< int > list(size, data);
  nikonov::ListIterator< int > begin = list.begin();
  nikonov::ListIterator< int > end = list.end();
  nikonov::List< int > newList(begin, end);
  nikonov::ListIterator< int > newBegin = newList.begin();
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
BOOST_AUTO_TEST_CASE(ListEqualityOp_test)
{
  nikonov::List< int > list1{ 10, 20, 30 };
  nikonov::List< int > list2{ 20, 20, 30 };
  nikonov::List< int > list3{ 10, 20, 30 };
  BOOST_TEST(list1 == list3);
  BOOST_TEST(list1 != list2);
  BOOST_TEST(list3 == list3);
}
BOOST_AUTO_TEST_CASE(ListGreaterLessOp_test)
{
  nikonov::List< int > list1{ 10, 10 };
  nikonov::List< int > list2{ 30, 30 };
  nikonov::List< int > list3;
  BOOST_TEST(list2 > list1);
  BOOST_TEST(list1 > list3);
  BOOST_TEST(list1 < list2);
  BOOST_TEST(list3 < list1);
  nikonov::List< int > list4{ 30, 30 };
  BOOST_TEST(list2 <= list4);
  BOOST_TEST(list2 >= list1);
}
BOOST_AUTO_TEST_CASE(ListBegin_test)
{
  constexpr int data1 = 10;
  constexpr int data2 = 100;
  nikonov::List< int > list{ data1, data2 };
  BOOST_TEST(*list.begin() == data1);
}
BOOST_AUTO_TEST_CASE(ListEnd_test)
{
  constexpr int data1 = 10;
  constexpr int data2 = 100;
  nikonov::List< int > list{ data1, data2 };
  nikonov::ListIterator< int > data1It = list.begin();
  nikonov::ListIterator< int > data2It = data1It++;
  BOOST_TEST(list.end().node != data1It.node);
  BOOST_TEST(list.end().node != data2It.node);
}
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
  nikonov::List< int > list{};
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
  constexpr size_t size = 20;
  constexpr int data2 = 20;
  nikonov::List< int > list(size, data1);
  list.push_back(data2);
  list.remove(data1);
  BOOST_TEST(list.size() == 1);
  BOOST_TEST(list.front() == data2);
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
BOOST_AUTO_TEST_CASE(ListSpliceEntireList_test)
{
  nikonov::List< int > list1{ 10, 20, 30 };
  nikonov::List< int > list2{ 40, 50, 60 };
  auto iter1 = list1.cbegin();
  auto iter2 = list2.cbegin();
  *iter1++;
  *iter1++;
  *iter1++;
  list1.splice(iter1, list2);
  iter1 = list1.cbegin();
  *iter1++;
  *iter1++;
  *iter1++;
  BOOST_TEST(list2.empty());
  BOOST_TEST(*(iter1++) == *(iter2++));
  BOOST_TEST(*(iter1++) == *(iter2++));
  BOOST_TEST(*iter1 == *iter2);
}
BOOST_AUTO_TEST_CASE(ListSpliceSingleEl_test)
{
  nikonov::List< int > list1{ 10, 20, 30 };
  nikonov::List< int > list2{ 40, 50, 60 };
  size_t size2BeforeSplice = list2.size();
  const int first2ListElem = *(list2.cbegin());
  auto iter1 = list1.cbegin();
  auto iter2 = list2.cbegin();
  list1.splice(iter1, list2, iter2);
  iter1 = list1.cbegin();
  BOOST_TEST(list2.size() == --size2BeforeSplice);
  BOOST_TEST(*(iter1) == first2ListElem);
}
BOOST_AUTO_TEST_CASE(ListSpliceElRange_test)
{
  nikonov::List< int > list1{ 10, 20, 30 };
  nikonov::List< int > list2{ 40, 50, 60 };
  auto iter1 = list1.cbegin();
  auto iter2First = list2.cbegin();
  auto iter2End = list2.cend();
  ++iter2First;
  list1.splice(iter1, list2, iter2First, iter2End);
  iter1 = list1.cbegin();
  BOOST_TEST(*(iter1++) == 50);
  BOOST_TEST(*(iter1) == 60);
}
BOOST_AUTO_TEST_CASE(ListReverse_test)
{
  nikonov::List< int > list1{ 10, 20, 30, 40 };
  list1.reverse();
  auto iter = list1.cbegin();
  BOOST_TEST(*(iter++) == 40);
  BOOST_TEST(*(iter++) == 30);
  BOOST_TEST(*(iter++) == 20);
  BOOST_TEST(*(iter) == 10);
  nikonov::List< int > list2{ 50 };
  list2.reverse();
  auto iter2 = list2.cbegin();
  BOOST_TEST(*(iter2++) == 50);
  BOOST_TEST(iter2.node == list2.cend().node);
}
BOOST_AUTO_TEST_CASE(ListAssignFill_test)
{
  constexpr int data1 = 10;
  constexpr int data2 = 20;
  constexpr int data3 = 30;
  nikonov::List< int > list{ data1, data2, data3 };
  list.assign(data2, data3);
  BOOST_TEST(list.size() == data2);
  for (auto el: list)
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
  for (auto el: list)
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
  for (auto el: list)
  {
    BOOST_TEST(el == data4);
  }
}
BOOST_AUTO_TEST_CASE(ListIsertSingleEl_test)
{
  constexpr int data1 = 10;
  constexpr int data2 = 20;
  constexpr int data3 = 30;
  constexpr int newVal = 144;
  nikonov::List< int > list{ data1, data2, data3 };
  list.insert(++list.cbegin(), newVal);
  BOOST_TEST(*(++list.cbegin()) == newVal);
}
BOOST_AUTO_TEST_CASE(ListInsertFill_test)
{
  constexpr int data1 = 10;
  constexpr int data2 = 20;
  constexpr int newVal = 30;
  constexpr size_t size = 3;
  nikonov::List< int > list{ data1, data2 };
  auto iter1 = list.cbegin();
  ++iter1;
  list.insert(iter1, size, newVal);
  BOOST_TEST(list.size() == 5);
  auto iter2 = list.cbegin();
  BOOST_TEST(*(iter2++) == data1);
  BOOST_TEST(*(iter2++) == newVal);
  BOOST_TEST(*(iter2++) == newVal);
  BOOST_TEST(*(iter2++) == newVal);
  BOOST_TEST(*iter2 == data2);
}
BOOST_AUTO_TEST_CASE(ListInsertRange_test)
{
  constexpr int data1 = 10;
  constexpr int data2 = 20;
  nikonov::List< int > list{ data1, data2 };
  nikonov::List< int > list2{ 30, 40, 50 };
  auto iter1 = list.cbegin();
  ++iter1;
  list.insert(iter1, list2.begin(), list2.end());
  BOOST_TEST(list.size() == 5);
  auto iter2 = list.cbegin();
  BOOST_TEST(*(iter2++) == data1);
  BOOST_TEST(*(iter2++) == 30);
  BOOST_TEST(*(iter2++) == 40);
  BOOST_TEST(*(iter2++) == 50);
  BOOST_TEST(*(iter2++) == data2);
}
BOOST_AUTO_TEST_CASE(ListRValue_test)
{
  nikonov::List< int > list;
  list.insert(list.cbegin(), 52);
  BOOST_TEST(list.size() == 1);
  BOOST_TEST(*list.begin() == 52);
}
BOOST_AUTO_TEST_CASE(ListInsertInitializerList_AdditionalTest)
{
  constexpr int data1 = 1;
  constexpr int data2 = 2;
  constexpr int data3 = 3;
  constexpr int data4 = 4;
  nikonov::List< int > list{ data1, data4 };
  auto iter1 = list.cbegin();
  ++iter1;
  list.insert(iter1, { data2, data3 });
  BOOST_TEST(list.size() == 4);
  auto iter2 = list.cbegin();
  BOOST_TEST(*(iter2++) == data1);
  BOOST_TEST(*(iter2++) == data2);
  BOOST_TEST(*(iter2++) == data3);
  BOOST_TEST(*(iter2++) == data4);
}
BOOST_AUTO_TEST_CASE(ListErase_test)
{
  constexpr int data1 = 10;
  constexpr int data2 = 20;
  constexpr int data3 = 30;
  nikonov::List< int > list{ data1, data2, data3 };
  nikonov::ConstListIterator< int > iter = list.cbegin();
  ++iter;
  list.erase(iter);
  BOOST_TEST(list.size() == 2);
  BOOST_TEST(list.front() == data1);
  BOOST_TEST(list.back() == data3);
  nikonov::ConstListIterator< int > first = list.cbegin();
  nikonov::ConstListIterator< int > last = list.cend();
  nikonov::ListIterator< int > lastAfterErase = list.erase(first, last);
  BOOST_TEST(list.size() == 0);
  BOOST_TEST(list.end().node == lastAfterErase.node);
}
