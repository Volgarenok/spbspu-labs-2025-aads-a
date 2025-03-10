#include <boost/test/unit_test_suite.hpp>
#include "test-utils.hpp"

using kizhin::ListT;

BOOST_AUTO_TEST_SUITE(constructors);

BOOST_AUTO_TEST_CASE(default_constructor)
{
  const ListT list;
  testForwardListInvariants(list);
  BOOST_TEST(list.empty());
}

BOOST_AUTO_TEST_CASE(copy_constructor_from_empty)
{
  const ListT list;
  const ListT copied(list);
  testForwardListInvariants(copied);
  BOOST_TEST(copied.empty());
}

BOOST_AUTO_TEST_CASE(copy_constructor)
{
  const ListT list{ 1, 2, 3, 4, 5 };
  const ListT copied(list);
  testForwardListInvariants(copied);
  BOOST_TEST(copied == list);
}

BOOST_AUTO_TEST_CASE(move_constructor)
{
  const ListT list{ 1, 2, 3, 4, 5 };
  const ListT copied(list);
  const ListT moved(std::move(list));
  testForwardListInvariants(moved);
  BOOST_TEST(moved == copied);
}

BOOST_AUTO_TEST_CASE(empty_fill_constructor)
{
  const ListT list(0, 10);
  testForwardListInvariants(list);
  BOOST_TEST(list.empty());
}

BOOST_AUTO_TEST_CASE(fill_constructor)
{
  constexpr ListT::size_type size = 100;
  constexpr ListT::value_type value = 10;
  const ListT list(size, value);
  testForwardListInvariants(list);
  BOOST_TEST(list.size() == size);
  auto pred = [](ListT::const_reference v) -> bool
  {
    return value == v;
  };
  BOOST_TEST(std::all_of(list.begin(), list.end(), pred));
}

BOOST_AUTO_TEST_CASE(empty_range_constructor)
{
  const std::initializer_list< ListT::value_type > init{};
  const ListT list(init.begin(), init.end());
  testForwardListInvariants(list);
  BOOST_TEST(list.empty());
}

BOOST_AUTO_TEST_CASE(range_constructor)
{
  const std::initializer_list< ListT::value_type > init{ 1, 2, 3, 4, 5 };
  const ListT list(init.begin(), init.end());
  testForwardListInvariants(list);
  BOOST_TEST(list.size() == init.size());
  BOOST_TEST(std::equal(list.begin(), list.end(), init.begin()));
}

BOOST_AUTO_TEST_CASE(initializer_list_constructor)
{
  const std::initializer_list< ListT::value_type > init{ 1, 2, 3, 4, 5 };
  const ListT list(init);
  testForwardListInvariants(list);
  BOOST_TEST(list.size() == init.size());
  BOOST_TEST(std::equal(list.begin(), list.end(), init.begin()));
}

BOOST_AUTO_TEST_SUITE_END();
BOOST_AUTO_TEST_SUITE(assignment_operators);

BOOST_AUTO_TEST_CASE(copy_assignment_operator)
{
  const ListT list{ 1, 2, 3, 4, 5 };
  ListT copied;
  copied = list;
  testForwardListInvariants(copied);
  BOOST_TEST(list == copied);
}

BOOST_AUTO_TEST_CASE(move_assignment_operator)
{
  const ListT list{ 1, 2, 3, 4, 5 };
  const ListT copied(list);
  ListT moved;
  moved = std::move(list);
  testForwardListInvariants(moved);
  BOOST_TEST(moved == copied);
}

BOOST_AUTO_TEST_CASE(initializer_list_assignment_operator)
{
  const std::initializer_list< ListT::value_type > init{ 1, 2, 3, 4, 5 };
  const ListT initied(init);
  ListT list;
  list = init;
  testForwardListInvariants(list);
  BOOST_TEST(list == initied);
}

BOOST_AUTO_TEST_SUITE_END();
BOOST_AUTO_TEST_SUITE(modifiers);

BOOST_AUTO_TEST_CASE(push_back_lvalue)
{
  constexpr ListT::value_type value = 7;
  constexpr ListT::size_type initSize = 77;
  ListT list(initSize);
  list.pushBack(value);
  testForwardListInvariants(list);
  BOOST_TEST(list.size() == initSize + 1);
  BOOST_TEST(list.back() == value);
}

BOOST_AUTO_TEST_CASE(push_back_rvalue)
{
  constexpr ListT::value_type value = 7;
  constexpr ListT::size_type initSize = 77;
  ListT list(initSize);
  list.pushBack(std::move(value));
  testForwardListInvariants(list);
  BOOST_TEST(list.size() == initSize + 1);
  BOOST_TEST(list.back() == value);
}

BOOST_AUTO_TEST_CASE(push_front_lvalue)
{
  constexpr ListT::value_type value = 7;
  constexpr ListT::size_type initSize = 77;
  ListT list(initSize);
  list.pushFront(value);
  testForwardListInvariants(list);
  BOOST_TEST(list.size() == initSize + 1);
  BOOST_TEST(list.front() == value);
}

BOOST_AUTO_TEST_CASE(push_front_rvalue)
{
  constexpr ListT::value_type value = 7;
  constexpr ListT::size_type initSize = 77;
  ListT list(initSize);
  list.pushFront(std::move(value));
  testForwardListInvariants(list);
  BOOST_TEST(list.size() == initSize + 1);
  BOOST_TEST(list.front() == value);
}

BOOST_AUTO_TEST_CASE(emplace_back_empty)
{
  constexpr ListT::value_type value = 7;
  ListT list;
  list.emplaceBack(value);
  testForwardListInvariants(list);
  BOOST_TEST(list.size() == 1);
  BOOST_TEST(list.front() == value);
  BOOST_TEST(list.back() == value);
}

BOOST_AUTO_TEST_CASE(emplace_front_empty)
{
  constexpr ListT::value_type value = 7;
  ListT list;
  list.emplaceFront(value);
  testForwardListInvariants(list);
  BOOST_TEST(list.size() == 1);
  BOOST_TEST(list.front() == value);
  BOOST_TEST(list.back() == value);
}

BOOST_AUTO_TEST_CASE(emplace_back)
{
  constexpr ListT::value_type value = 7;
  constexpr ListT::size_type initSize = 77;
  ListT list(initSize);
  list.emplaceBack(value);
  testForwardListInvariants(list);
  BOOST_TEST(list.size() == initSize + 1);
  BOOST_TEST(list.back() == value);
}

BOOST_AUTO_TEST_CASE(emplace_front)
{
  constexpr ListT::value_type value = 7;
  constexpr ListT::size_type initSize = 77;
  ListT list(initSize);
  list.emplaceFront(value);
  testForwardListInvariants(list);
  BOOST_TEST(list.size() == initSize + 1);
  BOOST_TEST(list.front() == value);
}

BOOST_AUTO_TEST_CASE(emplace_after_empty)
{
  constexpr ListT::value_type value = 7;
  ListT list;
  list.emplaceAfter(list.beforeBegin(), value);
  testForwardListInvariants(list);
  BOOST_TEST(list.size() == 1);
  BOOST_TEST(list.front() == value);
}

BOOST_AUTO_TEST_CASE(emplace_after_end_prev)
{
  constexpr ListT::value_type value = 7;
  constexpr ListT::size_type initSize = 77;
  ListT list(initSize);
  const auto pos = std::next(list.begin(), list.size() - 1);
  ListT::iterator result = list.emplaceAfter(pos, value);
  testForwardListInvariants(list);
  BOOST_TEST(list.size() == initSize + 1);
  BOOST_TEST(list.back() == value);
  BOOST_TEST(*result == value);
}

BOOST_AUTO_TEST_CASE(emplace_after_before_begin)
{
  constexpr ListT::value_type value = 7;
  constexpr ListT::size_type initSize = 77;
  ListT list(initSize);
  list.emplaceAfter(list.beforeBegin(), value);
  testForwardListInvariants(list);
  BOOST_TEST(list.size() == initSize + 1);
  BOOST_TEST(list.front() == value);
}

BOOST_AUTO_TEST_CASE(emplace_after)
{
  constexpr ListT::value_type value = 7;
  constexpr ListT::size_type initSize = 77;
  ListT list(initSize);
  ListT::iterator result = list.emplaceAfter(list.begin(), value);
  testForwardListInvariants(list);
  BOOST_TEST(list.size() == initSize + 1);
  BOOST_TEST(*result == value);
  BOOST_TEST((result == ++list.begin()));
}

BOOST_AUTO_TEST_CASE(pop_back_single_element)
{
  ListT list{ 1 };
  list.popBack();
  testForwardListInvariants(list);
  BOOST_TEST(list.empty());
}

BOOST_AUTO_TEST_CASE(pop_front_single_element)
{
  ListT list{ 1 };
  list.popFront();
  testForwardListInvariants(list);
  BOOST_TEST(list.empty());
}

BOOST_AUTO_TEST_CASE(pop_back)
{
  const ListT expected{ 1, 2, 3, 4 };
  ListT list{ 1, 2, 3, 4, 5 };
  list.popBack();
  testForwardListInvariants(list);
  BOOST_TEST(list == expected);
}

BOOST_AUTO_TEST_CASE(pop_front)
{
  const ListT expected{ 2, 3, 4, 5 };
  ListT list{ 1, 2, 3, 4, 5 };
  list.popFront();
  testForwardListInvariants(list);
  BOOST_TEST(list == expected);
}

BOOST_AUTO_TEST_CASE(erase_after_single_element)
{
  const ListT expected{ 1, 3, 4, 5 };
  ListT list{ 1, 2, 3, 4, 5 };
  ListT::iterator result = list.eraseAfter(list.begin());
  testForwardListInvariants(list);
  BOOST_TEST(list == expected);
  BOOST_TEST((result == list.begin()));
}

BOOST_AUTO_TEST_CASE(erase_after_empty_range)
{
  ListT list{ 1, 2, 3, 4, 5 };
  const ListT copied(list);
  ListT::iterator result = list.eraseAfter(list.begin(), ++list.begin());
  testForwardListInvariants(list);
  BOOST_TEST(list == copied);
  BOOST_TEST((result == list.begin()));
}

BOOST_AUTO_TEST_CASE(erase_after_range_with_end)
{
  const ListT expected{ 1, 2 };
  ListT list{ 1, 2, 3, 4, 5 };
  ListT::iterator result = list.eraseAfter(++list.begin(), list.end());
  testForwardListInvariants(list);
  BOOST_TEST(list == expected);
  BOOST_TEST((result == ++list.begin()));
}

BOOST_AUTO_TEST_CASE(erase_after_range)
{
  const ListT expected{ 1, 4, 5 };
  ListT list{ 1, 2, 3, 4, 5 };
  ListT::iterator result = list.eraseAfter(list.begin(), std::next(list.begin(), 3));
  testForwardListInvariants(list);
  BOOST_TEST(list == expected);
  BOOST_TEST((result == list.begin()));
}

BOOST_AUTO_TEST_CASE(empty_fill_assign)
{
  ListT list;
  list.assign(0, 0);
  testForwardListInvariants(list);
  BOOST_TEST(list.empty());
}

BOOST_AUTO_TEST_CASE(fill_assign)
{
  constexpr ListT::size_type size = 100;
  constexpr ListT::value_type value = 10;
  ListT list;
  list.assign(size, value);
  testForwardListInvariants(list);
  BOOST_TEST(list.size() == size);
  auto pred = [](ListT::const_reference v) -> bool
  {
    return value == v;
  };
  BOOST_TEST(std::all_of(list.begin(), list.end(), pred));
}

BOOST_AUTO_TEST_CASE(empty_range_assign)
{
  const std::initializer_list< ListT::value_type > init{};
  ListT list;
  list.assign(init.begin(), init.end());
  testForwardListInvariants(list);
  BOOST_TEST(list.empty());
}

BOOST_AUTO_TEST_CASE(range_assign)
{
  const std::initializer_list< ListT::value_type > init{ 1, 2, 3, 4, 5 };
  ListT list;
  list.assign(init.begin(), init.end());
  testForwardListInvariants(list);
  BOOST_TEST(list.size() == init.size());
  BOOST_TEST(std::equal(list.begin(), list.end(), init.begin()));
}

BOOST_AUTO_TEST_CASE(initializer_list_assign)
{
  const std::initializer_list< ListT::value_type > init{ 1, 2, 3, 4, 5 };
  ListT list;
  list.assign(init);
  testForwardListInvariants(list);
  BOOST_TEST(list.size() == init.size());
  BOOST_TEST(std::equal(list.begin(), list.end(), init.begin()));
}

BOOST_AUTO_TEST_CASE(insert_after)
{
  constexpr ListT::value_type value = 7;
  constexpr ListT::size_type initSize = 77;
  ListT list(initSize);
  ListT::iterator result = list.insertAfter(list.begin(), value);
  testForwardListInvariants(list);
  BOOST_TEST(list.size() == initSize + 1);
  BOOST_TEST(*result == value);
  BOOST_TEST((result == ++list.begin()));
}

BOOST_AUTO_TEST_CASE(insert_after_fill_empty)
{
  ListT list{ 1, 2, 3 };
  const ListT copied(list);
  ListT::iterator result = list.insertAfter(list.begin(), 0, 5);
  testForwardListInvariants(list);
  BOOST_TEST(list == copied);
  BOOST_TEST((result == list.begin()));
}

BOOST_AUTO_TEST_CASE(insert_after_fill)
{
  const ListT expected{ 1, 5, 5, 5, 2, 3 };
  ListT list{ 1, 2, 3 };
  ListT::iterator result = list.insertAfter(list.begin(), 3, 5);
  testForwardListInvariants(list);
  BOOST_TEST(list == expected);
  BOOST_TEST((result == ++list.begin()));
}

BOOST_AUTO_TEST_CASE(insert_after_range_empty)
{
  const std::initializer_list< ListT::value_type > init{};
  ListT list{ 1, 2, 3 };
  const ListT copied(list);
  ListT::iterator result = list.insertAfter(list.begin(), init.begin(), init.end());
  testForwardListInvariants(list);
  BOOST_TEST(list == copied);
  BOOST_TEST((result == list.begin()));
}

BOOST_AUTO_TEST_CASE(insert_after_range)
{
  const std::initializer_list< ListT::value_type > init{ 5, 5, 5 };
  const ListT expected{ 1, 5, 5, 5, 2, 3 };
  ListT list{ 1, 2, 3 };
  ListT::iterator result = list.insertAfter(list.begin(), init.begin(), init.end());
  testForwardListInvariants(list);
  BOOST_TEST(list == expected);
  BOOST_TEST((result == ++list.begin()));
}

BOOST_AUTO_TEST_CASE(insert_after_initializer_list)
{
  const std::initializer_list< ListT::value_type > init{ 5, 5, 5 };
  const ListT expected{ 1, 5, 5, 5, 2, 3 };
  ListT list{ 1, 2, 3 };
  ListT::iterator result = list.insertAfter(list.begin(), init);
  testForwardListInvariants(list);
  BOOST_TEST(list == expected);
  BOOST_TEST((result == ++list.begin()));
}

BOOST_AUTO_TEST_CASE(clear)
{
  ListT list{ 1, 2, 3, 4, 5 };
  list.clear();
  testForwardListInvariants(list);
  BOOST_TEST(list.empty());
}

BOOST_AUTO_TEST_CASE(swap)
{
  ListT first{ 1, 2, 3, 4, 5 };
  ListT second{ 2, 4, 6, 2 };
  const ListT firstCopy(first);
  const ListT secondCopy(second);
  first.swap(second);
  testForwardListInvariants(first);
  testForwardListInvariants(second);
  BOOST_TEST(first == secondCopy);
  BOOST_TEST(second == firstCopy);
}

BOOST_AUTO_TEST_SUITE_END();
BOOST_AUTO_TEST_SUITE(operations);

BOOST_AUTO_TEST_CASE(reverse_empty)
{
  ListT list;
  list.reverse();
  testForwardListInvariants(list);
  BOOST_TEST(list.empty());
}

BOOST_AUTO_TEST_CASE(reverse)
{
  const ListT expected{ 5, 4, 3, 2, 1 };
  ListT list{ 1, 2, 3, 4, 5 };
  list.reverse();
  testForwardListInvariants(list);
  BOOST_TEST(list == expected);
}

BOOST_AUTO_TEST_CASE(splice_after_this)
{
  ListT list{ 1, 2, 3, 4, 5 };
  const ListT copied(list);
  list.spliceAfter(list.begin(), list, list.begin(), list.end());
  testForwardListInvariants(list);
  BOOST_TEST(list == copied);
}

BOOST_AUTO_TEST_CASE(splice_after_end)
{
  const ListT expectedFirst{ 1 };
  const ListT expectedSecond{ 10, 11, 12, 2, 3, 4, 5 };
  ListT first = { 1, 2, 3, 4, 5 };
  ListT second = { 10, 11, 12 };
  const auto pos = std::next(second.begin(), second.size() - 1);
  second.spliceAfter(pos, first, first.begin(), first.end());
  testForwardListInvariants(first);
  testForwardListInvariants(second);
  BOOST_TEST(first == expectedFirst);
  BOOST_TEST(second == expectedSecond);
}

BOOST_AUTO_TEST_CASE(splice_after_empty_range)
{
  ListT list{ 1, 2, 3, 4, 5 };
  ListT other{ 1, 2, 3 };
  const ListT copied(list);
  list.spliceAfter(list.begin(), list, other.begin(), other.begin());
  testForwardListInvariants(list);
  BOOST_TEST(list == copied);
}

BOOST_AUTO_TEST_CASE(splice_after)
{
  const ListT expectedFirst{ 1 };
  const ListT expectedSecond{ 10, 2, 3, 4, 5, 11, 12 };
  ListT first = { 1, 2, 3, 4, 5 };
  ListT second = { 10, 11, 12 };
  second.spliceAfter(second.begin(), first, first.begin(), first.end());
  testForwardListInvariants(first);
  testForwardListInvariants(second);
  BOOST_TEST(first == expectedFirst);
  BOOST_TEST(second == expectedSecond);
}

BOOST_AUTO_TEST_CASE(remove_value_from_empty)
{
  ListT list;
  list.remove(1);
  testForwardListInvariants(list);
  BOOST_TEST(list.empty());
}

BOOST_AUTO_TEST_CASE(remove_value_from_filled)
{
  ListT list(100, 1);
  list.remove(1);
  testForwardListInvariants(list);
  BOOST_TEST(list.empty());
}

BOOST_AUTO_TEST_CASE(remove_value)
{
  ListT list{ 1, 1, 3, 4, 1, 1 };
  const ListT expected{ 3, 4 };
  list.remove(1);
  testForwardListInvariants(list);
  BOOST_TEST(list == expected);
}

BOOST_AUTO_TEST_CASE(remove_if)
{
  ListT list{ 1, 1, 3, 4, 1, 1 };
  const ListT expected{ 3, 4 };
  auto pred = [](ListT::const_reference v) -> bool
  {
    return v == 1;
  };
  list.removeIf(pred);
  testForwardListInvariants(list);
  BOOST_TEST(list == expected);
}

BOOST_AUTO_TEST_CASE(unique_empty)
{
  ListT list;
  list.unique();
  testForwardListInvariants(list);
  BOOST_TEST(list.empty());
}

BOOST_AUTO_TEST_CASE(unique_filled)
{
  ListT list(100, 1);
  const ListT expected{ 1 };
  list.unique();
  testForwardListInvariants(list);
  BOOST_TEST(list == expected);
}

BOOST_AUTO_TEST_CASE(unique)
{
  ListT list{ 1, 1, 1, 3, 4, 1, 1 };
  const ListT expected{ 1, 3, 4, 1 };
  list.unique();
  testForwardListInvariants(list);
  BOOST_TEST(list == expected);
}

BOOST_AUTO_TEST_CASE(unique_predicate)
{
  ListT list{ 1, 1, 1, 3, 4, 1, 1 };
  const ListT expected{ 1, 3, 4, 1 };
  auto pred = [](ListT::const_reference lhs, ListT::const_reference rhs) -> bool
  {
    return lhs == rhs;
  };
  list.unique(pred);
  testForwardListInvariants(list);
  BOOST_TEST(list == expected);
}

BOOST_AUTO_TEST_CASE(merge_empty_source)
{
  ListT list{ 1, 2, 3, 4, 5 };
  const ListT copied(list);
  ListT source;
  list.merge(source);
  testForwardListInvariants(list);
  testForwardListInvariants(source);
  BOOST_TEST(list == copied);
  BOOST_TEST(source.empty());
}

BOOST_AUTO_TEST_CASE(merge_this_source)
{
  ListT list{ 1, 2, 3, 4, 5 };
  const ListT copied(list);
  list.merge(list);
  testForwardListInvariants(list);
  BOOST_TEST(list == copied);
}

BOOST_AUTO_TEST_CASE(merge_in_empty)
{
  ListT list;
  ListT source{ 1, 2, 3, 4, 5 };
  const ListT expected(source);
  list.merge(source);
  testForwardListInvariants(list);
  testForwardListInvariants(source);
  BOOST_TEST(list == expected);
  BOOST_TEST(source.empty());
}

BOOST_AUTO_TEST_CASE(merge)
{
  ListT list{ 1, 3, 5, 7 };
  ListT source{ 2, 4, 6, 8 };
  const ListT expected{ 1, 2, 3, 4, 5, 6, 7, 8 };
  list.merge(source);
  testForwardListInvariants(list);
  testForwardListInvariants(source);
  BOOST_TEST(list == expected);
  BOOST_TEST(source.empty());
}

BOOST_AUTO_TEST_CASE(merge_comparator)
{
  ListT list{ 1, 3, 5, 7 };
  ListT source{ 2, 4, 6, 8 };
  const ListT expected{ 1, 2, 3, 4, 5, 6, 7, 8 };
  list.merge(source, std::less< ListT::value_type >{});
  testForwardListInvariants(list);
  testForwardListInvariants(source);
  BOOST_TEST(list == expected);
  BOOST_TEST(source.empty());
}

BOOST_AUTO_TEST_CASE(sort_single_elem)
{
  ListT list{ 3 };
  list.sort();
  testForwardListInvariants(list);
  BOOST_TEST(list.front() == 3);
  BOOST_TEST(list.size() == 1);
}

BOOST_AUTO_TEST_CASE(sort)
{
  ListT list{ 3, 5, 7, 6, 8, 2, 1, 4 };
  const ListT expected{ 1, 2, 3, 4, 5, 6, 7, 8 };
  list.sort();
  testForwardListInvariants(list);
  BOOST_TEST(list == expected);
}

BOOST_AUTO_TEST_CASE(sort_comparator)
{
  ListT list{ 3, 5, 7, 6, 8, 2, 1, 4 };
  const ListT expected{ 1, 2, 3, 4, 5, 6, 7, 8 };
  list.sort(std::less< ListT::value_type >{});
  testForwardListInvariants(list);
  BOOST_TEST(list == expected);
}

BOOST_AUTO_TEST_SUITE_END();
BOOST_AUTO_TEST_SUITE(comparison_operators);

BOOST_AUTO_TEST_CASE(comparison_empty_lists)
{
  const ListT list1;
  const ListT list2;
  BOOST_TEST(!(list1 < list2));
  BOOST_TEST(!(list1 > list2));
  BOOST_TEST(list1 == list2);
  testComparisonInvariants(list1, list2);
}

BOOST_AUTO_TEST_CASE(equality_operator_different_sizes)
{
  const ListT list1(10);
  const ListT list2(11);
  BOOST_TEST(!(list1 == list2));
  testComparisonInvariants(list1, list2);
}

BOOST_AUTO_TEST_CASE(equality_operator_different_values)
{
  const ListT list1{ 1, 2, 3, 4 };
  const ListT list2{ 1, 2, 3, 5 };
  BOOST_TEST(!(list1 == list2));
  testComparisonInvariants(list1, list2);
}

BOOST_AUTO_TEST_CASE(equality_operator_same)
{
  const ListT list1{ 1, 2, 3, 4 };
  const ListT list2{ 1, 2, 3, 4 };
  BOOST_TEST(list1 == list2);
  testComparisonInvariants(list1, list2);
}

BOOST_AUTO_TEST_CASE(less_operator_different_sizes)
{
  const ListT list1{ 1, 2 };
  const ListT list2{ 1, 2, 3 };
  BOOST_TEST(list1 < list2);
  testComparisonInvariants(list1, list2);
}

BOOST_AUTO_TEST_CASE(less_operator_different_values)
{
  const ListT list1{ 1, 2, 3 };
  const ListT list2{ 1, 4, 2 };
  BOOST_TEST(list1 < list2);
  testComparisonInvariants(list1, list2);
}

BOOST_AUTO_TEST_SUITE_END();

