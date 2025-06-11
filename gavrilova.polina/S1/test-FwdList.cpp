#include <boost/test/unit_test.hpp>
#include "ConstIteratorFwd.hpp"
#include "FwdList.hpp"
#include "IteratorFwd.hpp"

BOOST_AUTO_TEST_CASE(test_default_constructor)
{
  gavrilova::FwdList< int > list;
  BOOST_TEST(list.empty());
  BOOST_TEST(list.size() == 0);
}

BOOST_AUTO_TEST_CASE(test_initializer_list_constructor)
{
  gavrilova::FwdList< int > list{1, 2, 3};
  BOOST_TEST(list.size() == 3);
  BOOST_TEST(list.front() == 1);
}

BOOST_AUTO_TEST_CASE(test_copy_constructor)
{
  gavrilova::FwdList< int > original{1, 2, 3};
  gavrilova::FwdList< int > copy(original);
  BOOST_TEST(original == copy);
}

BOOST_AUTO_TEST_CASE(test_move_constructor)
{
  gavrilova::FwdList< int > temp{1, 2, 3};
  gavrilova::FwdList< int > moved(std::move(temp));
  BOOST_TEST(moved.size() == 3);
  BOOST_TEST(temp.empty());
}

BOOST_AUTO_TEST_CASE(test_front)
{
  gavrilova::FwdList< int > list{42};
  BOOST_TEST(list.front() == 42);
  BOOST_CHECK_THROW(gavrilova::FwdList< int >{}.front(), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_push_pop_front)
{
  gavrilova::FwdList< int > list;
  list.push_front(1);
  list.push_front(2);
  BOOST_TEST(list.front() == 2);
  list.pop_front();
  BOOST_TEST(list.front() == 1);
}

BOOST_AUTO_TEST_CASE(test_clear)
{
  gavrilova::FwdList< int > list{1, 2, 3};
  list.clear();
  BOOST_TEST(list.empty());
}

BOOST_AUTO_TEST_CASE(test_iterators)
{
  gavrilova::FwdList< int > list{1, 2, 3};
  auto it = list.begin();
  BOOST_TEST(*it == 1);
  ++it;
  BOOST_TEST(*it == 2);
  BOOST_TEST((it != list.end()));
}

BOOST_AUTO_TEST_CASE(test_remove)
{
  gavrilova::FwdList< int > list{1, 2, 3, 2};
  list.remove(2);
  BOOST_TEST(list.size() == 2);
  BOOST_TEST(*list.begin() == 1);
}

BOOST_AUTO_TEST_CASE(test_remove_if)
{
  gavrilova::FwdList< int > list{1, 2, 3, 4, 5};
  list.remove_if([](int x)
  {
    return x % 2 == 0;
  });
  BOOST_TEST(list.size() == 3);
}

BOOST_AUTO_TEST_CASE(test_reverse)
{
  gavrilova::FwdList< int > list{1, 2, 3};
  list.reverse();
  auto it = list.begin();
  BOOST_TEST(*it == 3);
  ++it;
  BOOST_TEST(*it == 2);
  ++it;
  BOOST_TEST(*it == 1);
}

BOOST_AUTO_TEST_CASE(test_copy_assignment_operator)
{
  gavrilova::FwdList< int > list1{1, 2, 3};
  gavrilova::FwdList< int > list2;
  list2 = list1;

  BOOST_TEST(list1 == list2);

  list2.pop_front();
  BOOST_TEST(list2.front() == 2);
  BOOST_TEST(list1.front() == 1);
}

BOOST_AUTO_TEST_CASE(test_self_assignment)
{
  gavrilova::FwdList< int > list{1, 2, 3};
  list = list;
  BOOST_TEST(list.size() == 3);
  BOOST_TEST(list.front() == 1);
}

BOOST_AUTO_TEST_CASE(test_move_assignment)
{
  gavrilova::FwdList< int > src{4, 5, 6};
  gavrilova::FwdList< int > dst;
  dst = std::move(src);

  BOOST_TEST(dst.size() == 3);
  BOOST_TEST(dst.front() == 4);
  BOOST_TEST(src.empty());
}

BOOST_AUTO_TEST_CASE(test_comparison_operators)
{
  gavrilova::FwdList< int > list1{1, 2};
  gavrilova::FwdList< int > list2{1, 2, 3};
  BOOST_TEST(list1 < list2);
  BOOST_TEST(list1 != list2);
}

BOOST_AUTO_TEST_CASE(test_insert)
{
  gavrilova::FwdList< int > list{1, 3};
  auto it = list.begin();
  ++it;
  list.insert(it, 2);
  BOOST_TEST(list.size() == 3);
  BOOST_TEST(*++list.begin() == 3);
  BOOST_TEST(*it == 3);
  BOOST_TEST(*++it == 2);
}

BOOST_AUTO_TEST_CASE(test_erase)
{
  gavrilova::FwdList< int > list{1, 2, 3};
  auto it = list.begin();
  list.erase(it);
  BOOST_TEST(list.size() == 2);
  BOOST_TEST(*++list.begin() == 3);
}

BOOST_AUTO_TEST_CASE(test_splice)
{
  gavrilova::FwdList< int > list1{1, 2};
  gavrilova::FwdList< int > list2{3, 4, 5};
  auto pos = list1.begin();
  list1.splice(pos, list2);
  BOOST_TEST(list1.size() == 5);
  BOOST_TEST(list2.empty());
}
BOOST_AUTO_TEST_CASE(test_swap)
{
  gavrilova::FwdList< int > list1{1, 2};
  gavrilova::FwdList< int > list2{3, 4, 5};
  list1.swap(list2);
  BOOST_TEST(list1.size() == 3);
  BOOST_TEST(list2.size() == 2);
}
