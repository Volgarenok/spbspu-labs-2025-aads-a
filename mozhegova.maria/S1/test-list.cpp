#include <boost/test/unit_test.hpp>
#include <sstream>
#include "list.hpp"

namespace
{
  template< typename T >
  std::ostream & printList(std::ostream & out, mozhegova::List< T > list)
  {
    for (auto it = list.begin(); it != list.end(); it++)
    {
      out << *it;
    }
    return out;
  }
}

BOOST_AUTO_TEST_CASE(constructors)
{
  mozhegova::List< int > list1;
  BOOST_TEST(list1.empty());

  mozhegova::List< int > list2(3, 5);
  std::ostringstream out1;
  printList(out1, list2);
  BOOST_TEST(out1.str() == "555");

  mozhegova::List< int > list3({1, 2, 3});
  std::ostringstream out2;
  printList(out2, list3);
  BOOST_TEST(out2.str() == "123");

  mozhegova::List< int > list4(list3.cbegin(), list3.cend());
  std::ostringstream out3;
  printList(out3, list4);
  BOOST_TEST(out3.str() == "123");

  mozhegova::List< int > list5(list2);
  std::ostringstream out4;
  printList(out4, list5);
  BOOST_TEST(out4.str() == "555");
}

BOOST_AUTO_TEST_CASE(capacity)
{
  mozhegova::List< int > list;
  BOOST_TEST(list.empty());
  BOOST_TEST(list.size() == 0);
  list.push_back(1);
  BOOST_TEST(!list.empty());
  list.clear();
  BOOST_TEST(list.empty());
}

BOOST_AUTO_TEST_CASE(front_back)
{
  mozhegova::List< int > list({1, 2, 3});
  BOOST_TEST(list.front() == 1);
  BOOST_TEST(list.back() == 3);
}

BOOST_AUTO_TEST_CASE(push_pop)
{
  mozhegova::List< int > list;
  list.push_front(1);
  BOOST_TEST(list.front() == 1);
  list.push_front(2);
  BOOST_TEST(list.front() == 2);
  list.pop_front();
  BOOST_TEST(list.front() == 1);
  list.pop_back();
  BOOST_TEST(list.empty());
  list.push_back(1);
  BOOST_TEST(list.back() == 1);
  list.push_back(2);
  BOOST_TEST(list.back() == 2);
}

BOOST_AUTO_TEST_CASE(swap)
{
  mozhegova::List< int > list1({1, 2, 3});
  mozhegova::List< int > list2({10, 20, 30});
  list1.swap(list2);
  std::ostringstream out1;
  printList(out1, list1);
  BOOST_TEST(out1.str() == "102030");
  std::ostringstream out2;
  printList(out2, list2);
  BOOST_TEST(out2.str() == "123");
}

BOOST_AUTO_TEST_CASE(iterators)
{
  mozhegova::List< int > list({1, 2, 3});
  BOOST_TEST(*(list.begin()) == 1);
  BOOST_TEST(*(list.cbegin()) == 1);
}

namespace
{
  bool isEven(int a)
  {
    return a % 2 == 0;
  }
}

BOOST_AUTO_TEST_CASE(remove_and_remove_if)
{
  mozhegova::List< int > list({1, 2, 3, 2, 4});
  list.remove(2);
  std::ostringstream out1;
  printList(out1, list);
  BOOST_TEST(out1.str() == "134");
  list.removeIf(isEven);
  std::ostringstream out2;
  printList(out2, list);
  BOOST_TEST(out2.str() == "13");
}

BOOST_AUTO_TEST_CASE(splice)
{
  mozhegova::List< int > list1({1, 2, 3, 4});
  mozhegova::List< int > list2({10, 20, 30});
  auto it = list1.cbegin();
  ++it;
  list1.splice(it, list2);
  std::ostringstream out1;
  printList(out1, list1);
  BOOST_TEST(out1.str() == "1102030234");
  BOOST_TEST(list2.empty());
  mozhegova::List< int > list3({1, 2});
  list3.splice(list3.cbegin(), list1, it);
  std::ostringstream out2;
  printList(out2, list1);
  BOOST_TEST(out2.str() == "110203034");
  BOOST_TEST(list3.front() == 2);
  it = list3.cbegin();
  ++it;
  list1.splice(list1.cbegin(), list3, it, list3.cend());
  std::ostringstream out3;
  printList(out3, list1);
  BOOST_TEST(out3.str() == "12110203034");
  BOOST_TEST(list3.front() == 2);
}

BOOST_AUTO_TEST_CASE(assign)
{
  mozhegova::List< int > list({1, 2, 3});
  list.assign(3, 5);
  std::ostringstream out1;
  printList(out1, list);
  BOOST_TEST(out1.str() == "555");
  mozhegova::List< int > list2({1, 2, 3, 4, 5, 6});
  auto it = list2.cbegin();
  std::advance(it, 3);
  list.assign(it, list2.cend());
  std::ostringstream out2;
  printList(out2, list);
  BOOST_TEST(out2.str() == "456");
  list.assign({1, 1, 1});
  std::ostringstream out3;
  printList(out3, list);
  BOOST_TEST(out3.str() == "111");
}

BOOST_AUTO_TEST_CASE(insert)
{
  mozhegova::List< int > list;
  list.insert(list.cbegin(), 2);
  BOOST_TEST(list.front() == 2);
  list.insert(list.cbegin(), 5);
  std::ostringstream out1;
  printList(out1, list);
  BOOST_TEST(out1.str() == "52");

  size_t n = 0;
  auto it = list.cbegin();
  ++it;
  list.insert(it, n, 5);
  std::ostringstream out2;
  printList(out2, list);
  BOOST_TEST(out2.str() == "52");
  n = 3;
  list.insert(it, n, 9);
  std::ostringstream out3;
  printList(out3, list);
  BOOST_TEST(out3.str() == "59992");

  mozhegova::List< int > list2({1, 2, 3, 4, 5, 6});
  it = list.cbegin();
  std::advance(it, 3);
  mozhegova::ConstIterator< int > it2 = list2.cbegin();
  list.insert(it, it2, list2.cbegin());
  std::ostringstream out4;
  printList(out4, list);
  BOOST_TEST(out4.str() == "59992");
  std::advance(it2, 3);
  list.insert(it, it2, list2.cend());
  std::ostringstream out5;
  printList(out5, list);
  BOOST_TEST(out5.str() == "59945692");

  --it;
  list.insert(it, {1, 1, 1});
  std::ostringstream out6;
  printList(out6, list);
  BOOST_TEST(out6.str() == "59945111692");
}

BOOST_AUTO_TEST_CASE(erase)
{
  mozhegova::List< int > list;
  list.push_back(1);
  list.erase(list.cbegin());
  BOOST_TEST(list.empty());
  list.assign({1, 2, 3, 4, 5});
  list.erase(list.cbegin());
  BOOST_TEST(list.front() == 2);
  auto it = list.cbegin();
  ++it;
  list.erase(it);
  std::ostringstream out1;
  printList(out1, list);
  BOOST_TEST(out1.str() == "245");
  it = list.cbegin();
  std::advance(it, 2);
  list.erase(it);
  std::ostringstream out2;
  printList(out2, list);
  BOOST_TEST(out2.str() == "24");

  list.assign({1, 2, 3, 4, 5});
  it = list.cbegin();
  std::advance(it, 3);
  list.erase(it, list.cend());
  std::ostringstream out3;
  printList(out3, list);
  BOOST_TEST(out3.str() == "123");
}
