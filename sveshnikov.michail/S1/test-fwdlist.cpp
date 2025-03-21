#include <boost/test/unit_test.hpp>
#include "fwdlist-ring.hpp"

BOOST_AUTO_TEST_SUITE(constructors)
BOOST_AUTO_TEST_CASE(default_constructor)
{
  sveshnikov::FwdList< int > list;
  BOOST_TEST(list.empty());
}

BOOST_AUTO_TEST_CASE(fill_constructor)
{
  sveshnikov::FwdList< int > list1(10, 5);
  BOOST_TEST(list1.getSize() == 10);
  sveshnikov::FwdList< int > list2(0, 5);
  BOOST_TEST(list2.getSize() == 0);
}

BOOST_AUTO_TEST_CASE(copy_constructor)
{
  sveshnikov::FwdList< int > list;
  sveshnikov::FwdList< int > list_copy = list;
  BOOST_TEST(list_copy.empty());
  list.push_back(1);
  sveshnikov::FwdList< int > list_copy2 = list;
  BOOST_TEST(list_copy2.front() == 1);
  list.push_back(2);
  sveshnikov::FwdList< int > list_copy3 = list;
  BOOST_TEST(list_copy3.getSize() == 2);
}

BOOST_AUTO_TEST_CASE(move_constructor)
{
  sveshnikov::FwdList< int > list;
  list.push_back(1);
  list.push_back(2);
  sveshnikov::FwdList< int > list_copy = std::move(list);
  BOOST_TEST(list_copy.getSize() == 2);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(interface)
BOOST_AUTO_TEST_CASE(iterators)
{
  sveshnikov::FwdList< int > list;
  list.push_back(1);
  list.push_back(2);
  BOOST_TEST(*list.begin() == 1);
  BOOST_TEST(*list.cbegin() == 1);

  BOOST_TEST(*list.end() == 1);
  BOOST_TEST(*list.cend() == 1);

  BOOST_TEST(*list.before_begin() == 2);
  BOOST_TEST(*list.cbefore_begin() == 2);
}

BOOST_AUTO_TEST_CASE(ellement_access)
{
  sveshnikov::FwdList< int > list;
  list.push_back(1);
  list.push_back(2);
  BOOST_TEST(list.front() == 1);
  BOOST_TEST(list.back() == 2);
}

BOOST_AUTO_TEST_CASE(capacity)
{
  sveshnikov::FwdList< int > list;
  BOOST_TEST(list.getSize() == 0);
  BOOST_TEST(list.empty());
  list.push_back(1);
  BOOST_TEST(list.getSize() == 1);
  BOOST_TEST(!list.empty());
}

BOOST_AUTO_TEST_CASE(push)
{
  sveshnikov::FwdList< int > list;
  list.push_back(1);
  BOOST_TEST(list.getSize() == 1);
  BOOST_TEST(list.front() == 1);
  BOOST_TEST(list.back() == 1);
  list.push_front(2);
  list.push_back(3);
  BOOST_TEST(list.front() == 2);
  BOOST_TEST(*(++list.begin()) == 1);
  BOOST_TEST(list.back() == 3);

  int a = 4, b = 5;
  list.push_front(a);
  list.push_back(b);
  BOOST_TEST(list.front() == a);
  BOOST_TEST(list.back() == b);
}

BOOST_AUTO_TEST_CASE(pop)
{
  sveshnikov::FwdList< int > list;
  list.push_back(1);
  list.pop_back();
  BOOST_TEST(list.empty());
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  list.pop_back();
  list.pop_front();
  BOOST_TEST(list.front() == 2);
  BOOST_TEST(list.back() == 2);
}

BOOST_AUTO_TEST_CASE(swap)
{
  sveshnikov::FwdList< int > list1;
  list1.push_back(1);
  sveshnikov::FwdList< int > list2;
  list2.push_back(2);
  list1.swap(list2);
  BOOST_TEST(list1.front() == 2);
  BOOST_TEST(list2.front() == 1);
}

BOOST_AUTO_TEST_CASE(clear)
{
  sveshnikov::FwdList< int > list;
  list.push_back(1);
  list.push_back(2);
  list.clear();
  BOOST_TEST(list.empty());
}

BOOST_AUTO_TEST_CASE(remove)
{
  sveshnikov::FwdList< int > list;
  list.push_back(1);
  list.remove(1);
  BOOST_TEST(list.empty());
  list.remove(1);
  BOOST_TEST(list.empty());
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  list.remove(1);
  BOOST_TEST(list.front() == 2);
  list.remove(3);
  BOOST_TEST(list.back() == 2);
  list.remove(1);
  BOOST_TEST(list.back() == 2);
  sveshnikov::FwdList< int > list2(5, 6);
  list2.push_back(1);
  list2.push_back(2);
  list2.remove(6);
  BOOST_TEST(list2.getSize() == 2);
}

BOOST_AUTO_TEST_SUITE(splice)
BOOST_AUTO_TEST_CASE(splice_array)
{
  sveshnikov::FwdList< int > list1;
  sveshnikov::FwdList< int > list2;
  list1.splice(list1.cbegin(), list2);

  sveshnikov::FwdList< int > list3(3, 1);
  list1.splice(list1.cbegin(), list3);
  BOOST_TEST(list3.empty());
  BOOST_TEST(list1.getSize() == 3);
  BOOST_TEST(list1.front() == 1);
  BOOST_TEST(list1.back() == 1);

  list2.push_back(2);
  list1.splice(list1.cbegin(), list2);
  BOOST_TEST(list1.getSize() == 4);
  BOOST_TEST(*(++list1.cbegin()) == 2);

  list2.push_back(2);
  list2.push_back(3);
  list1.splice(list1.cbefore_begin(), list2);
  BOOST_TEST(list1.getSize() == 6);
  BOOST_TEST(list1.back() == 3);
}

BOOST_AUTO_TEST_CASE(splice_element)
{
  sveshnikov::FwdList< int > list1;
  sveshnikov::FwdList< int > list2;

  list2.push_back(1);
  list1.splice(list1.cbegin(), list2, list2.cbegin());
  BOOST_TEST(list1.getSize() == 1);
  BOOST_TEST(list1.front() == 1);
  BOOST_TEST(list2.empty());

  list2.push_back(3);
  list2.push_back(2);
  list2.push_back(1);
  list1.splice(list1.cbegin(), list2, list2.cbegin());
  BOOST_TEST(list2.getSize() == 2);
  BOOST_TEST(list2.front() == 2);
  BOOST_TEST(list1.getSize() == 2);
  BOOST_TEST(*(++list1.cbegin()) == 3);
  list1.splice(list1.cbegin(), list2, list2.cbefore_begin());
  BOOST_TEST(list2.back() == 2);

  list2.push_back(1);
  list1.splice(list1.cbefore_begin(), list2, list2.cbefore_begin());
  BOOST_TEST(list1.back() == 1);
}

BOOST_AUTO_TEST_CASE(splice_segment)
{
  sveshnikov::FwdList< int > list1;
  sveshnikov::FwdList< int > list2;
  list2.push_back(1);
  list2.push_back(2);
  list2.push_back(3);
  list2.push_back(4);
  list2.push_back(5);
  sveshnikov::ConstIterator< int > it = ++list2.cbegin();
  sveshnikov::ConstIterator< int > prev = list2.cbegin();
  sveshnikov::ConstIterator< int > prev_prev = list2.cbefore_begin();
  while (it != list2.cbefore_begin())
  {
    it++;
    prev++;
    prev_prev++;
  }
  list1.splice(list1.cbegin(), list2, prev, prev_prev);
  BOOST_TEST(list2.getSize() == 2);
  BOOST_TEST(list2.front() == 3);
  BOOST_TEST(list2.back() == 4);
  BOOST_TEST(list1.getSize() == 3);
  BOOST_TEST(list1.front() == 5);
  BOOST_TEST(list1.back() == 2);

  list2.push_front(2);
  list2.push_front(1);
  list2.push_back(5);
  list1.splice(list1.cbegin(), list2, prev, prev_prev);
  BOOST_TEST(list1.getSize() == 6);
  BOOST_TEST(list1.back() == 2);

  list2.push_front(2);
  list2.push_front(1);
  list2.push_back(5);
  list1.clear();
  list1.push_back(1);
  list1.splice(list1.cbegin(), list2, prev, prev_prev);
  BOOST_TEST(list1.getSize() == 4);
  BOOST_TEST(list1.back() == 2);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_CASE(assign)
{
  sveshnikov::FwdList< int > list1;
  list1.assign(0, 2);
  BOOST_TEST(list1.empty());
  list1.assign(5, 4);
  BOOST_TEST(list1.getSize() == 5);
  BOOST_TEST(list1.front() == 4);
  BOOST_TEST(list1.back() == 4);
}
BOOST_AUTO_TEST_SUITE_END()
