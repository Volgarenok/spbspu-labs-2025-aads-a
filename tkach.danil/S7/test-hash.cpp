#include <boost/test/unit_test.hpp>
#include <sstream>
#include <string>
#include <cmath>
#include "hash_table.hpp"
#include "graph.hpp"

using namespace tkach;
template< class Key, class Value >
void print(const HashTable< Key, Value >& hash_table, std::ostream& out)
{
  out << hash_table.cbegin()->first << " " << hash_table.cbegin()->second;
  for (auto it = ++hash_table.cbegin(); it != hash_table.cend(); ++it)
  {
    out << " " << it->first << " " << it->second;
  }
}

BOOST_AUTO_TEST_CASE(basic_contructor_test)
{
  HashTable< size_t, std::string > hash_table;
  BOOST_TEST(hash_table.empty());
  BOOST_TEST(hash_table.size() == 0);
}

BOOST_AUTO_TEST_CASE(basic_contructor_size_test)
{
  HashTable< size_t, std::string > hash_table(17);
  BOOST_TEST(hash_table.empty());
  BOOST_TEST(hash_table.size() == 0);
}

BOOST_AUTO_TEST_CASE(copy_move_contructor_test)
{
  HashTable< size_t, std::string > hash_table;
  for (size_t i = 0; i <= 5; ++i)
  {
    hash_table.insert(std::make_pair(i, std::to_string(i)));
  }
  HashTable< size_t, std::string > hash_table2(std::move(hash_table));
  BOOST_TEST(hash_table2.size() == 6);
  std::ostringstream out;
  print(hash_table2, out);
  BOOST_TEST(out.str() == "0 0 1 1 2 2 3 3 4 4 5 5");
}

BOOST_AUTO_TEST_CASE(copy_contructor_test)
{
  HashTable< size_t, std::string > hash_table;
  for (size_t i = 0; i <= 5; ++i)
  {
    hash_table.insert(std::make_pair(i, std::to_string(i)));
  }
  HashTable< size_t, std::string > hash_table2(hash_table);
  BOOST_TEST(hash_table2.size() == 6);
  std::ostringstream out;
  print(hash_table2, out);
  BOOST_TEST(out.str() == "0 0 1 1 2 2 3 3 4 4 5 5");
}

BOOST_AUTO_TEST_CASE(hash_equal_operator_test)
{
  HashTable< size_t, std::string > hash_table;
  for (size_t i = 0; i <= 5; ++i)
  {
    hash_table.insert(std::make_pair(i, std::to_string(i)));
  }
  HashTable< size_t, std::string > hash_table2;
  hash_table2 = hash_table;
  BOOST_TEST(hash_table2.size() == 6);
  std::ostringstream out;
  print(hash_table2, out);
  BOOST_TEST(out.str() == "0 0 1 1 2 2 3 3 4 4 5 5");
}

BOOST_AUTO_TEST_CASE(hash_equal_move_operator_test)
{
  HashTable< size_t, std::string > hash_table;
  for (size_t i = 0; i <= 5; ++i)
  {
    hash_table.insert(std::make_pair(i, std::to_string(i)));
  }
  HashTable< size_t, std::string > hash_table2;
  hash_table2 = std::move(hash_table);
  BOOST_TEST(hash_table2.size() == 6);
  std::ostringstream out;
  print(hash_table2, out);
  BOOST_TEST(out.str() == "0 0 1 1 2 2 3 3 4 4 5 5");
}

BOOST_AUTO_TEST_CASE(hash_table_operator_access_test)
{
  HashTable< size_t, std::string > hash_table;
  for (size_t i = 0; i <= 5; ++i)
  {
    hash_table.insert(std::make_pair(i, std::to_string(i)));
  }
  for (size_t i = 0; i <= 5; ++i)
  {
    BOOST_TEST(hash_table[i] == std::to_string(i));
  }
}

BOOST_AUTO_TEST_CASE(hash_table_const_operator_access_test)
{
  HashTable< size_t, std::string > hash_table;
  for (size_t i = 0; i <= 5; ++i)
  {
    hash_table.insert(std::make_pair(i, std::to_string(i)));
  }
  const HashTable< size_t, std::string >& chash_table = hash_table;
  for (size_t i = 0; i <= 5; ++i)
  {
    BOOST_TEST(chash_table[i] == std::to_string(i));
  }
}

BOOST_AUTO_TEST_CASE(init_list_contructor_test)
{
  HashTable< size_t, std::string > hash_table{std::make_pair(1, "first"), std::make_pair(2, "second"), std::make_pair(3, "third")};
  std::ostringstream out;
  print(hash_table, out);
  BOOST_TEST(out.str() == "1 first 2 second 3 third");
}

BOOST_AUTO_TEST_CASE(iter_contructor_test)
{
  HashTable< size_t, std::string > hash_table1{std::make_pair(1, "first"), std::make_pair(2, "second"), std::make_pair(3, "third")};
  HashTable< size_t, std::string > hash_table2{hash_table1.begin(), hash_table1.end()};
  std::ostringstream out;
  print(hash_table2, out);
  BOOST_TEST(out.str() == "1 first 2 second 3 third");
}

BOOST_AUTO_TEST_CASE(hash_insert_rvalue_test)
{
  HashTable< size_t, std::string > hash_table;
  hash_table.insert(std::make_pair(2, "second"));
  BOOST_TEST(!hash_table.empty());
  BOOST_TEST(hash_table.size() == 1);
  hash_table.insert(std::make_pair(1, "first"));
  BOOST_TEST(hash_table.size() == 2);
  auto it = hash_table.insert(std::make_pair(3, "third"));
  BOOST_TEST(it.second);
  auto it2 = hash_table.insert(std::make_pair(3, "third"));
  BOOST_TEST(!it2.second);
  BOOST_TEST(hash_table.size() == 3);
  std::ostringstream out;
  print(hash_table, out);
  BOOST_TEST(out.str() == "1 first 2 second 3 third");
}

BOOST_AUTO_TEST_CASE(hash_insert_lvalue_test)
{
  HashTable< size_t, std::string > hash_table;
  std::pair< size_t, std::string > pair = std::make_pair(2, "second");
  hash_table.insert(pair);
  BOOST_TEST(!hash_table.empty());
  BOOST_TEST(hash_table.size() == 1);
  pair = std::make_pair(1, "first");
  hash_table.insert(pair);
  BOOST_TEST(hash_table.size() == 2);
  pair = std::make_pair(3, "third");
  auto it = hash_table.insert(pair);
  BOOST_TEST(it.second);
  auto it2 = hash_table.insert(pair);
  BOOST_TEST(!it2.second);
  BOOST_TEST(hash_table.size() == 3);
  std::ostringstream out;
  print(hash_table, out);
  BOOST_TEST(out.str() == "1 first 2 second 3 third");
}

BOOST_AUTO_TEST_CASE(hash_insert_cit_lvalue_test)
{
  HashTable< size_t, std::string > hash_table;
  std::pair< size_t, std::string > pair = std::make_pair(2, "second");
  hash_table.insert(hash_table.cend(), pair);
  BOOST_TEST(!hash_table.empty());
  BOOST_TEST(hash_table.size() == 1);
  pair = std::make_pair(1, "first");
  hash_table.insert(pair);
  BOOST_TEST(hash_table.size() == 2);
  pair = std::make_pair(3, "third");
  hash_table.insert(hash_table.cbegin(), pair);
  hash_table.insert(hash_table.cbegin(), pair);
  BOOST_TEST(hash_table.size() == 3);
  std::ostringstream out;
  print(hash_table, out);
  BOOST_TEST(out.str() == "1 first 2 second 3 third");
}

BOOST_AUTO_TEST_CASE(hash_insert_cit_rvalue_test)
{
  HashTable< size_t, std::string > hash_table;
  hash_table.insert(hash_table.cend(), std::make_pair(2, "second"));
  BOOST_TEST(!hash_table.empty());
  BOOST_TEST(hash_table.size() == 1);
  hash_table.insert(std::make_pair(1, "first"));
  BOOST_TEST(hash_table.size() == 2);
  hash_table.insert(hash_table.cbegin(), std::make_pair(3, "third"));
  hash_table.insert(hash_table.cbegin(), std::make_pair(3, "third"));
  BOOST_TEST(hash_table.size() == 3);
  std::ostringstream out;
  print(hash_table, out);
  BOOST_TEST(out.str() == "1 first 2 second 3 third");
}


BOOST_AUTO_TEST_CASE(hash_insert_emplace_and_emplace_hint_test)
{
  HashTable< size_t, std::string > hash_table;
  std::pair< size_t, std::string > pair = std::make_pair(2, "second");
  hash_table.emplace_hint(hash_table.cend(), 2, "second");
  BOOST_TEST(!hash_table.empty());
  BOOST_TEST(hash_table.size() == 1);
  hash_table.emplace(1, "first");
  BOOST_TEST(hash_table.size() == 2);
  auto it = hash_table.emplace_hint(hash_table.cbegin(), 3, "third");
  BOOST_TEST((*it).first == 3);
  hash_table.emplace_hint(hash_table.cbegin(), 3, "third");
  BOOST_TEST(hash_table.size() == 3);
  std::ostringstream out;
  print(hash_table, out);
  BOOST_TEST(out.str() == "1 first 2 second 3 third");
}

BOOST_AUTO_TEST_CASE(hash_insert_first_last_test)
{
  HashTable< size_t, std::string > hash_table;
  hash_table.insert(hash_table.cend(), std::make_pair(2, "second"));
  hash_table.insert(std::make_pair(1, "first"));
  hash_table.insert(hash_table.cbegin(), std::make_pair(3, "third"));
  HashTable< size_t, std::string > hash_table2;
  hash_table2.insert(hash_table.begin(), hash_table.end());
  std::ostringstream out;
  print(hash_table, out);
  BOOST_TEST(out.str() == "1 first 2 second 3 third");
}

BOOST_AUTO_TEST_CASE(hash_clear_test)
{
  HashTable< size_t, std::string > hash_table;
  for (size_t i = 0; i <= 5; ++i)
  {
    hash_table.insert(std::make_pair(i, std::to_string(i)));
  }
  BOOST_TEST(hash_table.size() == 6);
  hash_table.clear();
  BOOST_TEST(hash_table.empty());
}

BOOST_AUTO_TEST_CASE(hash_find_test)
{
  HashTable< size_t, std::string > hash_table;
  for (size_t i = 0; i <= 5; ++i)
  {
    hash_table.insert(std::make_pair(i, std::to_string(i)));
  }
  BOOST_TEST((hash_table.find(6) == hash_table.end()));
  BOOST_TEST((hash_table.find(5) != hash_table.end()));
}

BOOST_AUTO_TEST_CASE(hash_const_find_test)
{
  HashTable< size_t, std::string > hash_table;
  for (size_t i = 0; i <= 5; ++i)
  {
    hash_table.insert(std::make_pair(i, std::to_string(i)));
  }
  const HashTable< size_t, std::string >& chash_table = hash_table;
  bool k1 = (chash_table.find(6) == chash_table.cend());
  BOOST_TEST(k1);
  bool k2 = (chash_table.find(5) != chash_table.cend());
  BOOST_TEST(k2);
}

BOOST_AUTO_TEST_CASE(hash_at_test)
{
  HashTable< size_t, std::string > hash_table;
  for (size_t i = 0; i <= 5; ++i)
  {
    hash_table.insert(std::make_pair(i, std::to_string(i)));
  }
  for (size_t i = 0; i <= 5; ++i)
  {
    BOOST_TEST(hash_table.at(i) == std::to_string(i));
  }
  try
  {
    BOOST_TEST(hash_table.at(6) == std::to_string(6));
  }
  catch (...)
  {
    BOOST_TEST(true);
  }
}

BOOST_AUTO_TEST_CASE(hash_at_const_test)
{
  HashTable< size_t, std::string > hash_table;
  for (size_t i = 0; i <= 5; ++i)
  {
    hash_table.insert(std::make_pair(i, std::to_string(i)));
  }
  const HashTable< size_t, std::string >& chash_table = hash_table;
  for (size_t i = 0; i <= 5; ++i)
  {
    BOOST_TEST(chash_table.at(i) == std::to_string(i));
  }
}

BOOST_AUTO_TEST_CASE(hash_table_clear_test)
{
  HashTable< size_t, std::string > hash_table;
  for (size_t i = 0; i <= 5; ++i)
  {
    hash_table.insert(std::make_pair(i, std::to_string(i)));
  }
  BOOST_TEST(hash_table.size() == 6);
  hash_table.clear();
  BOOST_TEST(hash_table.empty());
}

BOOST_AUTO_TEST_CASE(hash_swap_test)
{
  HashTable< size_t, std::string > hash_table;
  for (size_t i = 0; i <= 5; ++i)
  {
    hash_table.insert(std::make_pair(i, std::to_string(i)));
  }
  HashTable< size_t, std::string > hash_table2;
  BOOST_TEST(hash_table.size() == 6);
  hash_table.swap(hash_table2);
  std::ostringstream out;
  print(hash_table2, out);
  BOOST_TEST(out.str() == "0 0 1 1 2 2 3 3 4 4 5 5");
}

BOOST_AUTO_TEST_CASE(hash_begin_test)
{
  HashTable< size_t, std::string > hash_table;
  for (size_t i = 0; i <= 5; ++i)
  {
    hash_table.insert(std::make_pair(i, std::to_string(i)));
  }
  HashIterator< size_t, std::string > it = hash_table.begin();
  BOOST_TEST((it->first == 0 && it->second == "0"));
}

BOOST_AUTO_TEST_CASE(hash_cbegin_test)
{
  HashTable< size_t, std::string > hash_table;
  for (size_t i = 0; i <= 5; ++i)
  {
    hash_table.insert(std::make_pair(i, std::to_string(i)));
  }
  CHashIterator< size_t, std::string > it = hash_table.cbegin();
  BOOST_TEST((it->first == 0 && it->second == "0"));
}

BOOST_AUTO_TEST_CASE(hash_erase_key_test)
{
  HashTable< size_t, std::string > hash_table;
  for (size_t i = 0; i <= 5; ++i)
  {
    hash_table.insert(std::make_pair(i, std::to_string(i)));
  }
  size_t count = hash_table.erase(2);
  std::ostringstream out;
  print(hash_table, out);
  BOOST_TEST(out.str() == "0 0 1 1 3 3 4 4 5 5");
  BOOST_TEST(count == 1);
  BOOST_TEST(hash_table.size() == 5);
}

BOOST_AUTO_TEST_CASE(hash_erase_it_test)
{
  HashTable< size_t, std::string > hash_table;
  for (size_t i = 0; i <= 5; ++i)
  {
    hash_table.insert(std::make_pair(i, std::to_string(i)));
  }
  auto it2 = ++hash_table.begin();
  it2++;
  auto it = hash_table.erase(it2);
  std::ostringstream out;
  print(hash_table, out);
  BOOST_TEST(out.str() == "0 0 1 1 3 3 4 4 5 5");
  BOOST_TEST((it->first == 3 && it->second == "3"));
  BOOST_TEST(hash_table.size() == 5);
}

BOOST_AUTO_TEST_CASE(hash_erase_cit_test)
{
  HashTable< size_t, std::string > hash_table;
  for (size_t i = 0; i <= 5; ++i)
  {
    hash_table.insert(std::make_pair(i, std::to_string(i)));
  }
  auto it2 = ++hash_table.cbegin();
  it2++;
  auto it = hash_table.erase(it2);
  std::ostringstream out;
  print(hash_table, out);
  BOOST_TEST(out.str() == "0 0 1 1 3 3 4 4 5 5");
  BOOST_TEST((it->first == 3 && it->second == "3"));
  BOOST_TEST(hash_table.size() == 5);
}

BOOST_AUTO_TEST_CASE(hash_table_erase_range_test)
{
  HashTable< size_t, std::string > hash_table1{std::make_pair(1, "first"), std::make_pair(2, "second"), std::make_pair(3, "third")};
  hash_table1.erase(hash_table1.cbegin(), ++hash_table1.cbegin());
  std::ostringstream out;
  print(hash_table1, out);
  BOOST_TEST(out.str() == "2 second 3 third");
}

BOOST_AUTO_TEST_CASE(hash_end_test)
{
  HashTable< size_t, std::string > hash_table;
  for (size_t i = 0; i <= 5; ++i)
  {
    hash_table.insert(std::make_pair(i, std::to_string(i)));
  }
  HashIterator< size_t, std::string > it2 = hash_table.end();
  size_t i = 0;
  for (auto it = hash_table.begin(); it != it2; ++i)
  {
    auto it3 = it;
    if (++it3 == it2)
    {
      BOOST_TEST((it->first == 5 && it->second == "5"));
    }
    it++;
  }
  BOOST_TEST(i == 6);
}

BOOST_AUTO_TEST_CASE(hash_cend_test)
{
  HashTable< size_t, std::string > hash_table;
  for (size_t i = 0; i <= 5; ++i)
  {
    hash_table.insert(std::make_pair(i, std::to_string(i)));
  }
  CHashIterator< size_t, std::string > it2 = hash_table.cend();
  size_t i = 0;
  for (auto it = hash_table.cbegin(); it != it2; ++i)
  {
    auto it3 = it;
    if (++it3 == it2)
    {
      BOOST_TEST((it->first == 5 && it->second == "5"));
    }
    it++;
  }
  BOOST_TEST(i == 6);
}

BOOST_AUTO_TEST_CASE(hash_load_factor_test)
{
  HashTable< size_t, std::string > hash_table(12);
  for (size_t i = 0; i <= 5; ++i)
  {
    hash_table.insert(std::make_pair(i, std::to_string(i)));
  }
  hash_table.maxLoadFactor(0.8);
  BOOST_TEST(std::fabs((hash_table.maxLoadFactor() - 0.8)) < 0.1e-6);
  BOOST_TEST(std::fabs((hash_table.load_factor() - 0.5)) < 0.1e-6);
}
