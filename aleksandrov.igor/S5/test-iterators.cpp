#include <boost/test/unit_test.hpp>
#include <numeric>
#include <vector>
#include "../common/tree/tree.hpp"

using aleksandrov::Tree;

BOOST_AUTO_TEST_SUITE(tree_traverses);

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

  std::vector< int > res;
  for (auto it = tree.beginLNR(); it != tree.endLNR(); ++it)
  {
    res.push_back(it->first);
  }
  BOOST_TEST(std::equal(res.begin(), res.end(), expected.begin()));

  res.clear();
  std::reverse(expected.begin(), expected.end());
  for (auto it = tree.rbeginLNR(); it != tree.rendLNR(); ++it)
  {
    res.push_back(it->first);
  }
  BOOST_TEST(std::equal(res.begin(), res.end(), expected.begin()));
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

  std::vector< int > res;
  for (auto it = tree.beginRNL(); it != tree.endRNL(); ++it)
  {
    res.push_back(it->first);
  }
  BOOST_TEST(std::equal(res.begin(), res.end(), expected.begin()));

  res.clear();
  std::reverse(expected.begin(), expected.end());
  for (auto it = tree.rbeginRNL(); it != tree.rendRNL(); ++it)
  {
    res.push_back(it->first);
  }
  BOOST_TEST(std::equal(res.begin(), res.end(), expected.begin()));
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

  std::vector< int > res;
  for (auto it = tree.beginBFS(); it != tree.endBFS(); ++it)
  {
    res.push_back(it->first);
  }
  BOOST_TEST(std::equal(res.begin(), res.end(), expected.begin()));

  res.clear();
  std::reverse(expected.begin(), expected.end());
  for (auto it = tree.rbeginBFS(); it != tree.rendBFS(); ++it)
  {
    res.push_back(it->first);
  }
  BOOST_TEST(std::equal(res.begin(), res.end(), expected.begin()));
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(iterators_conversion);

BOOST_AUTO_TEST_CASE(heavy_to_lite)
{
  Tree< int, int > tree;
  tree = { { 1, 1 }, { 2, 2 }, { 3, 3 }, { 4, 4 }, { 5, 5 } };

  auto heavy1 = tree.beginLNR();
  auto lite1 = heavy1.makeLite();
  BOOST_CHECK(heavy1 == lite1);

  auto heavy2 = std::next(tree.beginLNR(), 3);
  auto lite2 = heavy2.makeLite();
  BOOST_CHECK(heavy2 == lite2);

  auto heavy3 = tree.beginRNL();
  auto lite3 = heavy3.makeLite();
  BOOST_CHECK(heavy3 == lite3);

  auto heavy4 = std::next(tree.beginRNL(), 3);
  auto lite4 = heavy4.makeLite();
  BOOST_CHECK(heavy4 == lite4);

  auto heavy5 = tree.beginBFS();
  auto lite5 = heavy5.makeLite();
  BOOST_CHECK(heavy5 == lite5);

  auto heavy6 = std::next(tree.beginBFS(), 3);
  auto lite6 = heavy6.makeLite();
  BOOST_CHECK(heavy6 == lite6);
}

BOOST_AUTO_TEST_CASE(lite_to_heavy)
{
  Tree< int, int > tree;
  tree = { { 1, 1 }, { 2, 2 }, { 3, 3 }, { 4, 4 }, { 5, 5 } };

  auto lite1 = tree.begin();
  auto heavy1 = lite1.makeLNR();
  BOOST_CHECK(lite1 == heavy1);

  auto lite2 = std::next(tree.begin(), 3);
  auto heavy2 = lite2.makeLNR();
  BOOST_CHECK(lite2 == heavy2);

  auto lite3 = tree.begin();
  auto heavy3 = lite3.makeRNL();
  BOOST_CHECK(lite3 == heavy3);

  auto lite4 = std::next(tree.begin(), 3);
  auto heavy4 = lite2.makeRNL();
  BOOST_CHECK(lite4 == heavy4);

  auto lite5 = tree.begin();
  auto heavy5 = lite3.makeBFS();
  BOOST_CHECK(lite5 == heavy5);

  auto lite6 = std::next(tree.begin(), 3);
  auto heavy6 = lite2.makeBFS();
  BOOST_CHECK(lite6 == heavy6);
}

BOOST_AUTO_TEST_SUITE_END();

