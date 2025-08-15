#include <boost/test/unit_test.hpp>
#include <vector>
#include "../common/tree/tree.hpp"

using aleksandrov::Tree;

namespace
{
  const std::vector< int > keys = { 1, 4, 7, 9, 10, 12, 13, 11, 14, 3, 6, 2, 5, 8 };

  void insertKeys(Tree< int, int >& tree)
  {
    for (size_t i = 0; i < keys.size(); ++i)
    {
      tree.insert({ keys[i], 0 });
    }
  }
}

BOOST_AUTO_TEST_SUITE(tree_traverses);

BOOST_AUTO_TEST_CASE(forward_LNR)
{
  Tree< int, int > tree;
  insertKeys(tree);
  std::vector< int > expected = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };

  std::vector< int > res;
  for (auto it = tree.beginLNR(); it != tree.endLNR(); ++it)
  {
    res.push_back(it->first);
  }
  BOOST_TEST(std::equal(res.begin(), res.end(), expected.begin()));
}

BOOST_AUTO_TEST_CASE(backward_LNR)
{
  Tree< int, int > tree;
  insertKeys(tree);
  std::vector< int > expected = { 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };

  std::vector< int > res;
  for (auto it = tree.rbeginLNR(); it != tree.rendLNR(); ++it)
  {
    res.push_back(it->first);
  }
  BOOST_TEST(std::equal(res.begin(), res.end(), expected.begin()));
}

BOOST_AUTO_TEST_CASE(random_access_LNR)
{
  Tree< int, int > tree;
  insertKeys(tree);

  auto it = tree.beginLNR();
  BOOST_CHECK(it != tree.endLNR());
  BOOST_TEST(it->first == 1);

  std::advance(it, 5);
  BOOST_TEST(it->first == 6);

  std::advance(it, -2);
  BOOST_TEST(it->first == 4);

  while (it->first != 14)
  {
    ++it;
  }
  BOOST_TEST(it->first == 14);

  ++it;
  BOOST_CHECK(it == tree.endLNR());
}

BOOST_AUTO_TEST_CASE(forward_RNL)
{
  Tree< int, int > tree;
  insertKeys(tree);
  std::vector< int > expected = { 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };

  std::vector< int > res;
  for (auto it = tree.beginRNL(); it != tree.endRNL(); ++it)
  {
    res.push_back(it->first);
  }
  BOOST_TEST(std::equal(res.begin(), res.end(), expected.begin()));
}

BOOST_AUTO_TEST_CASE(backward_RNL)
{
  Tree< int, int > tree;
  insertKeys(tree);
  std::vector< int > expected = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };

  std::vector< int > res;
  for (auto it = tree.rbeginRNL(); it != tree.rendRNL(); ++it)
  {
    res.push_back(it->first);
  }
  BOOST_TEST(std::equal(res.begin(), res.end(), expected.begin()));
}

BOOST_AUTO_TEST_CASE(random_access_RNL)
{
  Tree< int, int > tree;
  insertKeys(tree);

  auto it = tree.beginRNL();
  BOOST_CHECK(it != tree.endRNL());
  BOOST_TEST(it->first == 14);

  std::advance(it, 5);
  BOOST_TEST(it->first == 9);

  std::advance(it, -2);
  BOOST_TEST(it->first == 11);

  while (it->first != 1)
  {
    ++it;
  }
  BOOST_TEST(it->first == 1);

  ++it;
  BOOST_CHECK(it == tree.endRNL());
}

BOOST_AUTO_TEST_CASE(forward_BFS)
{
  Tree< int, int > tree;
  insertKeys(tree);
  std::vector< int > expected = { 4, 9, 2, 6, 12, 1, 3, 5, 7, 8, 10, 11, 13, 14 };

  std::vector< int > res;
  for (auto it = tree.beginBFS(); it != tree.endBFS(); ++it)
  {
    res.push_back(it->first);
  }
  BOOST_TEST(std::equal(res.begin(), res.end(), expected.begin()));
}

BOOST_AUTO_TEST_CASE(backward_BFS)
{
  Tree< int, int > tree;
  insertKeys(tree);
  std::vector< int > expected = { 14, 13, 11, 10, 8, 7, 5, 3, 1, 12, 6, 2, 9, 4 };

  std::vector< int > res;
  for (auto it = tree.rbeginBFS(); it != tree.rendBFS(); ++it)
  {
    res.push_back(it->first);
  }
  BOOST_TEST(std::equal(res.begin(), res.end(), expected.begin()));
}

BOOST_AUTO_TEST_CASE(random_access_BFS)
{
  Tree< int, int > tree;
  insertKeys(tree);

  auto it = tree.beginBFS();
  BOOST_CHECK(it != tree.endBFS());
  BOOST_TEST(it->first == 4);

  std::advance(it, 5);
  BOOST_TEST(it->first == 1);

  std::advance(it, -2);
  BOOST_TEST(it->first == 6);

  while (it->first != 14)
  {
    ++it;
  }
  BOOST_TEST(it->first == 14);

  ++it;
  BOOST_CHECK(it == tree.endBFS());
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(iterators_conversion);

BOOST_AUTO_TEST_CASE(heavy_to_lite)
{
  Tree< int, int > empty;

  auto emptyLiteEnd = empty.end();
  auto emptyHeavyEnd = emptyLiteEnd.makeLNR();
  BOOST_CHECK(emptyLiteEnd == emptyHeavyEnd);
  BOOST_CHECK(emptyHeavyEnd == empty.endLNR());

  Tree< int, int > tree = { { 1, 0 }, { 2, 0 }, { 3, 0 }, { 4, 0 }, { 5, 0 } };

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

  auto heavyEnd = tree.endLNR();
  auto liteEnd = heavyEnd.makeLite();
  BOOST_CHECK(heavyEnd == liteEnd);

  auto heavyRBegin = tree.rbeginBFS();
  auto liteRBegin = heavyRBegin.makeLite();
  BOOST_CHECK(heavyRBegin == liteRBegin);
}

BOOST_AUTO_TEST_CASE(lite_to_heavy)
{
  Tree< int, int > empty;

  auto emptyHeavyEnd = empty.endBFS();
  auto emptyLiteEnd = emptyHeavyEnd.makeLite();
  BOOST_CHECK(emptyHeavyEnd == emptyLiteEnd);
  BOOST_CHECK(emptyLiteEnd == empty.end());

  Tree< int, int > tree = { { 1, 0 }, { 2, 0 }, { 3, 0 }, { 4, 0 }, { 5, 0 } };

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

  auto liteEnd = tree.end();
  auto heavyEnd = liteEnd.makeRNL();
  BOOST_CHECK(liteEnd == heavyEnd);
}

BOOST_AUTO_TEST_SUITE_END();

