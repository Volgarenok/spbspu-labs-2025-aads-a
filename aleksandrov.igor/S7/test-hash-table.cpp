#include <boost/test/unit_test.hpp>
#include <algorithm>
#include "hash-table.hpp"

using aleksandrov::HashTable;

BOOST_AUTO_TEST_SUITE(hash_table_construction)

BOOST_AUTO_TEST_CASE(default_construction)
{
  using aleksandrov::maxLoadFactorValue;

  HashTable< int, int > ht;
  BOOST_TEST(ht.empty());
  BOOST_TEST(ht.size() == 0);
  BOOST_TEST(ht.loadFactor() == 0);
  BOOST_TEST(ht.maxLoadFactor() == maxLoadFactorValue);
}

BOOST_AUTO_TEST_CASE(range_construction)
{
  std::vector< std::pair< int, int > > vector;
  vector = { { 1, 1 }, { 2, 2 }, { 3, 3 } };

  HashTable< int, int > ht(vector.begin(), vector.end());
  BOOST_CHECK(std::is_permutation(ht.begin(), ht.end(), vector.begin()));
}

BOOST_AUTO_TEST_CASE(initializer_list_construction)
{
  std::vector< std::pair< int, int > > vector;
  vector = { { 1, 1 }, { 2, 2 }, { 3, 3 } };

  HashTable< int, int > ht({ { 1, 1 }, { 2, 2 }, { 3, 3 } });
  BOOST_CHECK(std::is_permutation(ht.begin(), ht.end(), vector.begin()));
}

BOOST_AUTO_TEST_CASE(copy_construction)
{
  HashTable< int, int > ht1;
  HashTable< int, int > ht2(ht1);
  BOOST_TEST(ht2.empty());
}

BOOST_AUTO_TEST_CASE(move_construction)
{
  HashTable< int, int > ht1;
  HashTable< int, int > ht2(std::move(ht1));
  BOOST_TEST(ht2.empty());
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(hash_table_assignment)

BOOST_AUTO_TEST_CASE(copy_assignment)
{
  HashTable< int, int > ht1;
  HashTable< int, int > ht2;
  ht2 = ht1;
  BOOST_TEST(ht2.empty());
}

BOOST_AUTO_TEST_CASE(move_assignment)
{
  HashTable< int, int > ht1;
  HashTable< int, int > ht2;
  ht2 = std::move(ht1);
  BOOST_TEST(ht2.empty());
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(hash_table_iterators)

BOOST_AUTO_TEST_CASE(begin_end)
{
  HashTable< int, int > hashtable;
  BOOST_CHECK(hashtable.begin() == hashtable.end());
  BOOST_CHECK(hashtable.cbegin() == hashtable.cend());
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(hash_table_element_access)

BOOST_AUTO_TEST_CASE(square_brackets_operator)
{
  HashTable< int, int > ht;
  ht.insert({ 1, 1 });
  BOOST_TEST(ht[1] == 1);

  ht.insert({ 2, 2 });
  BOOST_TEST(ht[1] == 1);
  BOOST_TEST(ht[2] == 2);
}

BOOST_AUTO_TEST_CASE(at)
{
  HashTable< int, int > ht;
  ht.insert({ 1, 1 });
  BOOST_TEST(ht.at(1) == 1);

  ht.insert({ 2, 2 });
  BOOST_TEST(ht.at(1) == 1);
  BOOST_TEST(ht.at(2) == 2);

  ht.clear();
  BOOST_CHECK_THROW(ht.at(1), std::out_of_range);
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(hash_table_capacity)

BOOST_AUTO_TEST_CASE(empty)
{
  HashTable< int, int > ht;
  BOOST_TEST(ht.empty());

  ht.insert({ 0, 0 });
  BOOST_TEST(!ht.empty());

  ht.clear();
  ht[0] = 0;
  BOOST_TEST(!ht.empty());

  ht.clear();
  BOOST_TEST(ht.empty());
}

BOOST_AUTO_TEST_CASE(size)
{
  HashTable< int, int > ht;
  BOOST_TEST(ht.size() == 0);

  ht.insert({ 0, 0 });
  BOOST_TEST(ht.size() == 1);

  ht.clear();
  BOOST_TEST(ht.size() == 0);

  ht[0] = 0;
  BOOST_TEST(ht.size() == 1);

  ht[1] = 0;
  BOOST_TEST(ht.size() == 2);

  ht.clear();
  BOOST_TEST(ht.size() == 0);
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(hash_table_modifiers)

BOOST_AUTO_TEST_CASE(clear)
{
  HashTable< int, int > ht;
  ht.clear();

  ht = { { 1, 0 }, { 2, 0 }, { 3, 0 } };
  ht.clear();
  ht.clear();
  BOOST_TEST(ht.empty());
}

BOOST_AUTO_TEST_CASE(insert)
{
  HashTable< int, int > ht;

  ht.insert({ 1, 1 });
  BOOST_TEST(ht[1] == 1);

  auto pair = ht.insert({ 1, 1 });
  BOOST_TEST(ht[1] == 1);
  BOOST_CHECK(pair.first == ht.find(1));
  BOOST_TEST(pair.second == false);

  pair = ht.insert({ 2, 2 });
  BOOST_TEST(ht[1] == 1);
  BOOST_TEST(ht[2] == 2);
  BOOST_CHECK(pair.first == ht.find(2));
  BOOST_TEST(pair.second == true);

  pair = ht.insert({ 2, 3 });
  BOOST_TEST(ht[2] == 2);
  BOOST_CHECK(pair.first == ht.find(2));
  BOOST_TEST(pair.second == false);

  size_t n = ht[3] = 3;
  BOOST_TEST(ht.size() == 3);
  BOOST_TEST(n == 3);
}

BOOST_AUTO_TEST_CASE(insert_range)
{
  HashTable< int, char > ht = { { 3, 'c' } };
  std::vector< std::pair< int, char > > vector;
  vector = { { 1, 'a' }, { 2, 'b' }, { 4, 'd' } };
  HashTable< int, char > expected = { { 1, 'a' }, { 2, 'b' }, { 3, 'c' }, { 4, 'd' } };

  ht.insert(vector.begin(), vector.end());
  BOOST_TEST(ht.size() == 4);
  BOOST_TEST(std::is_permutation(ht.begin(), ht.end(), expected.begin()));
}

BOOST_AUTO_TEST_CASE(insert_initializer_list)
{
  HashTable< int, char > ht = { { 3, 'c' } };
  HashTable< int, char > expected = { { 1, 'a' }, { 2, 'b' }, { 3, 'c' }, { 4, 'd' } };

  ht.insert({ { 1, 'a' }, { 2, 'b' }, { 4, 'd' } });
  BOOST_TEST(ht.size() == 4);
  BOOST_TEST(std::is_permutation(ht.begin(), ht.end(), expected.begin()));
}

BOOST_AUTO_TEST_CASE(emplace)
{
  HashTable< int, char > ht;

  auto pair = ht.emplace(1, 'a');
  BOOST_TEST(ht[1] == 'a');
  BOOST_CHECK(pair.first == ht.find(1));
  BOOST_TEST(pair.second == true);

  pair = ht.emplace(1, 'b');
  BOOST_TEST(ht[1] == 'a');
  BOOST_CHECK(pair.first == ht.find(1));
  BOOST_TEST(pair.second == false);

  pair = ht.emplace(2, 'b');
  BOOST_TEST(ht[2] == 'b');
  BOOST_CHECK(pair.first == ht.find(2));
  BOOST_TEST(pair.second == true);
}

BOOST_AUTO_TEST_CASE(erase)
{
  HashTable< int, int > ht = { { 1, 1 }, { 2, 2 }, { 3, 3 } };

  auto it = ht.erase(ht.find(2));
  BOOST_TEST(ht.size() == 2);
  BOOST_TEST(ht.count(2) == 0);
  BOOST_TEST(it->first == 3);

  size_t i = ht.erase(3);
  BOOST_TEST(i == 1);
  BOOST_TEST(ht.size() == 1);
  BOOST_TEST(ht.count(3) == 0);

  i = ht.erase(0);
  BOOST_TEST(i == 0);
  BOOST_TEST(ht.size() == 1);
}

BOOST_AUTO_TEST_CASE(erase_range)
{
  HashTable< int, int > ht;
  ht = { { 1, 1 }, { 2, 2 }, { 3, 3 }, { 4, 4 }, { 5, 5 } };

  auto it = ht.erase(std::next(ht.cbegin()), std::next(ht.cbegin(), 3));
  BOOST_TEST(ht.size() == 3);
  BOOST_TEST(ht.count(2) == 0);
  BOOST_TEST(ht.count(3) == 0);
  BOOST_TEST(ht.count(4) == 1);

  it = ht.erase(ht.cbegin(), ht.cend());
  BOOST_TEST(ht.empty());
  BOOST_CHECK(it == ht.end());
}

BOOST_AUTO_TEST_CASE(swap)
{
  HashTable< int, int > ht1;
  HashTable< int, int > ht2;

  ht1.swap(ht2);
  ht2.swap(ht1);
  BOOST_TEST(ht1.empty());
  BOOST_TEST(ht2.empty());

  ht1 = { { 1, 1 }, { 2, 2 } };
  HashTable< int, int > ht1Copy(ht1);
  ht2.insert({ 3, 3 });
  HashTable< int, int > ht2Copy(ht2);

  ht1.swap(ht2);
  BOOST_TEST(std::is_permutation(ht1.begin(), ht1.end(), ht2Copy.begin()));
  BOOST_TEST(std::is_permutation(ht2.begin(), ht2.end(), ht1Copy.begin()));

  ht2.swap(ht1);
  BOOST_TEST(std::is_permutation(ht1.begin(), ht1.end(), ht1Copy.begin()));
  BOOST_TEST(std::is_permutation(ht2.begin(), ht2.end(), ht2Copy.begin()));
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(hash_table_lookup)

BOOST_AUTO_TEST_CASE(count)
{
  HashTable< int, int > ht;
  BOOST_TEST(ht.count(0) == 0);

  ht = { { 1, 1 }, { 2, 2 }, { 3, 3 } };
  BOOST_TEST(ht.count(0) == 0);
  BOOST_TEST(ht.count(1) == 1);
  BOOST_TEST(ht.count(2) == 1);
  BOOST_TEST(ht.count(3) == 1);
}

BOOST_AUTO_TEST_CASE(find)
{
  HashTable< int, int > ht;
  BOOST_CHECK(ht.find(0) == ht.end());

  ht = { { 1, 1 }, { 2, 2 }, { 3, 3 }, { 4, 4 }, { 5, 5 } };
  BOOST_CHECK(ht.find(0) == ht.end());

  auto it = ht.find(4);
  BOOST_CHECK(it != ht.end());
  BOOST_TEST(it->first == 4);
  BOOST_TEST(it->second == 4);
}

BOOST_AUTO_TEST_CASE(equal_range)
{
  HashTable< int, int > ht;
  auto p1 = ht.equalRange(1);
  BOOST_CHECK(p1.first == ht.end());
  BOOST_CHECK(p1.second == ht.end());

  ht = { { 1, 1 }, { 2, 2 }, { 3, 3 } };
  auto p2 = ht.equalRange(2);
  BOOST_CHECK(p2.first != ht.end());
  BOOST_CHECK(p2.second != ht.end());
  BOOST_CHECK(std::distance(p2.first, p2.second) == 1);
  BOOST_CHECK(p2.first->first == 2);

  auto p3 = ht.equalRange(0);
  BOOST_CHECK(p3.first == ht.end());
  BOOST_CHECK(p3.second == ht.end());
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(hash_table_hash_policy)

BOOST_AUTO_TEST_CASE(load_factor)
{
  HashTable< int, int > ht;
  BOOST_TEST(ht.loadFactor() == 0.0f);

  ht.insert({ 1, 1 });
  float lf = ht.loadFactor();
  BOOST_TEST(lf > 0.0f);

  ht.insert({ 2, 2 });
  BOOST_TEST(ht.loadFactor() > lf);

  ht.clear();
  BOOST_TEST(ht.loadFactor() == 0.0f);
}

BOOST_AUTO_TEST_CASE(max_load_factor)
{
  using aleksandrov::maxLoadFactorValue;

  HashTable< int, int > ht;
  BOOST_TEST(ht.maxLoadFactor() == maxLoadFactorValue);

  ht.maxLoadFactor(0.5f);
  BOOST_TEST(ht.maxLoadFactor() == 0.5f);

  ht.insert({ 0, 0 });
  BOOST_TEST(ht.maxLoadFactor() == 0.5f);
}

BOOST_AUTO_TEST_CASE(rehash)
{
  using aleksandrov::minCapacity;

  HashTable< int, int > ht;
  ht.insert({ 1, 1 });
  BOOST_TEST(ht.capacity() == minCapacity);
  BOOST_TEST(ht.count(1) == 1);

  ht.clear();
  ht.maxLoadFactor(0.5f);
  for (size_t i = 0; i < minCapacity; ++i)
  {
    ht.insert({ i + 1, i + 1 });
  }
  BOOST_TEST(ht.size() == minCapacity);
  BOOST_TEST(ht.capacity() > minCapacity);

  size_t oldCapacity = ht.capacity();
  ht.rehash(oldCapacity * 2);
  BOOST_TEST(ht.capacity() >= oldCapacity * 2);
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(hash_table_observers)

BOOST_AUTO_TEST_CASE(hash_function)
{
  struct SimpleHasher
  {
    size_t operator()(int key) const
    {
      return key + 1;
    }
  };

  HashTable< int, int > ht1;
  boost::hash< int > defaultHasher;
  BOOST_CHECK(ht1.hashFunction()(10) == defaultHasher(10));
  BOOST_CHECK(ht1.hashFunction()(100) == defaultHasher(100));

  HashTable< int, int, SimpleHasher > ht2;
  SimpleHasher hasher = ht2.hashFunction();
  BOOST_CHECK(hasher(10) == 11);
}

BOOST_AUTO_TEST_CASE(key_equal)
{
  struct SimpleKeyEqual
  {
    bool operator()(int lhs, int rhs) const
    {
      return lhs % 2 == rhs % 2;
    }
  };

  HashTable< int, int > ht1;
  BOOST_TEST(ht1.keyEq()(1, 1));

  HashTable< int, int, boost::hash< int >, SimpleKeyEqual > ht2;
  SimpleKeyEqual keyEqual = ht2.keyEq();
  BOOST_TEST(keyEqual(1, 3));
  BOOST_TEST(!keyEqual(1, 2));
}

BOOST_AUTO_TEST_SUITE_END();

