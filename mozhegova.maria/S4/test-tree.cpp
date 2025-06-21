#include <boost/test/unit_test.hpp>
#include <tree.hpp>

namespace
{
  template< typename Key, typename T, typename Cmp >
  void printTree(std::ostream & out, mozhegova::BiTree< Key, T, Cmp > tree)
  {
    for (auto it = tree.cbegin(); it != tree.cend(); ++it)
    {
      out << it->second;
    }
  }
}

BOOST_AUTO_TEST_CASE(constructors)
{
  mozhegova::BiTree< size_t, std::string > tree1;
  BOOST_TEST(tree1.empty());
  BOOST_TEST(tree1.size() == 0);

  mozhegova::BiTree< size_t, std::string > tree2{ {1, "1"}, {2, "2"}, {4, "4"}, {3, "3"} };
  std::ostringstream out1;
  printTree(out1, tree2);
  BOOST_TEST(out1.str() == "1234");
  BOOST_TEST(tree2.size() == 4);

  mozhegova::BiTree< size_t, std::string > tree3(++tree2.begin(), tree2.end());
  std::ostringstream out2;
  printTree(out2, tree3);
  BOOST_TEST(out2.str() == "234");
}

BOOST_AUTO_TEST_CASE(modifiers)
{
  mozhegova::BiTree< size_t, std::string > tree1;
  tree1.insert({1, "1"});
  tree1.insert({5, "5"});
  tree1.insert({3, "3"});
  tree1.insert({8, "8"});
  tree1.insert({2, "2"});
  std::ostringstream out1;
  printTree(out1, tree1);
  BOOST_TEST(out1.str() == "12358");

  mozhegova::BiTree< size_t, std::string > tree2{ {4, "4"}, {2, "2"}, {6, "6"}, {3, "3"} };
  tree1.insert(tree2.begin(), tree2.end());
  std::ostringstream out2;
  printTree(out2, tree1);
  BOOST_TEST(out2.str() == "1234568");

  tree2.erase(tree2.begin());
  std::ostringstream out3;
  printTree(out3, tree2);
  BOOST_TEST(out3.str() == "346");

  tree1.erase(3);
  std::ostringstream out4;
  printTree(out4, tree1);
  BOOST_TEST(out4.str() == "124568");

  auto it = tree1.begin();
  std::advance(it, 3);
  tree1.erase(tree1.begin(), it);
  std::ostringstream out5;
  printTree(out5, tree1);
  BOOST_TEST(out5.str() == "568");

  tree1.swap(tree2);
  std::ostringstream out6;
  printTree(out6, tree1);
  BOOST_TEST(out6.str() == "346");
  std::ostringstream out7;
  printTree(out7, tree2);
  BOOST_TEST(out7.str() == "568");

  tree1.clear();
  BOOST_TEST(tree1.empty());
}

BOOST_AUTO_TEST_CASE(element_access)
{
  mozhegova::BiTree< size_t, std::string > tree1;
  tree1[1] = "1";
  tree1[3] = "3";
  tree1.insert({2, "2"});
  BOOST_TEST(tree1[1] == "1");
  BOOST_TEST(tree1[2] == "2");
  BOOST_TEST(tree1[3] == "3");
  BOOST_TEST(tree1.at(2) == "2");
}

BOOST_AUTO_TEST_CASE(operations)
{
  mozhegova::BiTree< size_t, std::string > tree1;
  tree1.insert({1, "1"});
  tree1.insert({2, "2"});
  tree1.insert({5, "5"});
  tree1.insert({10, "10"});
  tree1.insert({13, "13"});
  auto it = tree1.find(2);
  BOOST_TEST(it->first == 2);
  BOOST_TEST(it->second == "2");

  it = tree1.lower_bound(5);
  BOOST_TEST(it->first == 5);
  it = tree1.lower_bound(7);
  BOOST_TEST(it->first == 10);

  it = tree1.upper_bound(5);
  BOOST_TEST(it->first == 10);

  auto itpair = tree1.equal_range(10);
  BOOST_TEST(itpair.first->first == 10);
  BOOST_TEST(itpair.second->first == 13);

  BOOST_TEST(tree1.count(1) == 1);
  BOOST_TEST(tree1.count(7) == 0);
}
