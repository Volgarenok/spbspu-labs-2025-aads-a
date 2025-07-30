#include <boost/test/unit_test.hpp>
#include <numeric>
#include <vector>
#include <tree/tree.hpp>

using aleksandrov::Tree;

BOOST_AUTO_TEST_CASE(traverse_LNR)
{
  Tree< int, int > tree;
  std::vector< int > keys;
  keys = { 1, 4, 7, 9, 10, 12, 13, 11, 14, 3, 6, 2, 5, 8 };
  for (size_t i = 0; i < keys.size(); ++i)
  {
    tree.insert({ keys[i], 0 });
  }
  std::vector< int > expected(keys.size());
  std::iota(expected.begin(), expected.end(), 1);

  std::vector< int > result;
  for (auto it = tree.beginLNR(); it != tree.endLNR(); ++it)
  {
    result.push_back(it->first);
  }
  BOOST_TEST(std::equal(result.begin(), result.end(), expected.begin()));
}

BOOST_AUTO_TEST_CASE(traverse_RNL)
{
  Tree< int, int > tree;
  std::vector< int > keys;
  keys = { 1, 4, 7, 9, 10, 12, 13, 11, 14, 3, 6, 2, 5, 8 };
  for (size_t i = 0; i < keys.size(); ++i)
  {
    tree.insert({ keys[i], 0 });
  }
  std::vector< int > expected(keys.size());
  std::iota(expected.begin(), expected.end(), 1);
  std::reverse(expected.begin(), expected.end());

  std::vector< int > result;
  for (auto it = tree.beginRNL(); it != tree.endRNL(); ++it)
  {
    result.push_back(it->first);
  }
  BOOST_TEST(std::equal(result.begin(), result.end(), expected.begin()));
}

BOOST_AUTO_TEST_CASE(traverse_BFS)
{
  Tree< int, int > tree;
  std::vector< int > keys;
  keys = { 1, 3, 7, 9, 10, 12, 13, 11, 14, 2, 6, 4, 5, 8 };
  for (size_t i = 0; i < keys.size(); ++i)
  {
    tree.insert({ keys[i], 0 });
  }
  std::vector< int > expected(keys.size());
  expected = { 9, 3, 6, 12, 1, 2, 4, 5, 7, 8, 10, 11, 13, 14 };

  std::vector< int > result;
  for (auto it = tree.beginBFS(); it != tree.endBFS(); ++it)
  {
    result.push_back(it->first);
  }
  BOOST_TEST(std::equal(result.begin(), result.end(), expected.begin()));
}

