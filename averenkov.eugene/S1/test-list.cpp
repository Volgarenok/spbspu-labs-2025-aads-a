#include <boost/test/unit_test.hpp>
#include <sstream>
#include <string>
#include "list.hpp"

using list_t = averenkov::List< int >;

std::string printList(list_t list)
{
  std::ostringstream out;
  for (auto it = list.begin(); it != list.end(); it++)
  {
    out << *it;
  }
  return out.str();
}

BOOST_AUTO_TEST_SUITE(ListTest)

BOOST_AUTO_TEST_CASE(DefaultConstructor)
{
  list_t list;
  BOOST_TEST(list.empty());
}

BOOST_AUTO_TEST_CASE(CopyConstructor)
{
  list_t list;
  list_t copyList1(list);
  BOOST_TEST(copyList1.empty());

  list_t list2{ 1, 2, 3 };
  list_t copyList(list2);
  BOOST_TEST(printList(copyList) == "123");
}

BOOST_AUTO_TEST_CASE(MoveConstructor)
{
  list_t list({ 1, 2 });
  list_t movedList(std::move(list));
  BOOST_TEST(printList(movedList) == "12");
  BOOST_TEST(list.empty());
}

BOOST_AUTO_TEST_CASE(FillConstructor)
{
  list_t emplist(0, 1);
  BOOST_TEST(emplist.empty());
  list_t list(3, 5);
  BOOST_TEST(printList(list) == "555");
}

BOOST_AUTO_TEST_CASE(BeginEnd)
{
  list_t list({ 1, 2 });
  auto it = list.begin();
  BOOST_TEST(*it == 1);
  ++it;
  BOOST_TEST(*it == 2);
  ++it;
  auto it2 = list.end();
  BOOST_TEST(*it == *it2);
}

BOOST_AUTO_TEST_CASE(CBeginCEnd)
{
  list_t list({ 1, 2 });
  auto it = list.cbegin();
  BOOST_TEST(*it == 1);
  ++it;
  BOOST_TEST(*it == 2);
  ++it;
  auto it2 = list.cend();
  BOOST_TEST(*it == *it2);
}


BOOST_AUTO_TEST_CASE(FrontBack)
{
  list_t list({ 1, 2 });
  BOOST_TEST(list.back() == 2);
  BOOST_TEST(list.front() == 1);
}

BOOST_AUTO_TEST_CASE(PushFront)
{
  list_t list({ 1, 3 });
  list.push_front(1);
  BOOST_TEST(printList(list) == "113");
  list.push_front(2);
  BOOST_TEST(printList(list) == "2113");
}

BOOST_AUTO_TEST_CASE(PushBack)
{
  list_t list;
  list.push_back(1);
  BOOST_TEST(printList(list) == "1");
  list.push_back(2);
  list.push_back(3);
  BOOST_TEST(printList(list) == "123");
}

BOOST_AUTO_TEST_CASE(PopFront)
{
  list_t list({ 1, 2, 3 });
  list.pop_front();
  BOOST_TEST(printList(list) == "23");
}

BOOST_AUTO_TEST_CASE(PopBack)
{
  list_t list({ 1, 2, 3 });
  list.pop_back();
  BOOST_TEST(printList(list) == "12");
}

BOOST_AUTO_TEST_CASE(Clear)
{
  list_t list;
  list.clear();
  BOOST_TEST(list.empty());
  list_t list2({ 1, 2, 3 });
  list2.clear();
  BOOST_TEST(list2.empty());
}

BOOST_AUTO_TEST_CASE(Swap)
{
  list_t list1({ 1, 2 });
  list_t list2({ 3, 4 });
  list1.swap(list2);
  BOOST_TEST(printList(list1) == "34");
  BOOST_TEST(printList(list2) == "12");
}

BOOST_AUTO_TEST_CASE(Remove)
{
  list_t list1({ 1 });
  list1.remove(1);
  BOOST_TEST(list1.empty());
  list_t list2({ 1 });
  list1.remove(2);
  BOOST_TEST(printList(list2) == "1");
  list_t list3({ 1, 2, 2, 3 });
  list3.remove(2);
  BOOST_TEST(printList(list3) == "13");
}

bool isEven(int value)
{
  return value % 2 == 0;
}

BOOST_AUTO_TEST_CASE(RemoveIf)
{
  list_t list1({ 2 });
  list1.removeIf(isEven);
  BOOST_TEST(list1.empty());
  list_t list2({ 1 });
  list2.removeIf(isEven);
  BOOST_TEST(printList(list2) == "1");
  list_t list3({ 1, 2, 3 });
  list3.removeIf(isEven);
  BOOST_TEST(printList(list3) == "13");
}

BOOST_AUTO_TEST_CASE(Assign)
{
  list_t list;
  list.assign(0, 0);
  BOOST_TEST(list.empty());
  list.assign(3, 5);
  BOOST_TEST(printList(list) == "555");
}

BOOST_AUTO_TEST_SUITE_END()
