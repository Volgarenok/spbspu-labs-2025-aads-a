#include <boost/test/unit_test_suite.hpp>
#include "test-utils.hpp"

using kizhin::BufferT;

BOOST_AUTO_TEST_SUITE(constructors);

BOOST_AUTO_TEST_CASE(default_constructor)
{
  const BufferT buffer;
  testBufferInvariants(buffer);
  BOOST_TEST(buffer.empty());
}

BOOST_AUTO_TEST_CASE(copy_constructor_from_empty)
{
  const BufferT buffer;
  const BufferT copied(buffer);
  testBufferInvariants(copied);
  BOOST_TEST(copied.empty());
}

BOOST_AUTO_TEST_CASE(copy_constructor)
{
  const BufferT buffer{ 1, 2, 3, 4, 5 };
  const BufferT copied(buffer);
  testBufferInvariants(copied);
  BOOST_TEST(copied == buffer);
}

BOOST_AUTO_TEST_CASE(move_constructor)
{
  const BufferT buffer{ 1, 2, 3, 4, 5 };
  const BufferT copied(buffer);
  const BufferT moved(std::move(buffer));
  testBufferInvariants(moved);
  BOOST_TEST(moved == copied);
}

BOOST_AUTO_TEST_CASE(empty_fill_constructor)
{
  const BufferT buffer(0, 10);
  testBufferInvariants(buffer);
  BOOST_TEST(buffer.empty());
}

BOOST_AUTO_TEST_CASE(fill_constructor)
{
  constexpr BufferT::size_type size = 100;
  constexpr BufferT::value_type value = 10;
  const BufferT buffer(size, value);
  testBufferInvariants(buffer);
  BOOST_TEST(buffer.size() == size);
  auto pred = [](BufferT::const_reference v) -> bool
  {
    return value == v;
  };
  BOOST_TEST(std::all_of(buffer.begin(), buffer.end(), pred));
}

BOOST_AUTO_TEST_CASE(empty_range_constructor)
{
  const std::initializer_list< BufferT::value_type > init{};
  const BufferT buffer(init.begin(), init.end());
  testBufferInvariants(buffer);
  BOOST_TEST(buffer.empty());
}

BOOST_AUTO_TEST_CASE(range_constructor)
{
  const std::initializer_list< BufferT::value_type > init{ 1, 2, 3, 4, 5 };
  const BufferT buffer(init.begin(), init.end());
  testBufferInvariants(buffer);
  BOOST_TEST(buffer.size() == init.size());
  BOOST_TEST(std::equal(buffer.begin(), buffer.end(), init.begin()));
}

BOOST_AUTO_TEST_CASE(initializer_list_constructor)
{
  const std::initializer_list< BufferT::value_type > init{ 1, 2, 3, 4, 5 };
  const BufferT buffer(init);
  testBufferInvariants(buffer);
  BOOST_TEST(buffer.size() == init.size());
  BOOST_TEST(std::equal(buffer.begin(), buffer.end(), init.begin()));
}

BOOST_AUTO_TEST_SUITE_END();
BOOST_AUTO_TEST_SUITE(assignment_operators);

BOOST_AUTO_TEST_CASE(copy_assignment_operator)
{
  const BufferT buffer{ 1, 2, 3, 4, 5 };
  BufferT copied;
  copied = buffer;
  testBufferInvariants(copied);
  BOOST_TEST(buffer == copied);
}

BOOST_AUTO_TEST_CASE(move_assignment_operator)
{
  const BufferT buffer{ 1, 2, 3, 4, 5 };
  const BufferT copied(buffer);
  BufferT moved;
  moved = std::move(buffer);
  testBufferInvariants(moved);
  BOOST_TEST(moved == copied);
}

BOOST_AUTO_TEST_SUITE_END();
BOOST_AUTO_TEST_SUITE(modifiers);

BOOST_AUTO_TEST_CASE(push_back_lvalue)
{
  BufferT buffer{ 1, 2, 3 };
  const BufferT expected{ 1, 2, 3, 4 };
  constexpr BufferT::value_type value = 4;
  buffer.pushBack(value);
  testBufferInvariants(buffer);
  BOOST_TEST(buffer == expected);
}

BOOST_AUTO_TEST_CASE(push_back_rvalue)
{
  BufferT buffer{ 1, 2, 3 };
  const BufferT expected{ 1, 2, 3, 4 };
  constexpr BufferT::value_type value = 4;
  buffer.pushBack(std::move(value));
  testBufferInvariants(buffer);
  BOOST_TEST(buffer == expected);
}

BOOST_AUTO_TEST_CASE(push_front_lvalue)
{
  BufferT buffer{ 1, 2, 3 };
  const BufferT expected{ 4, 1, 2, 3 };
  constexpr BufferT::value_type value = 4;
  buffer.pushFront(value);
  testBufferInvariants(buffer);
  BOOST_TEST(buffer == expected);
}

BOOST_AUTO_TEST_CASE(push_front_rvalue)
{
  BufferT buffer{ 1, 2, 3 };
  const BufferT expected{ 4, 1, 2, 3 };
  constexpr BufferT::value_type value = 4;
  buffer.pushFront(std::move(value));
  testBufferInvariants(buffer);
  BOOST_TEST(buffer == expected);
}

BOOST_AUTO_TEST_CASE(emplace_back_empty)
{
  BufferT buffer;
  const BufferT expected{ 4 };
  buffer.emplaceBack(4);
  testBufferInvariants(buffer);
  BOOST_TEST(buffer == expected);
}

BOOST_AUTO_TEST_CASE(emplace_front_empty)
{
  BufferT buffer;
  const BufferT expected{ 4 };
  buffer.emplaceFront(4);
  testBufferInvariants(buffer);
  BOOST_TEST(buffer == expected);
}

BOOST_AUTO_TEST_CASE(emplace_back)
{
  BufferT buffer{ 1, 2, 3 };
  const BufferT expected{ 1, 2, 3, 4 };
  buffer.emplaceBack(4);
  testBufferInvariants(buffer);
  BOOST_TEST(buffer == expected);
}

BOOST_AUTO_TEST_CASE(emplace_front)
{
  BufferT buffer{ 1, 2, 3 };
  const BufferT expected{ 4, 1, 2, 3 };
  buffer.emplaceFront(4);
  testBufferInvariants(buffer);
  BOOST_TEST(buffer == expected);
}

BOOST_AUTO_TEST_CASE(pop_back_single_element)
{
  BufferT buffer{ 1 };
  buffer.popBack();
  testBufferInvariants(buffer);
  BOOST_TEST(buffer.empty());
}

BOOST_AUTO_TEST_CASE(pop_front_single_element)
{
  BufferT buffer{ 1 };
  buffer.popFront();
  testBufferInvariants(buffer);
  BOOST_TEST(buffer.empty());
}

BOOST_AUTO_TEST_CASE(pop_back)
{
  const BufferT expected{ 1, 2, 3, 4 };
  BufferT buffer{ 1, 2, 3, 4, 5 };
  buffer.popBack();
  testBufferInvariants(buffer);
  BOOST_TEST(buffer == expected);
}

BOOST_AUTO_TEST_CASE(pop_front)
{
  const BufferT expected{ 2, 3, 4, 5 };
  BufferT buffer{ 1, 2, 3, 4, 5 };
  buffer.popFront();
  testBufferInvariants(buffer);
  BOOST_TEST(buffer == expected);
}

BOOST_AUTO_TEST_CASE(empty_fill_assign)
{
  BufferT buffer;
  buffer.assign(0, 0);
  testBufferInvariants(buffer);
  BOOST_TEST(buffer.empty());
}

BOOST_AUTO_TEST_CASE(fill_assign)
{
  constexpr BufferT::size_type size = 100;
  constexpr BufferT::value_type value = 10;
  BufferT buffer;
  buffer.assign(size, value);
  testBufferInvariants(buffer);
  BOOST_TEST(buffer.size() == size);
  auto pred = [](BufferT::const_reference v) -> bool
  {
    return value == v;
  };
  BOOST_TEST(std::all_of(buffer.begin(), buffer.end(), pred));
}

BOOST_AUTO_TEST_CASE(empty_range_assign)
{
  const std::initializer_list< BufferT::value_type > init{};
  BufferT buffer;
  buffer.assign(init.begin(), init.end());
  testBufferInvariants(buffer);
  BOOST_TEST(buffer.empty());
}

BOOST_AUTO_TEST_CASE(range_assign)
{
  const std::initializer_list< BufferT::value_type > init{ 1, 2, 3, 4, 5 };
  BufferT buffer;
  buffer.assign(init.begin(), init.end());
  testBufferInvariants(buffer);
  BOOST_TEST(buffer.size() == init.size());
  BOOST_TEST(std::equal(buffer.begin(), buffer.end(), init.begin()));
}

BOOST_AUTO_TEST_CASE(initializer_list_assign)
{
  const std::initializer_list< BufferT::value_type > init{ 1, 2, 3, 4, 5 };
  BufferT buffer;
  buffer.assign(init);
  testBufferInvariants(buffer);
  BOOST_TEST(buffer.size() == init.size());
  BOOST_TEST(std::equal(buffer.begin(), buffer.end(), init.begin()));
}

BOOST_AUTO_TEST_CASE(clear)
{
  BufferT buffer{ 1, 2, 3, 4, 5 };
  buffer.clear();
  testBufferInvariants(buffer);
  BOOST_TEST(buffer.empty());
}

BOOST_AUTO_TEST_CASE(swap)
{
  BufferT first{ 1, 2, 3, 4, 5 };
  BufferT second{ 2, 4, 6, 2 };
  const BufferT firstCopy(first);
  const BufferT secondCopy(second);
  first.swap(second);
  testBufferInvariants(first);
  testBufferInvariants(second);
  BOOST_TEST(first == secondCopy);
  BOOST_TEST(second == firstCopy);
}

BOOST_AUTO_TEST_SUITE_END();
BOOST_AUTO_TEST_SUITE(comparison_operators);

BOOST_AUTO_TEST_CASE(comparison_empty_buffers)
{
  const BufferT buffer1;
  const BufferT buffer2;
  BOOST_TEST(!(buffer1 < buffer2));
  BOOST_TEST(!(buffer1 > buffer2));
  BOOST_TEST(buffer1 == buffer2);
  testBufferComparison(buffer1, buffer2);
}

BOOST_AUTO_TEST_CASE(equality_operator_different_sizes)
{
  const BufferT buffer1(10);
  const BufferT buffer2(11);
  BOOST_TEST(!(buffer1 == buffer2));
  testBufferComparison(buffer1, buffer2);
}

BOOST_AUTO_TEST_CASE(equality_operator_different_values)
{
  const BufferT buffer1{ 1, 2, 3, 4 };
  const BufferT buffer2{ 1, 2, 3, 5 };
  BOOST_TEST(!(buffer1 == buffer2));
  testBufferComparison(buffer1, buffer2);
}

BOOST_AUTO_TEST_CASE(equality_operator_same)
{
  const BufferT buffer1{ 1, 2, 3, 4 };
  const BufferT buffer2{ 1, 2, 3, 4 };
  BOOST_TEST(buffer1 == buffer2);
  testBufferComparison(buffer1, buffer2);
}

BOOST_AUTO_TEST_CASE(less_operator_different_sizes)
{
  const BufferT buffer1{ 1, 2 };
  const BufferT buffer2{ 1, 2, 3 };
  BOOST_TEST(buffer1 < buffer2);
  testBufferComparison(buffer1, buffer2);
}

BOOST_AUTO_TEST_CASE(less_operator_different_values)
{
  const BufferT buffer1{ 1, 2, 3 };
  const BufferT buffer2{ 1, 4, 2 };
  BOOST_TEST(buffer1 < buffer2);
  testBufferComparison(buffer1, buffer2);
}

BOOST_AUTO_TEST_SUITE_END();

