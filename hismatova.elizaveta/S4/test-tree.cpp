#include <boost/test/unit_test.hpp>
#include <unordered_map>
#include "BinarySeachTree.hpp"
#include "handle_functions.hpp"

BOOST_AUTO_TEST_CASE(BinarySeachTreeTest_Print)
{
  std::unordered_map< std::string, BinarySearchTree< int, std::string > > datasets;
  BinarySearchTree< int, std::string > tree1;
  tree1.insert({1, "one"});
  tree1.insert({2, "two"});
  tree1.insert({3, "three"});
  datasets["dataset1"] = std::move(tree1);
  std::stringstream output;
  std::streambuf* old = std::cout.rdbuf(output.rdbuf());
  handle_print(datasets, "dataset1");
  BOOST_CHECK(output.str() == "1 one\n2 two\n3 three\n");
  datasets["dataset2"] = BinarySearchTree< int, std::string >();
  output.str("");
  handle_print(datasets, "dataset2");
  BOOST_CHECK(output.str() == "<EMPTY>\n");
  std::cout.rdbuf(old);
}

BOOST_AUTO_TEST_CASE(BinarySeachTreeTest_Complement)
{
  std::unordered_map< std::string, BinarySearchTree< int, std::string > > datasets;
  BinarySearchTree< int, std::string > tree1;
  tree1.insert({1, "one"});
  tree1.insert({2, "two"});
  tree1.insert({3, "three"});
  datasets["dataset1"] = std::move(tree1);
  BinarySearchTree< int, std::string > tree2;
  tree2.insert({2, "two"});
  tree2.insert({3, "three"});
  tree2.insert({4, "four"});
  datasets["dataset2"] = std::move(tree2);
  handle_complement(datasets, "complementResult", "dataset1", "dataset2");
  const auto& resultTree = datasets["complementResult"];
  std::stringstream output;
  for (const auto& pair: resultTree)
  {
    const auto key = pair.first;
    const auto value = pair.second;
    output << key << " " << value << "\n";
  }
  BOOST_CHECK(output.str() == "1 one\n");
}

BOOST_AUTO_TEST_CASE(BinarySeachTreeTest_Intersect)
{
  std::unordered_map< std::string, BinarySearchTree< int, std::string > > datasets;
  BinarySearchTree< int, std::string > tree1;
  tree1.insert({1, "one"});
  tree1.insert({2, "two"});
  tree1.insert({3, "three"});
  datasets["dataset1"] = std::move(tree1);
  BinarySearchTree< int, std::string > tree2;
  tree2.insert({2, "two"});
  tree2.insert({3, "three"});
  tree2.insert({4, "four"});
  datasets["dataset2"] = std::move(tree2);
  handle_intersect(datasets, "intersectResult", "dataset1", "dataset2");
  const auto& resultTree = datasets["intersectResult"];
  std::stringstream output;
  for (const auto& pair: resultTree)
  {
    const auto key = pair.first;
    const auto value = pair.second;
    output << key << " " << value << "\n";
  }
  BOOST_CHECK(output.str() == "2 two\n3 three\n");
}

BOOST_AUTO_TEST_CASE(BinarySeachTreeTest_Union)
{
  std::unordered_map< std::string, BinarySearchTree< int, std::string > > datasets;
  BinarySearchTree< int, std::string > tree1;
  tree1.insert({1, "one"});
  tree1.insert({2, "two"});
  datasets["dataset1"] = std::move(tree1);
  BinarySearchTree< int, std::string > tree2;
  tree2.insert({2, "two"});
  tree2.insert({3,"three"});
  datasets["dataset2"] = std::move(tree2);
  const auto& resultTree = datasets["unionResult"];
  std::stringstream output;
  for (const auto& pair: resultTree)
  {
    const auto key = pair.first;
    const auto value = pair.second;
    output << key << " " << value << "\n";
  }
  BOOST_CHECK(output.str() == "1 one\n2 two\n3 three\n");
}
