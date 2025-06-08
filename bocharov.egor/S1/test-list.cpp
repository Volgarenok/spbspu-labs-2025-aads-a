#include <boost/test/unit_test.hpp>
#include "list.hpp"

namespace {
  template< typename T >
  void compare_list_values(const bocharov::List< T > & first, const bocharov::List< T > & second) noexcept
  {
    if (first.empty() && second.empty())
    {
      return;
    }
    BOOST_TEST(first.size() == second.size());
    BOOST_TEST(first.front() == second.front());
    auto it1 = ++first.cbegin(), it2 = ++second.cbegin();
    for (; it1 != first.cend() && it2 != second.cend(); ++it1, ++it2)
    {
      BOOST_TEST(*it1 == *it2);
    }
  }
}

BOOST_AUTO_TEST_SUITE(constructors_tests)
BOOST_AUTO_TEST_CASE(default_constructor_test)
{
  bocharov::List< int > list;
  BOOST_TEST(list.empty());
}

BOOST_AUTO_TEST_CASE(copy_constructor_test)
{
  bocharov::List< int > list1;
  bocharov::List< int > list2(list1);
  BOOST_TEST(list2.empty());
  list1.push_front(0);
  bocharov::List< int > list3(list1);
  BOOST_TEST(list3.front() == 0);
  BOOST_TEST(list3.size() == 1);
  list1.push_front(1);
  bocharov::List< int > list4(list1);
  BOOST_TEST(list4.front() == 1);
  BOOST_TEST(list4.back() == 0);
  BOOST_TEST(list4.size() == 2);
}

BOOST_AUTO_TEST_CASE(move_constructor_test)
{
  bocharov::List< int > list1 = {0, 1};
  bocharov::List< int > list2(std::move(list1));
  BOOST_TEST(list2.front() == 0);
  BOOST_TEST(list2.back() == 1);
  BOOST_TEST(list2.size() == 2);
  BOOST_TEST(list1.empty());
}

BOOST_AUTO_TEST_CASE(fill_constructor_test)
{
  bocharov::List< char > list1(0, 'r');
  BOOST_TEST(list1.empty());
  bocharov::List< char > list2(2, 'r');
  BOOST_TEST(list2.front() == 'r');
  BOOST_TEST(list2.back() == 'r');
  BOOST_TEST(list2.size() == 2);
}

BOOST_AUTO_TEST_CASE(range_constructor_test)
{
  bocharov::List< char > list1 = {'k', 'e', 'k'};
  bocharov::List< int > list2(list1.begin(), list1.begin());
  BOOST_TEST(list2.empty());
  bocharov::List< int > list3(++list1.begin(), list1.end());
  BOOST_TEST(list3.front() == 'e');
  BOOST_TEST(list3.back() == 'k');
  BOOST_TEST(list3.size() == 2);
}

BOOST_AUTO_TEST_CASE(initializer_list_constructor_test)
{
  bocharov::List< int > list = {0, 1};
  BOOST_TEST(list.front() == 0);
  BOOST_TEST(list.back() == 1);
  BOOST_TEST(list.size() == 2);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(assignment_operators_tests)
BOOST_AUTO_TEST_CASE(copy_assignment_operator_test)
{
  bocharov::List< int > list1 = {0, 1};
  bocharov::List< int > list2;
  list2 = list1;
  BOOST_TEST(list2.front() == 0);
  BOOST_TEST(list2.back() == 1);
  BOOST_TEST(list2.size() == 2);
}

BOOST_AUTO_TEST_CASE(move_assignment_operator_test)
{
  bocharov::List< int > list1 = {0, 1};
  bocharov::List< int > list2;
  list2 = std::move(list1);
  BOOST_TEST(list2.front() == 0);
  BOOST_TEST(list2.back() == 1);
  BOOST_TEST(list2.size() == 2);
  BOOST_TEST(list1.empty());
}

BOOST_AUTO_TEST_CASE(initializer_list_assignment_operator_test)
{
  bocharov::List< int > list;
  list = {0, 1};
  BOOST_TEST(list.front() == 0);
  BOOST_TEST(list.back() == 1);
  BOOST_TEST(list.size() == 2);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(assign_tests)
BOOST_AUTO_TEST_CASE(fill_assign_test)
{
  bocharov::List< char > list;
  list.assign(2, 'r');
  BOOST_TEST(list.front() == 'r');
  BOOST_TEST(list.back() == 'r');
  BOOST_TEST(list.size() == 2);
}

BOOST_AUTO_TEST_CASE(range_assign_test)
{
  bocharov::List< char > list1 = {'u', 'w', 'u'};
  bocharov::List< char > list2;
  list2.assign(++list1.begin(), list1.end());
  BOOST_TEST(list2.front() == 'w');
  BOOST_TEST(list2.back() == 'u');
  BOOST_TEST(list2.size() == 2);
}

BOOST_AUTO_TEST_CASE(initializer_list_assign_test)
{
  bocharov::List< char > list;
  list.assign({'r', 'r'});
  BOOST_TEST(list.front() == 'r');
  BOOST_TEST(list.back() == 'r');
  BOOST_TEST(list.size() == 2);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(element_access_tests)
BOOST_AUTO_TEST_CASE(front_test)
{
  bocharov::List< int > list = {0, 1};
  BOOST_TEST(list.front() == 0);
}

BOOST_AUTO_TEST_CASE(back_test)
{
  bocharov::List< int > list = {0, 1};
  BOOST_TEST(list.back() == 1);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(iterators_tests)
BOOST_AUTO_TEST_CASE(begin_test)
{
  bocharov::List< int > list = {0, 1};
  auto it = list.begin();
  auto c_it = list.cbegin();
  BOOST_TEST(*it == 0);
  BOOST_TEST(*c_it == 0);
}

BOOST_AUTO_TEST_CASE(end_test)
{
  bocharov::List< int > list = {0, 1};
  auto it = list.end();
  auto c_it = list.cend();
  BOOST_TEST(*it == 0);
  BOOST_TEST(*c_it == 0);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(capacity_tests)
BOOST_AUTO_TEST_CASE(size_test)
{
  bocharov::List< int > list = {0, 1};
  BOOST_TEST(list.size() == 2);
}

BOOST_AUTO_TEST_CASE(empty_test)
{
  bocharov::List< int > list;
  BOOST_TEST(list.empty());
  list.push_front(0);
  BOOST_TEST(!list.empty());
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(modifiers_tests)
BOOST_AUTO_TEST_CASE(clear_test)
{
  bocharov::List< int > list;
  list.clear();
  BOOST_TEST(list.empty());
  list.push_front(0);
  list.clear();
  BOOST_TEST(list.empty());
}

BOOST_AUTO_TEST_CASE(insert_after_test)
{
  bocharov::List< char > list1 = {'0', '1'};
  auto it = list1.cbegin();
  list1.insert_after(it, '2');
  ++(++it);
  list1.insert_after(it, '3');
  compare_list_values(list1, {'0', '2', '1', '3'});

  list1.insert_after(list1.cbegin(), 2, '7');
  compare_list_values(list1, {'0', '7', '7', '2', '1', '3'});

  bocharov::List< char > list2 = {'0', '1', '2'};
  list1.insert_after(list1.cbegin(), ++list2.cbegin(), list2.cend());
  compare_list_values(list1, {'0', '1', '2', '7', '7', '2', '1', '3'});

  list2.insert_after(list2.cbegin(), {'1', '2', '3'});
  compare_list_values(list2, {'0', '1', '2', '3', '1', '2'});
}

BOOST_AUTO_TEST_CASE(erase_after_test)
{
  bocharov::List< int > list1 = {0, 1, 2};
  list1.erase_after(list1.cbegin());
  compare_list_values(list1, {0, 2});
  list1.erase_after(list1.cbegin());
  compare_list_values(list1, {0});
  list1.erase_after(list1.cbegin());
  BOOST_TEST(list1.empty());

  bocharov::List< int > list2 = {0, 1, 2, 3, 4, 5};
  list2.erase_after(++list2.cbegin(), list2.cend());
  compare_list_values(list2, {0, 1});
}

BOOST_AUTO_TEST_CASE(push_front_test)
{
  bocharov::List< int > list;
  list.push_front(0);
  list.push_front(1);
  BOOST_TEST(list.front() == 1);
  BOOST_TEST(list.back() == 0);
  BOOST_TEST(list.size() == 2);
}

BOOST_AUTO_TEST_CASE(push_back_test)
{
  bocharov::List< int > list;
  list.push_back(0);
  list.push_back(1);
  BOOST_TEST(list.front() == 0);
  BOOST_TEST(list.back() == 1);
  BOOST_TEST(list.size() == 2);
}

BOOST_AUTO_TEST_CASE(pop_front_test)
{
  bocharov::List< int > list = {0, 1};
  list.pop_front();
  BOOST_TEST(list.front() == 1);
  BOOST_TEST(list.size() == 1);
  list.pop_front();
  BOOST_TEST(list.empty());
}

BOOST_AUTO_TEST_CASE(pop_back_test)
{
  bocharov::List<int> list = {1, 2};
  list.pop_back();
  BOOST_TEST(list.back() == 1);
  list.pop_back();
  BOOST_TEST(list.empty());
}

BOOST_AUTO_TEST_CASE(swap_test)
{
  bocharov::List< int > list1 = {0};
  bocharov::List< int > list2 = {1, 2};
  list1.swap(list2);
  BOOST_TEST(list1.front() == 1);
  BOOST_TEST(list1.back() == 2);
  BOOST_TEST(list1.size() == 2);
  BOOST_TEST(list2.front() == 0);
  BOOST_TEST(list2.size() == 1);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(operations_tests)
BOOST_AUTO_TEST_CASE(splice_after_test)
{
  bocharov::List< int > list1 = {1, 2, 3, 4, 5};
  bocharov::List< int > list2 = {10, 11, 12};
  list1.splice_after(list1.cbegin(), list2);
  compare_list_values(list1, {1, 10, 11, 12, 2, 3, 4, 5});
  compare_list_values(list2, {});

  list1 = {1, 2, 3, 4, 5};
  list2 = {10, 11, 12};
  list1.splice_after(list1.cbegin(), list2, list2.cbegin());
  compare_list_values(list1, {1, 11, 2, 3, 4, 5});
  compare_list_values(list2, {10, 12});

  list1 = {1, 2, 3, 4, 5};
  list2 = {10, 11, 12};
  list2.splice_after(list2.cbegin(), list1, list1.cbegin(), list1.cend());
  compare_list_values(list1, {1});
  compare_list_values(list2, {10, 2, 3, 4, 5, 11, 12});
  list1.splice_after(list1.cbegin(), list2, list2.cbegin(), ++list2.cbegin());
  compare_list_values(list1, {1});
  compare_list_values(list2, {10, 2, 3, 4, 5, 11, 12});
}

BOOST_AUTO_TEST_CASE(remove_test)
{
  bocharov::List< char > list = {'_', 'k', 'e', 'k', '_', 'w', '_'};
  list.remove('_');
  compare_list_values(list, {'k', 'e', 'k', 'w'});
}

BOOST_AUTO_TEST_CASE(remove_if_test)
{
  bocharov::List< int > list = {1, 2, 3, 4, 5, 6};
  list.remove_if([&](const int& list_value)
  {
    return list_value % 2 == 0;
  });
  compare_list_values(list, {1, 3, 5});
}
BOOST_AUTO_TEST_SUITE_END()

