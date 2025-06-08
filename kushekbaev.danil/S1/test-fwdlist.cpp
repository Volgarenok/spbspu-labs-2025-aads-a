#include <boost/test/unit_test.hpp>
#include <sstream>
#include <string>
#include "fwdlist.hpp"

using list_t = kushekbaev::FwdList< int >;

namespace
{
  std::string printList(list_t list)
  {
    std::ostringstream out;
    for (auto it = list.begin(); it != list.end(); it++)
    {
      out << *it;
    }
    return out.str();
  }

  bool isEven(int value)
  {
    return value % 2 == 0;
  }
}

BOOST_AUTO_TEST_SUITE(constructors)

BOOST_AUTO_TEST_CASE(defaultConstructor)
{
  list_t list;
  BOOST_TEST(list.empty());
}

BOOST_AUTO_TEST_CASE(copyEmptyList)
{
  list_t list1;
  list_t list2(list1);
  BOOST_TEST(list2.empty());
}

BOOST_AUTO_TEST_CASE(copyListWithValues)
{
  list_t list1{ 1, 2, 3 };
  list_t list2(list1);
  BOOST_TEST(printList(list2) == "123");
}

BOOST_AUTO_TEST_CASE(moveConstructor)
{
  list_t list1({ 0, 1 });
  list_t list2(std::move(list1));
  BOOST_TEST(printList(list2) == "01");
  BOOST_TEST(list1.empty());
}

BOOST_AUTO_TEST_CASE(initlistConstructor)
{
  list_t list1{ 1, 2, 3, 4 };
  BOOST_TEST(printList(list1) == "1234");
}

BOOST_AUTO_TEST_CASE(iteratorConstructor)
{
  list_t list1{ 1, 2, 4, 3 };
  list_t list2(list1.begin(), list1.end());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(iterators)

BOOST_AUTO_TEST_CASE(beginEnd)
{
  list_t list({ 1, 2, 3 });
  auto it1 = list.begin();
  BOOST_TEST(*it1 == 1);
  ++it1;
  BOOST_TEST(*it1 == 2);
  ++it1;
  BOOST_TEST(*it1 == 3);
  ++it1;
  auto it2 = list.end();
  BOOST_TEST(*it1 == *it2);
}

BOOST_AUTO_TEST_CASE(constBeginEnd)
{
  list_t list({ 1, 2, 3 });
  auto it1 = list.cbegin();
  BOOST_TEST(*it1 == 1);
  ++it1;
  BOOST_TEST(*it1 == 2);
  ++it1;
  BOOST_TEST(*it1 == 3);
  ++it1;
  auto it2 = list.cend();
  BOOST_TEST(*it1 == *it2);
}

BOOST_AUTO_TEST_CASE(beforeBegin)
{
  list_t list({ 4, 5, 6 });
  auto it = list.before_begin();
  ++it;
  BOOST_TEST(*it == 4);
}

BOOST_AUTO_TEST_CASE(constBeforeBegin)
{
  list_t list({ 4, 5, 6 });
  auto it = list.cbefore_begin();
  ++it;
  BOOST_TEST(*it == 4);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(elementAccess)

BOOST_AUTO_TEST_CASE(front)
{
  list_t list({ 1, 2, 3 });
  BOOST_TEST(list.front() == 1);
}

BOOST_AUTO_TEST_CASE(back)
{
  list_t list({ 1, 2, 3 });
  BOOST_TEST(list.back() == 3);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(modifiers)

BOOST_AUTO_TEST_CASE(assignFill)
{
  list_t list;
  list.assign(3, 20);
  BOOST_TEST(printList(list) == "202020");
}

BOOST_AUTO_TEST_CASE(assignRange)
{
  list_t list1{1, 3, 3, 7};
  list_t list2;
  list2.assign(list1.begin(), list1.end());
  BOOST_TEST(printList(list2) == "1337");
}

BOOST_AUTO_TEST_CASE(pushfront)
{
  list_t list({ 1, 2, 3 });
  list.push_front(0);
  BOOST_TEST(printList(list) == "0123");
}

BOOST_AUTO_TEST_CASE(popfront)
{
  list_t list({ 0, 1, 2, 3 });
  list.pop_front();
  BOOST_TEST(printList(list) == "123");
}

BOOST_AUTO_TEST_CASE(clear)
{
  list_t list({ 1, 2, 3 });
  list.clear();
  BOOST_TEST(list.empty());
}

BOOST_AUTO_TEST_CASE(swap)
{
  list_t list1({ 1337 });
  list_t list2({ 1984 });
  list1.swap(list2);
  BOOST_TEST(printList(list1) == "1984");
  BOOST_TEST(printList(list2) == "1337");
}

BOOST_AUTO_TEST_CASE(insertAfterValue)
{
  list_t list;
  auto it = list.insert_after(list.cend(), 1984);
  BOOST_TEST(*it == 1984);
  it = list.insert_after(list.cbegin(), 1337);
  BOOST_TEST(*it == 1337);
}

BOOST_AUTO_TEST_CASE(inserAfterSizeValue)
{
  list_t list;
  list.insert_after(list.cbefore_begin(), 3, 7);
  BOOST_TEST(printList(list) == "777");
  list.insert_after(list.cbefore_begin(), 7, 3);
  BOOST_TEST(printList(list) == "3333333777");
}

BOOST_AUTO_TEST_CASE(insertAfterFirstLast)
{
  list_t list1;
  list_t list2({ 1, 2, 3 });
  list1.insert_after(list1.cend(), list2.cbegin(), list2.cend());
  BOOST_TEST(printList(list1) == "123");
}

BOOST_AUTO_TEST_CASE(insertAfterInitializerList)
{
  list_t list;
  list.insert_after(list.cbegin(), { 1, 2, 3, 4, 1984 });
  BOOST_TEST(printList(list) == "12341984");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(capacity)

BOOST_AUTO_TEST_CASE(size)
{
  list_t list({ 0, 1, 2, 3, 4, 5, 6 });
  BOOST_TEST(list.size() == 7);
}

BOOST_AUTO_TEST_CASE(empty)
{
  list_t list;
  BOOST_TEST(list.empty());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(operations)

BOOST_AUTO_TEST_CASE(remove)
{
  list_t list({ 1, 2, 1, 3 });
  list.remove(1);
  BOOST_TEST(printList(list) == "23");
}

BOOST_AUTO_TEST_CASE(removeIf)
{
  list_t list({ 1, 2, 3, 4 });
  list.remove_if(isEven);
  BOOST_TEST(printList(list) == "13");
}

BOOST_AUTO_TEST_CASE(spliceAfter)
{
  list_t list1({ 1, 2, 3 });
  list_t list2({ 4, 5, 6 });
  list1.splice_after(list1.begin(), list2);
  BOOST_TEST(list2.empty());
  BOOST_TEST(printList(list1) == "145623");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_CASE(operators)
{
  list_t list1({ 1, 2, 3 });
  list_t list2({ 2, 3, 4 });
  list_t list3({ 1, 2, 3 });
  list_t list4({ 1, 2, 3, 4});
  BOOST_TEST(list1 < list2);
  BOOST_TEST(list1 == list3);
  BOOST_TEST(list4 >= list1);
  BOOST_TEST(list1 != list2);
  BOOST_TEST(list3 <= list1);
  BOOST_TEST(list4 > list3);
}
