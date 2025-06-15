#include <boost/test/unit_test.hpp>
#include <sstream>
#include <string>
#include <AVLtree.hpp>

namespace
{
  template< class Key, class Value, class Cmp >
  void printTree(std::ostream& out, finaev::AVLtree< Key, Value, Cmp > tree)
  {
    for (auto it = tree.cBegin(); it != tree.cEnd(); ++it)
    {
      out << it->second;
    }
  }
}

BOOST_AUTO_TEST_CASE(tree_constructors)
{
  finaev::AVLtree< std::string, int > tree;
  BOOST_TEST(tree.empty());
  BOOST_TEST(tree.size() == 0);
  finaev::AVLtree< std::string, int > tree2;
  tree2.insert({"1", 1});
  tree2.insert({"2", 2});
  tree2.insert({"3", 3});
  tree2.insert({"4", 4});
  std::ostringstream out1;
  printTree(out1, tree2);
  BOOST_TEST(out1.str() == "1234");
  BOOST_TEST(tree2.size() == 4);
}

BOOST_AUTO_TEST_CASE(tree_operations)
{
  finaev::AVLtree< std::string, int > tree;
  tree.insert({"1", 1});
  tree.insert({"2", 2});
  tree.insert({"3", 3});
  auto it = tree.find("2");
  BOOST_REQUIRE(it != tree.end());
  BOOST_TEST(it->first == "2");
  BOOST_TEST(it->second == 2);

  BOOST_TEST(tree.size() == 3);

  BOOST_CHECK_EQUAL(tree["1"], 1);
  BOOST_CHECK_EQUAL(tree["3"], 3);
  tree["4"] = 4;
  BOOST_CHECK_EQUAL(tree["4"], 4);

  BOOST_CHECK_EQUAL(tree["1"], tree.at("1"));
  BOOST_CHECK_EQUAL(tree.at("1"), 1);
  BOOST_CHECK_EQUAL(tree.at("2"), 2);
  BOOST_CHECK_THROW(tree.at("2134"), std::out_of_range);
}
