#include <boost/test/unit_test.hpp>
#include <sstream>
#include <list/list.hpp>

namespace
{
  template< typename T >
  std::ostream& printList(std::ostream& out, demehin::List< T > lst)
  {
    for (auto it = lst.begin(); it != lst.end(); it++)
    {
      out << *it;
    }
    return out;
  }
}

BOOST_AUTO_TEST_CASE(list_construct_test)
{
  demehin::List< int > list;
  demehin::List< int > fill_list(5ull, 5);
  demehin::List< int > mov_list(std::move(list));
  demehin::List< int > cop_list(mov_list);
  std::initializer_list< int > ilist;
  demehin::List< int > init_list(ilist);
  demehin::List< int > range_list(fill_list.begin(), fill_list.end());
}

BOOST_AUTO_TEST_CASE(list_iter_test)
{
  demehin::List< int > list;
  list.push_back(5);
  list.push_back(4);
  list.push_back(5);
  BOOST_TEST(*(++list.cbegin()) == 4);
  std::ostringstream out;
  printList(out, list);
  BOOST_TEST(out.str() == "545");
}

BOOST_AUTO_TEST_CASE(list_el_access_test)
{
  demehin::List< int > list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  BOOST_TEST(list.front() == 1);
  BOOST_TEST(list.back() == 3);
}

BOOST_AUTO_TEST_CASE(list_capac_test)
{
  demehin::List< int > list;
  BOOST_TEST(list.empty());
  list.assign(5ull, 5);
  BOOST_TEST(list.size() == 5);
}

BOOST_AUTO_TEST_CASE(list_mods_test)
{
  demehin::List< int > list(3ull, 5);
  demehin::List< int > list2(3ull, 4);
  list.push_front(4);
  list.push_back(4);
  std::ostringstream out;
  list.swap(list2);
  printList(out, list2);
  BOOST_TEST(out.str() == "45554");
  list2.pop_back();
  list2.pop_front();
  std::ostringstream out1;
  printList(out1, list2);
  BOOST_TEST(out1.str() == "555");

  list2.clear();
  BOOST_TEST(list2.empty());

  list2.assign(3ull, 5);
  std::ostringstream out2;
  printList(out2, list2);
  BOOST_TEST(out2.str() == "555");

  std::ostringstream out3;
  list2.erase(list2.cbegin());
  printList(out3, list2);
  BOOST_TEST(out3.str() == "55");
  list2.erase(list2.cbegin(), list2.cend());
  BOOST_TEST(list2.empty());

  list2.push_back(1);
  list2.assign(list.begin(), list.end());
  std::ostringstream out4;
  printList(out4, list2);
  BOOST_TEST(out4.str() == "444");

  std::initializer_list< int > ilist;
  list2.assign(ilist);
  BOOST_TEST(list2.empty());

  list2.insert(list2.cbegin(), 5);
  BOOST_TEST(list2.front() == 5);

  list2.insert(list2.cbegin(), 3ull, 5);
  std::ostringstream out5;
  printList(out5, list2);
  BOOST_TEST(out5.str() == "5555");

  list2.insert(list2.cbegin(), list.begin(), list.end());
  std::ostringstream out6;
  printList(out6, list2);
  BOOST_TEST(out6.str() == "4445555");

  list2.insert(list2.cbegin(), ilist);
  std::ostringstream out7;
  printList(out7, list2);
  BOOST_TEST(out7.str() == "4445555");

  demehin::List< int > list3;
  for (int i = 0; i < 4; i++)
  {
    list3.push_back(i);
  }
  list3.reverse();
  std::ostringstream out8;
  printList(out8, list3);
  BOOST_TEST(out8.str() == "3210");
}

BOOST_AUTO_TEST_CASE(list_operations_test)
{
  demehin::List< int > lst1(3ull, 5);
  demehin::List< int > lst2(3ull, 4);
  lst2.splice(++lst2.cbegin(), lst1);
  std::ostringstream out;
  printList(out, lst2);
  BOOST_TEST(out.str() == "455544");
  BOOST_TEST(lst1.empty());

  demehin::List< int > lst3(1ull, 3);
  lst3.push_back(2);
  lst2.splice(lst2.cbegin(), lst3, ++lst3.cbegin());
  std::ostringstream out1;
  printList(out1, lst2);
  BOOST_TEST(out1.str() == "2455544");

  demehin::List< int > lst4(3ull, 1);
  lst4.push_front(2);
  lst2.splice(++lst2.cbegin(), lst4, lst4.cbegin(), ++(++lst4.cbegin()));
  std::ostringstream out2;
  printList(out2, lst2);
  BOOST_TEST(out2.str() == "221455544");

  std::ostringstream out3;
  lst2.remove(5);
  printList(out3, lst2);
  BOOST_TEST(out3.str() == "221444");

  lst2.removeIf([](int n){ return n < 4; });
  std::ostringstream out4;
  printList(out4, lst2);
  BOOST_TEST(out4.str() == "444");

}

BOOST_AUTO_TEST_CASE(list_operators_test)
{
  demehin::List< int > lst1(3ull, 5);
  demehin::List< int > lst2(3ull, 5);
  BOOST_TEST(lst1 == lst2);

  lst1.push_front(1);
  BOOST_TEST(lst1 != lst2);
  BOOST_TEST(lst1 < lst2);

  lst1.pop_front();
  lst2.push_back(6);
  BOOST_TEST(lst1 <= lst2);
  demehin::List< int > lst3(lst2);
  BOOST_TEST(lst3 <= lst2);

  lst1.push_front(10);
  BOOST_TEST(lst1 > lst2);

  BOOST_TEST(lst3 >= lst2);
  BOOST_TEST(lst1 >= lst3);
}

BOOST_AUTO_TEST_CASE(list_test)
{
  demehin::List< int > list1(5ull, 5);
  BOOST_TEST(*(++list1.begin()) == 5);
  list1.push_front(6);
  BOOST_TEST(list1.front() == 6);
  demehin::List< int > mlist(std::move(list1));

  demehin::List< int > list;
  list.push_back(4);
  list.push_back(5);
  list.push_front(5);
  std::ostringstream out;
  printList(out, list);
  BOOST_TEST(out.str() == "545");
  list.pop_back();
  list.pop_front();
  list.pop_back();
  BOOST_TEST(list.empty());
  BOOST_TEST(list.size() == 0);

  mlist.clear();
  BOOST_TEST(mlist.empty());
}
