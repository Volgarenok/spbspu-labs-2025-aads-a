#include <boost/test/unit_test.hpp>
#include <vector>
#include "vector.hpp"

using aleksandrov::Vector;

BOOST_AUTO_TEST_SUITE(vector_construction)

BOOST_AUTO_TEST_CASE(default_construction)
{
  Vector< int > vector;
  BOOST_TEST(vector.empty());
  BOOST_TEST(vector.size() == 0);
}

BOOST_AUTO_TEST_CASE(fill_construction)
{
  Vector< int > vector(4ull, 1);
  BOOST_CHECK(vector == Vector< int >({ 1, 1, 1, 1 }));
}

BOOST_AUTO_TEST_CASE(range_construction)
{
  std::vector< int > numbers = { 1, 2, 3, 4 };
  Vector< int > vector(numbers.begin(), numbers.end());
  BOOST_CHECK(vector == Vector< int >({ 1, 2, 3, 4 }));
}

BOOST_AUTO_TEST_CASE(initializer_list_construction)
{
  Vector< int > vector({ 1, 2, 3, 4 });
  BOOST_CHECK(vector == Vector< int >({ 1, 2, 3, 4 }));
}

BOOST_AUTO_TEST_CASE(copy_construction)
{
  Vector< int > v1 = { 1, 2, 3, 4 };
  Vector< int > v2(v1);
  BOOST_CHECK(v2 == v1);
}

BOOST_AUTO_TEST_CASE(move_construction)
{
  Vector< int > v1 = { 1, 2, 3, 4 };
  Vector< int > v2(std::move(v1));
  BOOST_CHECK(v2 == Vector< int >({ 1, 2, 3, 4 }));
  BOOST_TEST(v1.size() == 0);
  BOOST_TEST(v1.capacity() == 0);
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(vector_assignment)

BOOST_AUTO_TEST_CASE(copy_assignment)
{
  Vector< int > v1 = { 1, 2, 3, 4 };
  Vector< int > v2 = { 2, 4, 6 };
  v2 = v1;
  BOOST_CHECK(v2 == v1);
}

BOOST_AUTO_TEST_CASE(move_assignment)
{
  Vector< int > v1 = { 1, 2, 3, 4 };
  Vector< int > v2 = { 3, 6, 9 };
  v2 = std::move(v1);
  BOOST_CHECK(v2 == Vector< int >({ 1, 2, 3, 4 }));
  BOOST_TEST(v1.size() == 0);
  BOOST_TEST(v1.capacity() == 0);
}

BOOST_AUTO_TEST_CASE(assign_fill)
{
  Vector< int > vector = { 1, 2, 3, 4, 5 };
  vector.assign(2ull, 1);
  BOOST_CHECK(vector == Vector< int >({ 1, 1 }));
}

BOOST_AUTO_TEST_CASE(assign_range)
{
  std::vector< int > numbers = { 1, 1 };
  Vector< int > vector = { 1, 2, 3, 4, 5 };
  vector.assign(numbers.begin(), numbers.end());
  BOOST_CHECK(vector == Vector< int >({ 1, 1 }));
}

BOOST_AUTO_TEST_CASE(assign_initializer_list)
{
  Vector< int > vector = { 1, 2, 3, 4, 5 };
  vector.assign({ 1, 1 });
  BOOST_CHECK(vector == Vector< int >({ 1, 1 }));
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(vector_element_access)

BOOST_AUTO_TEST_CASE(at)
{
  Vector< int > vector = { 1, 2, 3, 4 };
  BOOST_TEST(vector.at(0) == 1);
  BOOST_CHECK_THROW(vector.at(4), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(square_brackets_operator)
{
  Vector< int > vector = { 1, 2, 3, 4 };
  BOOST_TEST(vector[0] == 1);
  BOOST_TEST(vector[1] == 2);
}

BOOST_AUTO_TEST_CASE(front)
{
  Vector< int > vector = { 1, 2, 3 };
  BOOST_TEST(vector.front() == 1);
  vector = { 2, 3 };
  BOOST_TEST(vector.front() == 2);
}

BOOST_AUTO_TEST_CASE(back)
{
  Vector< int > vector = { 1, 2, 3 };
  BOOST_TEST(vector.back() == 3);
  vector.popBack();
  BOOST_TEST(vector.back() == 2);
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(vector_capacity)

BOOST_AUTO_TEST_CASE(empty)
{
  Vector< int > vector;
  BOOST_TEST(vector.empty());

  vector.pushBack(1);
  BOOST_TEST(!vector.empty());

  vector.popBack();
  BOOST_TEST(vector.empty());
}

BOOST_AUTO_TEST_CASE(size)
{
  Vector< int > vector;
  BOOST_TEST(vector.size() == 0);

  vector.pushBack(1);
  BOOST_TEST(vector.size() == 1);

  vector.assign(5ull, 1);
  BOOST_TEST(vector.size() == 5);

  vector.popBack();
  BOOST_TEST(vector.size() == 4);

  vector.clear();
  BOOST_TEST(vector.size() == 0);
}

BOOST_AUTO_TEST_CASE(capacity)
{
  using aleksandrov::minVectorCapacity;

  Vector< int > v1;
  BOOST_TEST(v1.capacity() == 0);

  v1.pushBack(0);
  BOOST_TEST(v1.capacity() == minVectorCapacity);

  v1.assign(minVectorCapacity + 1, 0);
  BOOST_TEST(v1.capacity() > minVectorCapacity);

  v1.clear();
  BOOST_TEST(v1.capacity() > minVectorCapacity);

  Vector< int > v2;
  BOOST_TEST(v2.capacity() < v1.capacity());
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(vector_modifiers)

BOOST_AUTO_TEST_CASE(clear)
{
  Vector< int > vector;
  vector.clear();
  BOOST_TEST(vector.empty());

  vector.assign(10ull, 0);
  vector.clear();
  vector.clear();
  BOOST_TEST(vector.empty());
}

BOOST_AUTO_TEST_CASE(pushBack)
{
  Vector< int > vector;
  vector.pushBack(1);
  BOOST_TEST(vector.size() == 1);
  BOOST_TEST(vector[0] == 1);

  vector.pushBack(2);
  BOOST_TEST(vector.size() == 2);
  BOOST_CHECK(vector == Vector< int >({ 1, 2 }));
}

BOOST_AUTO_TEST_CASE(emplaceBack)
{
  Vector< std::pair< int, int > > vector;
  vector.emplaceBack(1, 1);
  BOOST_TEST(vector.size() == 1);
  BOOST_TEST(vector[0].first == 1);

  vector.emplaceBack(2, 2);
  BOOST_TEST(vector.size() == 2);
  BOOST_TEST(vector[0].first == 1);
  BOOST_TEST(vector[1].first == 2);
}

BOOST_AUTO_TEST_CASE(popBack)
{
  Vector< int > vector(1ull, 1);
  vector.popBack();
  BOOST_TEST(vector.empty());

  vector.assign(2ull, 1);
  vector.popBack();
  BOOST_TEST(vector.size() == 1);

  vector.popBack();
  BOOST_TEST(vector.empty());
}

BOOST_AUTO_TEST_CASE(swap)
{
  Vector< int > v1;
  Vector< int > v2;
  v1.swap(v2);

  v1 = { 1, 2, 3 };
  v2.swap(v1);
  BOOST_TEST(v1.empty());
  BOOST_CHECK(v2 == Vector< int >({ 1, 2, 3 }));

  v1.swap(v2);
  BOOST_TEST(v2.empty());
  BOOST_CHECK(v1 == Vector< int >({ 1, 2, 3 }));
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_CASE(equal_operators)
{
  Vector< int > v1;
  Vector< int > v2;
  BOOST_CHECK(v1 == v2);

  v1.pushBack(0);
  BOOST_CHECK(v1 != v2);

  v2.pushBack(0);
  BOOST_CHECK(v1 == v2);

  v1.assign(10ull, 0);
  v2 = v1;
  BOOST_CHECK(v2 == v1);
}

