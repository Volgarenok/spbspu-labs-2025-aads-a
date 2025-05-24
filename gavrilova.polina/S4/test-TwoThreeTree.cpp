#include <boost/test/unit_test.hpp>
#include "TwoThreeTree.hpp"
#include <string>


BOOST_AUTO_TEST_CASE(TestEmptyTree)
{

  gavrilova::TwoThreeTree< int, std::string > tree;
  BOOST_TEST(tree.empty());
  BOOST_TEST(tree.size() == 0);
  BOOST_TEST(tree.find(42) == nullptr);
}

BOOST_AUTO_TEST_CASE(TestInsertAndFind)
{
  gavrilova::TwoThreeTree< int, std::string > tree;

  tree.push(10, "ten");
  BOOST_TEST(!tree.empty());
  BOOST_TEST(tree.size() == 1);
  BOOST_TEST(tree.find(10) != nullptr);
  BOOST_TEST(tree.get(10) == "ten");

  tree.push(5, "five");
  BOOST_TEST(tree.size() == 2);
  BOOST_TEST(tree.find(5) != nullptr);
  BOOST_TEST(tree.get(5) == "five");

  tree.push(15, "fifteen");
  BOOST_TEST(tree.size() == 3);
  BOOST_TEST(tree.find(15) != nullptr);
  BOOST_TEST(tree.get(15) == "fifteen");

  tree.push(20, "twenty");
  BOOST_TEST(tree.size() == 4);
  BOOST_TEST(tree.find(20) != nullptr);
  BOOST_TEST(tree.get(20) == "twenty");
}

BOOST_AUTO_TEST_CASE(TestGetNonExistentKey)
{
  gavrilova::TwoThreeTree< int, std::string > tree;
  tree.push(10, "ten");
  BOOST_CHECK_THROW(tree.get(42), std::logic_error);
}

BOOST_AUTO_TEST_CASE(TestInsertMultipleElements)
{
  gavrilova::TwoThreeTree< int, std::string > tree;
  const int count = 5;

  for (int i = 0; i < count; ++i) {
    tree.push(i, "value_" + std::to_string(i));
  }

  BOOST_TEST(tree.size() == count);

  for (int i = 0; i < count; ++i) {
    BOOST_TEST(tree.find(i) != nullptr);
    BOOST_TEST(tree.get(i) == "value_" + std::to_string(i));
  }
}

// BOOST_AUTO_TEST_CASE(TestCopyConstructor)
// {
//   gavrilova::TwoThreeTree< int, std::string > tree1;
//   tree1.push(10, "ten");
//   tree1.push(5, "five");
//   tree1.push(15, "fifteen");

//   gavrilova::TwoThreeTree< int, std::string > tree2(tree1);

//   BOOST_TEST(tree1.size() == tree2.size());
//   BOOST_TEST(tree2.find(10) != nullptr);
//   BOOST_TEST(tree2.get(10) == "ten");
//   BOOST_TEST(tree2.find(5) != nullptr);
//   BOOST_TEST(tree2.get(5) == "five");
// }

BOOST_AUTO_TEST_CASE(TestMoveConstructor)
{
  gavrilova::TwoThreeTree< int, std::string > tree1;
  tree1.push(10, "ten");
  tree1.push(5, "five");

  gavrilova::TwoThreeTree< int, std::string > tree2(std::move(tree1));

  BOOST_TEST(tree1.empty());
  BOOST_TEST(tree2.size() == 2);
  BOOST_TEST(tree2.find(10) != nullptr);
  BOOST_TEST(tree2.get(10) == "ten");
}

// BOOST_AUTO_TEST_CASE(TestAssignmentOperator)
// {
//   gavrilova::TwoThreeTree< int, std::string > tree1;
//   tree1.push(10, "ten");
//   tree1.push(5, "five");

//   gavrilova::TwoThreeTree< int, std::string > tree2;
//   tree2 = tree1;

//   BOOST_TEST(tree1.size() == tree2.size());
//   BOOST_TEST(tree2.find(10) != nullptr);
//   BOOST_TEST(tree2.get(10) == "ten");
// }

BOOST_AUTO_TEST_CASE(TestMoveAssignmentOperator)
{
  gavrilova::TwoThreeTree< int, std::string > tree1;
  tree1.push(10, "ten");
  tree1.push(5, "five");

  gavrilova::TwoThreeTree< int, std::string > tree2;
  tree2 = std::move(tree1);

  BOOST_TEST(tree1.empty());
  BOOST_TEST(tree2.size() == 2);
  BOOST_TEST(tree2.find(10) != nullptr);
  BOOST_TEST(tree2.get(10) == "ten");
}

BOOST_AUTO_TEST_CASE(TestSwap)
{
  gavrilova::TwoThreeTree< int, std::string > tree1;
  tree1.push(10, "ten");
  tree1.push(5, "five");

  gavrilova::TwoThreeTree< int, std::string > tree2;
  tree2.push(20, "twenty");

  tree1.swap(tree2);

  BOOST_TEST(tree1.size() == 1);
  BOOST_TEST(tree1.find(20) != nullptr);
  BOOST_TEST(tree2.size() == 2);
  BOOST_TEST(tree2.find(10) != nullptr);
}

BOOST_AUTO_TEST_CASE(TestClear)
{
  gavrilova::TwoThreeTree< int, std::string > tree;
  tree.push(10, "ten");
  tree.push(5, "five");

  tree.clear();

  BOOST_TEST(tree.empty());
  BOOST_TEST(tree.size() == 0);
  BOOST_TEST(tree.find(10) == nullptr);
}

BOOST_AUTO_TEST_CASE(TestTreeStructureAfterInsertions)
{
  gavrilova::TwoThreeTree< int, std::string > tree;
  tree.push(50, "50");
  tree.push(30, "30");
  tree.push(70, "70");
  tree.push(20, "20");
  tree.push(40, "40");
  tree.push(60, "60");
  tree.push(80, "80");

  auto root = tree.get_node();
  BOOST_TEST(root != nullptr);
  BOOST_TEST(!root->is_3_node);
  BOOST_TEST(root->data[0].first == 50);

  BOOST_TEST(tree.get(20) == "20");
  BOOST_TEST(tree.get(30) == "30");
  BOOST_TEST(tree.get(40) == "40");
  BOOST_TEST(tree.get(50) == "50");
  BOOST_TEST(tree.get(60) == "60");
  BOOST_TEST(tree.get(70) == "70");
  BOOST_TEST(tree.get(80) == "80");
}
