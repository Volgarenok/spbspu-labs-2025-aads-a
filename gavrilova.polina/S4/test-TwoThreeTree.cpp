#include <boost/test/unit_test.hpp>
#include <string>
#include <tree/ConstIterator.hpp>
#include <tree/Iterator.hpp>
#include <tree/TwoThreeTree.hpp>

BOOST_AUTO_TEST_CASE(TestEmptyTree)
{
  gavrilova::TwoThreeTree< int, std::string > tree;
  BOOST_TEST(tree.empty());
  BOOST_TEST(tree.size() == 0);
  BOOST_CHECK(tree.find(42) == tree.end());
}

BOOST_AUTO_TEST_CASE(TestInsertAndFind)
{
  gavrilova::TwoThreeTree< int, std::string > tree;

  auto result1 = tree.insert({10, "ten"});
  BOOST_TEST(result1.second);
  BOOST_CHECK(result1.first->first == 10);
  BOOST_CHECK(!tree.empty());
  BOOST_TEST(tree.size() == 1);

  auto it1 = tree.find(10);
  BOOST_CHECK(it1 != tree.end());
  BOOST_TEST(it1->second == "ten");
  BOOST_TEST(tree.at(10) == "ten");

  auto result2 = tree.insert({5, "five"});
  BOOST_TEST(result2.second);
  BOOST_TEST(tree.size() == 2);
  BOOST_CHECK(tree.find(5) != tree.end());
  BOOST_TEST(tree.at(5) == "five");

  tree.insert({15, "fifteen"});
  tree.insert({20, "twenty"});
  BOOST_TEST(tree.size() == 4);
  BOOST_CHECK(tree.find(15) != tree.end());
  BOOST_CHECK(tree.find(20) != tree.end());
  BOOST_TEST(tree.at(15) == "fifteen");
  BOOST_TEST(tree.at(20) == "twenty");

  auto result_duplicate = tree.insert({15, "another_fifteen"});
  BOOST_TEST(!result_duplicate.second);
  BOOST_TEST(tree.size() == 4);
  BOOST_TEST(tree.at(15) == "fifteen");
}

BOOST_AUTO_TEST_CASE(TestAtNonExistentKey)
{
  gavrilova::TwoThreeTree< int, std::string > tree;
  tree.insert({10, "ten"});
  BOOST_CHECK_THROW(tree.at(42), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(TestInsertMultipleElements)
{
  gavrilova::TwoThreeTree< int, std::string > tree;
  const int count = 500;

  for (int i = 0; i < count; ++i) {
    tree.insert({i, "value_" + std::to_string(i)});
  }

  BOOST_TEST(tree.size() == count);

  for (int i = 0; i < count; ++i) {
    auto it = tree.find(i);
    BOOST_CHECK(it != tree.end());
    BOOST_TEST(it->second == "value_" + std::to_string(i));
  }
}

BOOST_AUTO_TEST_CASE(TestCopyConstructor)
{
  gavrilova::TwoThreeTree< int, std::string > tree1;
  tree1.insert({10, "ten"});
  tree1.insert({5, "five"});
  tree1.insert({15, "fifteen"});

  gavrilova::TwoThreeTree< int, std::string > tree2(tree1);

  BOOST_TEST(tree1.size() == tree2.size());
  BOOST_CHECK(tree2.find(10) != tree2.end());
  BOOST_TEST(tree2.at(10) == "ten");
  BOOST_CHECK(tree2.find(5) != tree2.end());
  BOOST_TEST(tree2.at(5) == "five");

  tree2.insert({20, "twenty"});
  BOOST_TEST(tree1.size() == 3);
  BOOST_CHECK(tree1.find(20) == tree1.end());
}

BOOST_AUTO_TEST_CASE(TestMoveConstructor)
{
  gavrilova::TwoThreeTree< int, std::string > tree1;
  tree1.insert({10, "ten"});
  tree1.insert({5, "five"});

  gavrilova::TwoThreeTree< int, std::string > tree2(std::move(tree1));

  BOOST_TEST(tree1.empty());
  BOOST_TEST(tree2.size() == 2);
  BOOST_CHECK(tree2.find(10) != tree2.end());
  BOOST_TEST(tree2.at(10) == "ten");
}

BOOST_AUTO_TEST_CASE(TestInitializerListConstructor)
{
  gavrilova::TwoThreeTree< int, std::string > empty_tree({});
  BOOST_TEST(empty_tree.empty());
  BOOST_TEST(empty_tree.size() == 0);

  gavrilova::TwoThreeTree< int, std::string > single_tree{{42, "answer"}};
  BOOST_TEST(!single_tree.empty());
  BOOST_TEST(single_tree.size() == 1);
  BOOST_CHECK(single_tree.find(42) != single_tree.end());
  BOOST_TEST(single_tree.at(42) == "answer");

  gavrilova::TwoThreeTree< int, std::string > multi_tree{
      {10, "ten"},
      {5, "five"},
      {15, "fifteen"},
      {3, "three"},
      {7, "seven"}};

  BOOST_TEST(multi_tree.size() == 5);
  BOOST_CHECK(multi_tree.find(5) != multi_tree.end());
  BOOST_CHECK(multi_tree.find(10) != multi_tree.end());
  BOOST_CHECK(multi_tree.find(15) != multi_tree.end());
  BOOST_TEST(multi_tree.at(3) == "three");
  BOOST_TEST(multi_tree.at(7) == "seven");

  gavrilova::TwoThreeTree< int, std::string > dup_tree{
      {1, "one"},
      {1, "uno"},
      {2, "two"},
      {1, "ein"}};
  BOOST_TEST(dup_tree.size() == 2);
  BOOST_TEST(dup_tree.at(1) == "one");
}

BOOST_AUTO_TEST_CASE(TestRangeConstructor)
{
  std::vector< std::pair< int, std::string > > empty_vec;
  gavrilova::TwoThreeTree< int, std::string > empty_tree(empty_vec.begin(), empty_vec.end());
  BOOST_TEST(empty_tree.empty());

  std::vector< std::pair< int, std::string > > elements = {
      {100, "hundred"},
      {50, "fifty"},
      {150, "one-fifty"}};

  gavrilova::TwoThreeTree< int, std::string > vec_tree(elements.begin(), elements.end());
  BOOST_TEST(vec_tree.size() == 3);
  BOOST_CHECK(vec_tree.find(50) != vec_tree.end());
  BOOST_CHECK(vec_tree.find(100) != vec_tree.end());
  BOOST_CHECK(vec_tree.find(150) != vec_tree.end());

  std::pair< int, std::string > arr[] = {
      {2, "two"},
      {1, "one"},
      {3, "three"}};

  gavrilova::TwoThreeTree< int, std::string > arr_tree(std::begin(arr), std::end(arr));
  BOOST_TEST(arr_tree.size() == 3);
  BOOST_TEST(arr_tree.at(1) == "one");
  BOOST_TEST(arr_tree.at(2) == "two");
  BOOST_TEST(arr_tree.at(3) == "three");

  std::vector< std::pair< int, std::string > > long_vec = {
      {1, "a"},
      {2, "b"},
      {3, "c"},
      {4, "d"},
      {5, "e"}};

  gavrilova::TwoThreeTree< int, std::string > partial_tree(long_vec.begin() + 1, long_vec.end() - 1);
  BOOST_TEST(partial_tree.size() == 3);
  BOOST_CHECK(partial_tree.find(1) == partial_tree.end());
  BOOST_CHECK(partial_tree.find(2) != partial_tree.end());
  BOOST_CHECK(partial_tree.find(4) != partial_tree.end());
  BOOST_CHECK(partial_tree.find(5) == partial_tree.end());
}

BOOST_AUTO_TEST_CASE(TestInitializerListVsRangeConstructor)
{
  std::initializer_list< std::pair< int, std::string > > il = {
      {7, "seven"},
      {3, "three"},
      {11, "eleven"}};

  gavrilova::TwoThreeTree< int, std::string > tree1(il);
  gavrilova::TwoThreeTree< int, std::string > tree2(il.begin(), il.end());

  BOOST_TEST(tree1.size() == tree2.size());
  BOOST_CHECK(tree1.find(3) != tree1.end());
  BOOST_CHECK(tree2.find(3) != tree2.end());
  BOOST_CHECK(tree1.find(7) != tree1.end());
  BOOST_CHECK(tree2.find(7) != tree2.end());
  BOOST_CHECK(tree1.find(11) != tree1.end());
  BOOST_CHECK(tree2.find(11) != tree2.end());
}

BOOST_AUTO_TEST_CASE(TestConstructorWithDuplicates)
{
  std::vector< std::pair< int, std::string > > with_duplicates = {
      {1, "first"},
      {1, "second"},
      {2, "two"},
      {1, "third"}};

  gavrilova::TwoThreeTree< int, std::string > tree1(with_duplicates.begin(), with_duplicates.end());
  gavrilova::TwoThreeTree< int, std::string > tree2{
      {1, "first"},
      {1, "second"},
      {2, "two"},
      {1, "third"}};

  BOOST_TEST(tree1.size() == 2);
  BOOST_TEST(tree2.size() == 2);
  BOOST_TEST(tree1.at(1) == "first");
  BOOST_TEST(tree2.at(1) == "first");
}

BOOST_AUTO_TEST_CASE(TestAssignmentOperator)
{
  gavrilova::TwoThreeTree< int, std::string > tree1;
  tree1.insert({10, "ten"});
  tree1.insert({5, "five"});

  gavrilova::TwoThreeTree< int, std::string > tree2;
  tree2.insert({100, "hundred"});
  tree2 = tree1;

  BOOST_TEST(tree1.size() == tree2.size());
  BOOST_CHECK(tree2.find(10) != tree2.end());
  BOOST_TEST(tree2.at(10) == "ten");
  BOOST_CHECK(tree2.find(100) == tree2.end());
}

BOOST_AUTO_TEST_CASE(TestMoveAssignmentOperator)
{
  gavrilova::TwoThreeTree< int, std::string > tree1;
  tree1.insert({10, "ten"});
  tree1.insert({5, "five"});

  gavrilova::TwoThreeTree< int, std::string > tree2;
  tree2 = std::move(tree1);

  BOOST_TEST(tree1.empty());
  BOOST_TEST(tree2.size() == 2);
  BOOST_CHECK(tree2.find(10) != tree2.end());
  BOOST_TEST(tree2.at(10) == "ten");
}

BOOST_AUTO_TEST_CASE(TestSwap)
{
  gavrilova::TwoThreeTree< int, std::string > tree1;
  tree1.insert({10, "ten"});
  tree1.insert({5, "five"});

  gavrilova::TwoThreeTree< int, std::string > tree2;
  tree2.insert({20, "twenty"});

  tree1.swap(tree2);

  BOOST_TEST(tree1.size() == 1);
  BOOST_CHECK(tree1.find(20) != tree1.end());
  BOOST_CHECK(tree1.find(10) == tree1.end());

  BOOST_TEST(tree2.size() == 2);
  BOOST_CHECK(tree2.find(10) != tree2.end());
  BOOST_CHECK(tree2.find(20) == tree2.end());
}

BOOST_AUTO_TEST_CASE(TestClear)
{
  gavrilova::TwoThreeTree< int, std::string > tree;
  tree.insert({10, "ten"});
  tree.insert({5, "five"});

  tree.clear();

  BOOST_TEST(tree.empty());
  BOOST_TEST(tree.size() == 0);
  BOOST_CHECK(tree.find(10) == tree.end());
}

BOOST_AUTO_TEST_CASE(TestTreeStructureAfterInsertions)
{
  gavrilova::TwoThreeTree< int, std::string > tree;
  tree.insert({50, "50"});
  tree.insert({30, "30"});
  tree.insert({70, "70"});
  tree.insert({20, "20"});
  tree.insert({40, "40"});
  tree.insert({60, "60"});
  tree.insert({80, "80"});

  auto root = tree.get_node();
  BOOST_CHECK(root != nullptr);
  BOOST_CHECK(!root->is_3_node);
  BOOST_TEST(root->data[0].first == 50);

  for (int key: {20, 30, 40, 50, 60, 70, 80}) {
    BOOST_CHECK(tree.find(key) != tree.end());
    BOOST_TEST(tree.at(key) == std::to_string(key));
  }
}

BOOST_AUTO_TEST_CASE(TestIteratorBegin)
{
  gavrilova::TwoThreeTree< int, std::string > tree;
  tree.insert({50, "50"});
  tree.insert({30, "30"});
  tree.insert({70, "70"});

  auto it = tree.begin();
  BOOST_CHECK(it != tree.end());
  BOOST_TEST(it->first == 30);
  BOOST_TEST(it->second == "30");
}

BOOST_AUTO_TEST_CASE(TestIteratorForwardTraversal)
{
  gavrilova::TwoThreeTree< int, std::string > tree;
  tree.insert({10, "ten"});
  tree.insert({5, "five"});
  tree.insert({15, "fifteen"});

  std::vector< std::pair< int, std::string > > expected = {
      {5, "five"}, {10, "ten"}, {15, "fifteen"}};

  auto it = tree.begin();
  for (size_t i = 0; i < expected.size(); ++i, ++it) {
    BOOST_CHECK(it != tree.end());
    BOOST_TEST(it->first == expected[i].first);
    BOOST_TEST(it->second == expected[i].second);
  }
  BOOST_CHECK(it == tree.end());
}

BOOST_AUTO_TEST_CASE(TestIteratorBackwardTraversal)
{
  gavrilova::TwoThreeTree< int, std::string > tree;
  tree.insert({10, "ten"});
  tree.insert({5, "five"});
  tree.insert({15, "fifteen"});
  tree.insert({2, "two"});
  tree.insert({1, "one"});
  tree.insert({3, "three"});

  std::vector< std::pair< int, std::string > > expected = {
      {15, "fifteen"}, {10, "ten"}, {5, "five"}, {3, "three"}, {2, "two"}, {1, "one"}};

  auto it = tree.end();
  for (size_t i = 0; i < expected.size(); ++i) {
    --it;
    BOOST_TEST(it->first == expected[i].first);
    BOOST_TEST(it->second == expected[i].second);
  }
  BOOST_CHECK(it == tree.begin());
}

BOOST_AUTO_TEST_CASE(TestIteratorEquality)
{
  gavrilova::TwoThreeTree< int, std::string > tree;
  tree.insert({10, "ten"});
  tree.insert({20, "twenty"});

  auto it1 = tree.begin();
  auto it2 = tree.begin();

  BOOST_CHECK(it1 == it2);
  ++it1;
  BOOST_CHECK(it1 != it2);
  ++it2;
  BOOST_CHECK(it1 == it2);
}

BOOST_AUTO_TEST_CASE(TestOperatorSquareBrackets)
{
  gavrilova::TwoThreeTree< int, std::string > tree;

  tree[10] = "ten";
  BOOST_TEST(tree.size() == 1);
  BOOST_TEST(tree.at(10) == "ten");

  tree[10] = "new_ten";
  BOOST_TEST(tree.size() == 1);
  BOOST_TEST(tree.at(10) == "new_ten");

  tree[20];
  BOOST_TEST(tree.size() == 2);
  BOOST_TEST(tree.at(20) == "");
}

BOOST_AUTO_TEST_CASE(TestEraseByKey)
{
  gavrilova::TwoThreeTree< int, std::string > tree;

  BOOST_TEST(tree.erase(42) == 0);

  tree.insert({10, "ten"});
  tree.insert({5, "five"});
  tree.insert({15, "fifteen"});
  tree.insert({3, "three"});
  tree.insert({7, "seven"});
  tree.insert({12, "twelve"});
  tree.insert({20, "twenty"});

  BOOST_TEST(tree.erase(42) == 0);
  BOOST_TEST(tree.size() == 7);

  BOOST_TEST(tree.erase(3) == 1);
  BOOST_TEST(tree.size() == 6);
  BOOST_CHECK(tree.find(3) == tree.end());

  BOOST_TEST(tree.erase(12) == 1);
  BOOST_TEST(tree.size() == 5);
  BOOST_CHECK(tree.find(12) == tree.end());

  BOOST_TEST(tree.erase(5) == 1);
  BOOST_TEST(tree.size() == 4);
  BOOST_CHECK(tree.find(5) == tree.end());

  tree.insert({8, "eight"});
  tree.insert({6, "six"});
  BOOST_TEST(tree.erase(7) == 1);
  BOOST_TEST(tree.size() == 5);
  BOOST_CHECK(tree.find(7) == tree.end());

  BOOST_TEST(tree.erase(10) == 1);
  BOOST_TEST(tree.size() == 4);
  BOOST_CHECK(tree.find(10) == tree.end());
}

BOOST_AUTO_TEST_CASE(TestEraseByIterator)
{
  gavrilova::TwoThreeTree< int, std::string > tree;

  BOOST_CHECK_THROW(tree.erase(tree.end()), std::out_of_range);

  tree.insert({10, "ten"});
  tree.insert({5, "five"});
  tree.insert({15, "fifteen"});
  tree.insert({3, "three"});
  tree.insert({7, "seven"});
  tree.insert({12, "twelve"});
  tree.insert({20, "twenty"});

  auto it1 = tree.begin();
  BOOST_CHECK_NO_THROW(tree.erase(it1));
  BOOST_TEST(tree.size() == 6);
  BOOST_CHECK(tree.find(3) == tree.end());

  auto it2 = tree.find(7);
  BOOST_REQUIRE(it2 != tree.end());
  auto next_it = tree.erase(it2);
  BOOST_TEST(tree.size() == 5);
  BOOST_CHECK(tree.find(7) == tree.end());
  BOOST_CHECK(next_it->first == 10);

  auto it3 = tree.find(20);
  BOOST_REQUIRE(it3 != tree.end());
  BOOST_CHECK_NO_THROW(tree.erase(it3));
  BOOST_TEST(tree.size() == 4);
  BOOST_CHECK(tree.find(20) == tree.end());

  auto it4 = tree.find(10);
  BOOST_REQUIRE(it4 != tree.end());
  BOOST_CHECK_NO_THROW(tree.erase(it4));
  BOOST_TEST(tree.size() == 3);
  BOOST_CHECK(tree.find(10) == tree.end());
}

BOOST_AUTO_TEST_CASE(TestEraseComplexCases)
{
  gavrilova::TwoThreeTree< int, std::string > tree;

  tree.insert({50, "50"});
  tree.insert({30, "30"});
  tree.insert({70, "70"});
  tree.insert({20, "20"});
  tree.insert({40, "40"});
  tree.insert({60, "60"});
  tree.insert({80, "80"});
  tree.insert({10, "10"});
  tree.insert({25, "25"});
  tree.insert({35, "35"});
  tree.insert({45, "45"});
  tree.insert({55, "55"});
  tree.insert({65, "65"});
  tree.insert({75, "75"});
  tree.insert({85, "85"});

  BOOST_TEST(tree.erase(10) == 1);
  BOOST_TEST(tree.size() == 14);
  BOOST_CHECK(tree.find(10) == tree.end());

  BOOST_TEST(tree.erase(75) == 1);
  BOOST_TEST(tree.size() == 13);
  BOOST_CHECK(tree.find(75) == tree.end());

  BOOST_TEST(tree.erase(25) == 1);
  BOOST_TEST(tree.erase(20) == 1);
  BOOST_TEST(tree.size() == 11);
  BOOST_CHECK(tree.find(20) == tree.end());
  BOOST_CHECK(tree.find(25) == tree.end());

  BOOST_TEST(tree.erase(65) == 1);
  BOOST_TEST(tree.erase(60) == 1);
  BOOST_TEST(tree.size() == 9);
  BOOST_CHECK(tree.find(60) == tree.end());
  BOOST_CHECK(tree.find(65) == tree.end());

  BOOST_TEST(tree.erase(50) == 1);
  BOOST_TEST(tree.size() == 8);
  BOOST_CHECK(tree.find(50) == tree.end());

  std::vector< int > expected = {30, 35, 40, 45, 55, 70, 80, 85};
  BOOST_TEST(tree.size() == expected.size());
  for (int key: expected) {
    BOOST_CHECK(tree.find(key) != tree.end());
  }
}

BOOST_AUTO_TEST_CASE(TestEraseAllElements)
{
  gavrilova::TwoThreeTree< int, std::string > tree;
  const int count = 100;

  for (int i = 0; i < count; ++i) {
    tree.insert({i, "val_" + std::to_string(i)});
  }
  BOOST_TEST(tree.size() == count);

  for (int i = 0; i < count; ++i) {
    BOOST_TEST(tree.erase(i) == 1);
    BOOST_TEST(tree.size() == count - i - 1);
    BOOST_CHECK(tree.find(i) == tree.end());
  }
  BOOST_TEST(tree.empty());
}

BOOST_AUTO_TEST_CASE(TestRangeInsert)
{
  gavrilova::TwoThreeTree< int, std::string > tree;

  std::vector< std::pair< int, std::string > > empty_vec;
  tree.insert(empty_vec.begin(), empty_vec.end());
  BOOST_TEST(tree.empty());

  std::vector< std::pair< int, std::string > > elements = {
      {30, "thirty"},
      {10, "ten"},
      {20, "twenty"},
      {40, "forty"}};
  tree.insert(elements.begin(), elements.end());

  BOOST_TEST(tree.size() == 4);
  BOOST_CHECK(tree.find(10) != tree.end());
  BOOST_CHECK(tree.find(20) != tree.end());
  BOOST_CHECK(tree.find(30) != tree.end());
  BOOST_CHECK(tree.find(40) != tree.end());

  std::pair< int, std::string > arr[] = {
      {50, "fifty"},
      {60, "sixty"}};
  tree.insert(std::begin(arr), std::end(arr));

  BOOST_TEST(tree.size() == 6);
  BOOST_CHECK(tree.find(50) != tree.end());
  BOOST_CHECK(tree.find(60) != tree.end());

  std::vector< std::pair< int, std::string > > duplicates = {
      {10, "ten_duplicate"},
      {20, "twenty_duplicate"},
      {70, "seventy"}};
  tree.insert(duplicates.begin(), duplicates.end());

  BOOST_TEST(tree.size() == 7);
  BOOST_TEST(tree.at(10) == "ten");
  BOOST_TEST(tree.at(70) == "seventy");
}

BOOST_AUTO_TEST_CASE(TestInitializerListInsert)
{
  gavrilova::TwoThreeTree< int, std::string > tree;

  tree.insert({});
  BOOST_TEST(tree.empty());

  tree.insert({{1, "one"}});
  BOOST_TEST(tree.size() == 1);
  BOOST_CHECK(tree.find(1) != tree.end());

  tree.insert({{2, "two"},
      {3, "three"},
      {4, "four"}});

  BOOST_TEST(tree.size() == 4);
  BOOST_CHECK(tree.find(2) != tree.end());
  BOOST_CHECK(tree.find(3) != tree.end());
  BOOST_CHECK(tree.find(4) != tree.end());

  tree.insert({{1, "one_duplicate"},
      {4, "four_duplicate"},
      {5, "five"}});

  BOOST_TEST(tree.size() == 5);
  BOOST_TEST(tree.at(1) == "one");
  BOOST_TEST(tree.at(5) == "five");
}

BOOST_AUTO_TEST_CASE(TestRangeErase)
{
  gavrilova::TwoThreeTree< int, std::string > tree;

  for (int i = 1; i <= 10; ++i) {
    tree.insert({i, "value_" + std::to_string(i)});
  }

  BOOST_TEST(tree.size() == 10);

  auto it_begin = tree.find(1);
  tree.erase(it_begin, it_begin);
  BOOST_TEST(tree.size() == 10);

  auto it1 = tree.find(3);
  auto it2 = tree.find(4);
  tree.erase(it1, it2);

  BOOST_TEST(tree.size() == 9);
  BOOST_CHECK(tree.find(3) == tree.end());
  BOOST_CHECK(tree.find(4) != tree.end());

  auto it3 = tree.find(5);
  auto it4 = tree.find(8);
  tree.erase(it3, it4);

  BOOST_TEST(tree.size() == 6);
  BOOST_CHECK(tree.find(5) == tree.end());
  BOOST_CHECK(tree.find(6) == tree.end());
  BOOST_CHECK(tree.find(7) == tree.end());
  BOOST_CHECK(tree.find(8) != tree.end());

  auto it5 = tree.find(100);
  auto it6 = tree.find(200);
  if (it5 != tree.end()) {
    tree.erase(it5, it6);
  }
  BOOST_TEST(tree.size() == 6);
}

BOOST_AUTO_TEST_CASE(TestRangeEraseBoundaryCases)
{
  gavrilova::TwoThreeTree< int, std::string > tree;

  tree.insert({{1, "one"},
      {2, "two"},
      {3, "three"},
      {4, "four"},
      {5, "five"}});

  tree.erase(tree.begin(), tree.find(3));
  BOOST_TEST(tree.size() == 3);
  BOOST_CHECK(tree.find(1) == tree.end());
  BOOST_CHECK(tree.find(2) == tree.end());
  BOOST_CHECK(tree.find(3) != tree.end());

  tree.erase(tree.find(4), tree.end());
  BOOST_TEST(tree.size() == 1);
  BOOST_CHECK(tree.find(3) != tree.end());
  BOOST_CHECK(tree.find(4) == tree.end());
  BOOST_CHECK(tree.find(5) == tree.end());

  tree.erase(tree.begin(), tree.end());
  BOOST_TEST(tree.empty());
}
