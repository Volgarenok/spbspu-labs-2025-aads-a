#include <boost/test/unit_test.hpp>
#include <vector>
#include "tree.hpp"

using aleksandrov::Tree;

BOOST_AUTO_TEST_CASE(default_construction)
{
  Tree< int, char > tree;
  BOOST_TEST(tree.empty());
}

BOOST_AUTO_TEST_CASE(copy_construction)
{
  Tree< int, char > t1;
  t1.insert({1, 'a'});
  t1.insert({2, 'b'});
  t1.insert({3, 'c'});
  Tree< int, char > t2(t1);
  BOOST_TEST(t2[3] == 'c');
}

BOOST_AUTO_TEST_CASE(move_construction)
{
  Tree< int, char > t1;
  t1.insert({1, 'a'});
  t1.insert({2, 'b'});
  t1.insert({3, 'c'});
  Tree< int, char > t2(std::move(t1));
  BOOST_TEST(t2[3] == 'c');
  BOOST_TEST(t1.empty());
}

BOOST_AUTO_TEST_CASE(copy_assignment)
{
  Tree< int, char > t1;
  t1.insert({1, 'a'});
  t1.insert({2, 'b'});
  t1.insert({3, 'c'});
  Tree< int, char > t2;
  t2 = t1;
  BOOST_TEST(t2[3] == 'c');
}

BOOST_AUTO_TEST_CASE(move_assignment)
{
  Tree< int, char > t1;
  t1.insert({1, 'a'});
  t1.insert({2, 'b'});
  t1.insert({3, 'c'});
  Tree< int, char > t2;
  t2 = std::move(t1);
  BOOST_TEST(t2[3] == 'c');
  BOOST_TEST(t1.empty());
}

BOOST_AUTO_TEST_CASE(begin)
{
  Tree< int, char > tree;
  tree.insert({1, 'a'});
  BOOST_TEST((*tree.begin()).first == 1);
  tree.insert({2, 'b'});
  auto it = tree.begin();
  BOOST_TEST((*it).first == 1);
  ++it;
  BOOST_TEST((*it).first == 2);
}

BOOST_AUTO_TEST_CASE(element_access)
{
  Tree< int, char > tree;
  tree.insert({1, 'a'});
  BOOST_TEST(tree[1] == 'a');
  BOOST_TEST(tree.at(1) == 'a');
  tree.insert({2, 'b'});
  BOOST_TEST(tree[2] == 'b');
  BOOST_TEST(tree.at(2) == 'b');
}

BOOST_AUTO_TEST_CASE(size_empty)
{
  Tree< int, char > tree;
  BOOST_TEST(tree.empty());
  BOOST_TEST(tree.size() == 0);
  tree.insert({0, '\0'});
  BOOST_TEST(!tree.empty());
  BOOST_TEST(tree.size() == 1);
  tree.insert({1, '\0'});
  BOOST_TEST(!tree.empty());
  BOOST_TEST(tree.size() == 2);
}

BOOST_AUTO_TEST_CASE(insert)
{
  Tree< int, char > tree;
  tree.insert({12, 'a'});
  tree.insert({1, 'b'});
  tree.insert({13, 'c'});
  tree.insert({14, 'd'});
  tree.insert({15, 'e'});
  tree.insert({6, 'f'});
  tree.insert({9, 'g'});
  tree.insert({4, 'h'});
  tree.insert({5, 'i'});
  tree.insert({7, 'j'});
  tree.insert({2, 'k'});
  tree.insert({3, 'l'});
  tree.insert({8, 'm'});
  tree.insert({10, 'n'});
  tree.insert({11, 'o'});

  std::vector< int > expected;
  expected = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
  struct Pred
  {
    using Pair = std::pair< int, char >;
    bool operator()(const Pair& a, int b)
    {
      return a.first == b;
    }
  };
  BOOST_TEST(std::equal(tree.begin(), tree.end(), expected.begin(), Pred{}));
}

BOOST_AUTO_TEST_CASE(erase)
{
  Tree< int, char > tree;
  std::vector< int > keys;
  keys = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 5, 15, 25, 8};
  for (size_t i = 0; i < keys.size(); ++i)
  {
    tree.insert({keys[i], '0'});
  }
  tree.erase(tree.find(50));
  tree.erase(tree.find(20));
  tree.erase(tree.find(15));
  tree.erase(tree.find(10));
  tree.erase(tree.find(80));
  BOOST_TEST(tree.size() == 14);
}

BOOST_AUTO_TEST_CASE(insert_erase)
{
  Tree< int, char > tree;
  std::vector< int > keys = {1, 5, 6, 8, 7, 2, 4, 9, 10, 3, 11, 12, 13, 14, 15};
  for (size_t i = 0; i < keys.size(); ++i)
  {
    tree.insert({keys[i], '0'});
  }
  for (size_t i = 0; i < keys.size(); ++i)
  {
    tree.erase(tree.find(keys[i]));
  }
}

BOOST_AUTO_TEST_CASE(clear)
{
  Tree< int, char > tree;
  tree.clear();
  tree.insert({1, 'a'});
  tree.insert({2, 'b'});
  tree.insert({3, 'c'});
  tree.clear();
  tree.clear();
  BOOST_TEST(tree.empty());
}

BOOST_AUTO_TEST_CASE(swap)
{
  Tree< int, char > t1;
  Tree< int, char > t2;
  t2.swap(t1);
  t1.swap(t2);
  t1.insert({1, 'a'});
  t1.insert({2, 'b'});
  t2.insert({3, 'c'});
  t1.swap(t2);
  BOOST_TEST(t1.size() == 1);
  BOOST_TEST(t1[3] == 'c');
  t1.swap(t2);
  BOOST_TEST(t2.size() == 1);
  BOOST_TEST(t2[3] == 'c');
}

BOOST_AUTO_TEST_CASE(count)
{
  Tree< int, char > tree;
  BOOST_TEST(tree.count(1) == 0);
  tree.insert({1, 'a'});
  tree.insert({2, 'b'});
  BOOST_TEST(tree.count(1) == 1);
  BOOST_TEST(tree.count(3) == 0);
}

BOOST_AUTO_TEST_CASE(find)
{
  Tree< int, char > tree;
  auto it = tree.find(1);
  bool isNullptr = it == tree.end();
  BOOST_TEST(isNullptr);
  tree.insert({1, 'a'});
  BOOST_TEST((*tree.find(1)).second == 'a');
  tree.insert({2, 'b'});
  BOOST_TEST((*tree.find(2)).second == 'b');
  tree.insert({3, 'c'});
  BOOST_TEST((*tree.find(3)).second == 'c');
  tree.insert({4, 'd'});
  BOOST_TEST((*tree.find(4)).second == 'd');
  tree.insert({5, 'e'});
  BOOST_TEST((*tree.find(5)).second == 'e');
  tree.insert({6, 'f'});
  BOOST_TEST((*tree.find(6)).second == 'f');
}

