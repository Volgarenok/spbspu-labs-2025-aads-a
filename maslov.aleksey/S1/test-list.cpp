#include <boost/test/unit_test.hpp>
#include <sstream>
#include <list/list.hpp>

namespace
{
  std::ostream & printList(std::ostream & out, maslov::FwdList< int > list)
  {
    for (auto it = list.begin(); it != list.end(); it++)
    {
      out << *it;
    }
    return out;
  }
  bool lessThanThree(const int & value)
  {
    return (value < 3);
  }
}

BOOST_AUTO_TEST_SUITE(constructors)

BOOST_AUTO_TEST_CASE(defaultConstructor)
{
  maslov::FwdList< int > list;
  BOOST_TEST(list.empty());
}

BOOST_AUTO_TEST_CASE(fillConstructor)
{
  maslov::FwdList< int > list(4, 4);
  BOOST_TEST(list.size() == 4);
  std::ostringstream out;
  printList(out, list);
  BOOST_TEST(out.str() == "4444");
}

BOOST_AUTO_TEST_CASE(copyConstructor)
{
  maslov::FwdList< int > list1({1, 2});
  maslov::FwdList< int > list2 = list1;
  BOOST_TEST(list1 == list2);
  list2.pushFront(3);
  BOOST_TEST(list1 != list2);
}

BOOST_AUTO_TEST_CASE(moveConstructor)
{
  maslov::FwdList< int > list1({1, 2});
  maslov::FwdList< int > list3 = list1;
  maslov::FwdList< int > list2 = std::move(list1);
  BOOST_TEST(list1.empty());
  BOOST_TEST(list2 == list3);
}

BOOST_AUTO_TEST_CASE(initializerListConstructor)
{

  std::initializer_list< int > il{1, 2, 3};
  maslov::FwdList< int > list1(il);
  maslov::FwdList< int > list2;
  list2.pushFront(3);
  list2.pushFront(2);
  list2.pushFront(1);
  BOOST_TEST(list2 == list1);
}

BOOST_AUTO_TEST_CASE(rangeConstructor)
{
  std::initializer_list< int > il{1, 2, 3};
  maslov::FwdList< int > list1(il.begin(), il.end());
  maslov::FwdList< int > list2({1, 2, 3});
  BOOST_TEST(list2 == list1);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(elementAccess)

BOOST_AUTO_TEST_CASE(front)
{
  maslov::FwdList< int > list({1, 2, 3});
  BOOST_TEST(list.front() == 1);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(operators)

BOOST_AUTO_TEST_CASE(copyOperator)
{
  maslov::FwdList< int > list1({1, 2});
  maslov::FwdList< int > list2;
  list2 = list1;
  BOOST_TEST(list2 == list1);
}

BOOST_AUTO_TEST_CASE(moveOperator)
{
  maslov::FwdList< int > list1({1, 2});
  maslov::FwdList< int > list3 = list1;
  maslov::FwdList< int > list2;
  list2 = std::move(list1);
  BOOST_TEST(list2 == list3);
  BOOST_TEST(list1.empty());
}

BOOST_AUTO_TEST_CASE(equalOperator)
{
  maslov::FwdList< int > list1({1, 2});
  maslov::FwdList< int > list2({1, 2});
  BOOST_TEST(list1 == list2);
  list1.pushFront(3);
  BOOST_TEST(list1 != list2);
}

BOOST_AUTO_TEST_CASE(comparisonOperators)
{
  maslov::FwdList< int > list1({1, 5, 6});
  maslov::FwdList< int > list2({1, 5, 7});
  maslov::FwdList< int > list3({1, 5});
  BOOST_TEST(list1 < list2);
  BOOST_TEST(list3 < list1);
  BOOST_TEST(list2 > list1);
  BOOST_TEST(list1 > list3);
  BOOST_TEST(list1 <= list1);
  BOOST_TEST(list1 >= list1);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(modifiers)

BOOST_AUTO_TEST_CASE(pushFront)
{
  maslov::FwdList< int > list;
  list.pushFront(1);
  BOOST_TEST(list.size() == 1);
  BOOST_TEST(list.front() == 1);
  list.pushFront(2);
  BOOST_TEST(list.size() == 2);
  BOOST_TEST(list.front() == 2);
}

BOOST_AUTO_TEST_CASE(popFront)
{
  maslov::FwdList< int > list({1, 2});
  list.popFront();
  BOOST_TEST(list.size() == 1);
  BOOST_TEST(list.front() == 2);
  list.popFront();
  BOOST_TEST(list.empty());
}

BOOST_AUTO_TEST_CASE(swap)
{
  maslov::FwdList< int > list1({1, 2});
  maslov::FwdList< int > list2({3, 4, 5});
  maslov::FwdList< int > tempList1 = list1;
  maslov::FwdList< int > tempList2 = list2;
  list1.swap(list2);
  BOOST_TEST(list1 == tempList2);
  BOOST_TEST(list2 == tempList1);
}

BOOST_AUTO_TEST_CASE(clear)
{
  maslov::FwdList< int > list({1, 2});
  list.clear();
  BOOST_TEST(list.empty());
}

BOOST_AUTO_TEST_CASE(assignInitializerList)
{
  maslov::FwdList< int > list;
  list.pushFront(1);
  list.assign({2});
  std::ostringstream out;
  printList(out, list);
  BOOST_TEST(out.str() == "2");

  list.assign({1, 2, 3});
  out.str("");
  printList(out, list);
  BOOST_TEST(out.str() == "123");
}

BOOST_AUTO_TEST_CASE(assignRange)
{
  maslov::FwdList< int > list({1, 2, 3});
  list.assign(list.begin(), list.end());
  std::ostringstream out;
  printList(out, list);
  BOOST_TEST(out.str() == "123");

  list.assign({7, 8, 9});
  list.assign(list.cbegin(), list.cend());
  out.str("");
  printList(out, list);
  BOOST_TEST(out.str() == "789");
}

BOOST_AUTO_TEST_CASE(assignFill)
{
  maslov::FwdList< int > list;
  list.assign(3, 4);
  std::ostringstream out;
  printList(out, list);
  BOOST_TEST(out.str() == "444");
}

BOOST_AUTO_TEST_CASE(eraseAfter)
{
  maslov::FwdList< int > list({1, 2, 3});
  auto it = ++list.cbegin();
  maslov::FwdListIterator< int > tempIt = list.eraseAfter(it);
  BOOST_TEST(*(++tempIt) == 1);
  std::ostringstream out;
  printList(out, list);
  BOOST_TEST(out.str() == "12");

  list.assign({1, 2, 3, 4, 5});
  auto first = list.cbegin();
  auto last = list.cend();
  tempIt = list.eraseAfter(first, last);
  BOOST_TEST(*(++tempIt) == 1);
  out.str("");
  printList(out, list);
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_CASE(insertAfter)
{
  maslov::FwdList< int > list;
  auto it1 = list.insertAfter(list.cend(), 1);
  BOOST_TEST(*it1 == 1);
  it1 = list.insertAfter(list.cend(), 2);
  it1 = list.insertAfter(list.cbegin(), 3);
  BOOST_TEST(*it1 == 3);
  std::ostringstream out;
  printList(out, list);
  BOOST_TEST(out.str() == "231");

  auto it2 = list.insertAfter(list.cbegin(), 4, 4);
  BOOST_TEST(*it2 == 4);
  out.str("");
  printList(out, list);
  BOOST_TEST(out.str() == "2444431");

  maslov::FwdList< int > list1({5, 5, 6, 7});
  auto it3 = list.insertAfter(++list.cbegin(), list1.cbegin(), list1.cend());
  BOOST_TEST(*it3 == 7);
  out.str("");
  printList(out, list);
  BOOST_TEST(out.str() == "24556744431");

  auto it4 = list.insertAfter(list.cbegin(), {8, 9});
  BOOST_TEST(*it4 == 9);
  out.str("");
  printList(out, list);
  BOOST_TEST(out.str() == "2894556744431");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(operations)

BOOST_AUTO_TEST_CASE(reverse)
{
  maslov::FwdList< int > emptyList;
  emptyList.reverse();
  BOOST_TEST(emptyList.empty());
  maslov::FwdList< int > listOneElement;
  listOneElement.pushFront(1);
  listOneElement.reverse();
  BOOST_TEST(listOneElement.size() == 1);
  BOOST_TEST(listOneElement.front() == 1);
  maslov::FwdList< int > list({1, 2, 3});
  list.reverse();
  std::ostringstream out;
  printList(out, list);
  BOOST_TEST(out.str() == "321");
}

BOOST_AUTO_TEST_CASE(remove)
{
  maslov::FwdList< int > list({1, 2, 5, 3, 5, 4});
  list.remove(5);
  std::ostringstream out;
  printList(out, list);
  BOOST_TEST(out.str() == "1234");

}

BOOST_AUTO_TEST_CASE(removeIf)
{
  maslov::FwdList< int > list({2, 5, 3, 1});
  list.removeIf(lessThanThree);
  std::ostringstream out;
  printList(out, list);
  BOOST_TEST(out.str() == "53");
}

BOOST_AUTO_TEST_CASE(SpliceAfterEntireList)
{
  maslov::FwdList< int > list1({1, 2, 3});
  maslov::FwdList< int > list2({4, 5, 6});
  list1.spliceAfter(list1.cbegin(), list2);
  std::ostringstream out1;
  printList(out1, list1);
  std::ostringstream out2;
  printList(out2, list2);
  BOOST_TEST(out1.str() == "145623");
  BOOST_TEST(out2.str() == "");
}

BOOST_AUTO_TEST_CASE(SpliceAfterSingle)
{
  maslov::FwdList< int > list1({1, 2, 3});
  maslov::FwdList< int > list2({4, 5, 6});
  auto it = ++list2.cbegin();
  list1.spliceAfter(list1.cbegin(), list2, it);
  std::ostringstream out1;
  printList(out1, list1);
  std::ostringstream out2;
  printList(out2, list2);
  BOOST_TEST(out1.str() == "1623");
  BOOST_TEST(out2.str() == "45");
  BOOST_TEST(list2.size() == 2);
  BOOST_TEST(list1.size() == 4);
}
BOOST_AUTO_TEST_CASE(SpliceAfterElementRange)
{
  maslov::FwdList< int > list1({1, 2, 3});
  maslov::FwdList< int > list2({4, 5, 6, 7, 8});
  auto begin = list2.cbegin();
  auto last = list2.cbegin();
  for (size_t i = 0; i < 4; i++)
  {
    ++last;
  }
  list1.spliceAfter(list1.cbegin(), list2, begin, last);
  std::ostringstream out1;
  printList(out1, list1);
  std::ostringstream out2;
  printList(out2, list2);
  BOOST_TEST(out1.str() == "156723");
  BOOST_TEST(out2.str() == "48");
  BOOST_TEST(list2.size() == 2);
  BOOST_TEST(list1.size() == 6);
}

BOOST_AUTO_TEST_SUITE_END()
