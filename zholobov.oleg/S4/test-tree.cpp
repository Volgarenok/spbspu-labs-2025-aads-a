#include <boost/test/unit_test.hpp>
#include <list>

#include <tree/tree.hpp>

namespace {
  template < typename Key, typename Value, typename Cmp = std::less< Key > >
  std::string toString(const zholobov::Tree< Key, Value, Cmp >& tree)
  {
    std::stringstream ss;
    if (tree.size() != 0) {
      auto it = tree.begin();
      ss << it->first << " " << (it++)->second;
      for (; it != tree.end(); ++it) {
        ss << " " << it->first << " " << it->second;
      }
    }
    return ss.str();
  }
}

BOOST_AUTO_TEST_SUITE(Tree_Constructors)

BOOST_AUTO_TEST_CASE(default_ctor)
{
  zholobov::Tree< int, std::string > tree;
  BOOST_TEST(tree.empty());
  BOOST_TEST(tree.size() == 0);

  zholobov::Tree< int, std::string, std::greater< int > > reverse_tree;
  BOOST_TEST(reverse_tree.empty());
  BOOST_TEST(reverse_tree.size() == 0);
}

BOOST_AUTO_TEST_CASE(initializer_list_ctor)
{
  zholobov::Tree< std::string, int > tree{
      {"this", 10}, {"that", 11}, {"zoom", 12}, {"these", 13}, {"ahead", 14}, {"those", 15}};
  BOOST_TEST(!tree.empty());
  BOOST_TEST(tree.size() == 6);
  BOOST_TEST(toString(tree) == "ahead 14 that 11 these 13 this 10 those 15 zoom 12");

  zholobov::Tree< std::string, int, std::greater< std::string > > reverse_tree{
      {"this", 10}, {"that", 11}, {"zoom", 12}, {"these", 13}, {"ahead", 14}, {"those", 15}};
  BOOST_TEST(!reverse_tree.empty());
  BOOST_TEST(reverse_tree.size() == 6);
  BOOST_TEST(toString(reverse_tree) == "zoom 12 those 15 this 10 these 13 that 11 ahead 14");
}

BOOST_AUTO_TEST_CASE(copy_ctor)
{
  zholobov::Tree< std::string, int > tree{
      {"this", 10}, {"that", 11}, {"zoom", 12}, {"these", 13}, {"ahead", 14}, {"those", 15}};
  BOOST_TEST(tree.size() == 6);
  zholobov::Tree< std::string, int > treeCopy(tree);
  BOOST_TEST(treeCopy.size() == 6);
  BOOST_TEST(toString(tree) == toString(treeCopy));
}

BOOST_AUTO_TEST_CASE(move_ctor)
{
  zholobov::Tree< std::string, int > tree{
      {"this", 10}, {"that", 11}, {"zoom", 12}, {"these", 13}, {"ahead", 14}, {"those", 15}};
  BOOST_TEST(tree.size() == 6);
  std::string tree_contents = toString(tree);
  zholobov::Tree< std::string, int > treeCopy(std::move(tree));
  BOOST_TEST(tree.size() == 0);
  BOOST_TEST(treeCopy.size() == 6);
  BOOST_TEST(toString(tree) == "");
  BOOST_TEST(toString(treeCopy) == tree_contents);
}

BOOST_AUTO_TEST_CASE(range_ctor)
{
  std::list< std::pair< std::string, int > > list{
      {"this", 10}, {"that", 11}, {"zoom", 12}, {"these", 13}, {"ahead", 14}, {"those", 15}};

  zholobov::Tree< std::string, int > tree(list.begin(), list.end());
  BOOST_TEST(tree.size() == 6);
  BOOST_TEST(toString(tree) == "ahead 14 that 11 these 13 this 10 those 15 zoom 12");

  zholobov::Tree< std::string, int, std::greater< std::string > > reverse_tree(list.begin(), list.end());
  BOOST_TEST(reverse_tree.size() == 6);
  BOOST_TEST(toString(reverse_tree) == "zoom 12 those 15 this 10 these 13 that 11 ahead 14");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(Assignment_Operators)

BOOST_AUTO_TEST_CASE(copy_assignment)
{
  zholobov::Tree< std::string, int > tree{
      {"this", 10}, {"that", 11}, {"zoom", 12}, {"these", 13}, {"ahead", 14}, {"those", 15}};
  zholobov::Tree< std::string, int > other_tree{{"something", 3}, {"anything", 5}};
  tree = other_tree;
  BOOST_TEST(tree.size() == 2);
  BOOST_TEST(toString(tree) == "anything 5 something 3");
  BOOST_TEST(toString(tree) == toString(other_tree));
}

BOOST_AUTO_TEST_CASE(move_assignment)
{
  zholobov::Tree< std::string, int > tree{
      {"this", 10}, {"that", 11}, {"zoom", 12}, {"these", 13}, {"ahead", 14}, {"those", 15}};
  zholobov::Tree< std::string, int > other_tree{{"something", 3}, {"anything", 5}};
  tree = std::move(other_tree);
  BOOST_TEST(tree.size() == 2);
  BOOST_TEST(toString(tree) == "anything 5 something 3");
  BOOST_TEST(other_tree.empty());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(Iterators)

BOOST_AUTO_TEST_CASE(iterators)
{
  zholobov::Tree< std::string, int > tree{
      {"this", 10}, {"that", 11}, {"zoom", 12}, {"these", 13}, {"ahead", 14}, {"those", 15}};
  BOOST_TEST(tree.begin()->first == "ahead");
  BOOST_TEST((--(tree.end()))->first == "zoom");
  BOOST_TEST(tree.cbegin()->first == "ahead");
  BOOST_TEST((--(tree.cend()))->first == "zoom");
  auto it = tree.cend();
  std::advance(it, -4);
  BOOST_TEST(it->first == "these");
  std::advance(it, 2);
  BOOST_TEST(it->first == "those");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(Modifiers)

BOOST_AUTO_TEST_CASE(clear)
{
  zholobov::Tree< std::string, int > tree{{"something", 3}, {"anything", 5}};
  BOOST_TEST(tree.size() == 2);
  tree.clear();
  BOOST_TEST(tree.size() == 0);
}

BOOST_AUTO_TEST_CASE(insertion)
{
  zholobov::Tree< std::string, int > tree;
  auto result = tree.insert(std::make_pair("red", 200));
  BOOST_TEST(result.second);
  BOOST_TEST(result.first->first == "red");
  result = tree.insert(std::make_pair("green", 50));
  BOOST_TEST(tree.size() == 2);
  BOOST_TEST(result.second);
  BOOST_TEST(result.first->first == "green");
  result = tree.insert(std::make_pair("red", 100));
  BOOST_TEST(tree.size() == 2);
  BOOST_TEST(!result.second);
  BOOST_TEST(result.first->first == "red");
  BOOST_TEST(result.first->second == 200);
  result = tree.emplace("blue", 230);
  BOOST_TEST(tree.size() == 3);
  BOOST_TEST(result.second);
  BOOST_TEST(result.first->first == "blue");
  result = tree.emplace(std::make_pair("green", 30));
  BOOST_TEST(tree.size() == 3);
  BOOST_TEST(!result.second);
  BOOST_TEST(result.first->first == "green");
  BOOST_TEST(result.first->second == 50);

  auto hint = tree.lower_bound("alpha");
  BOOST_TEST(hint->first == "blue");
  tree.emplace_hint(hint, "alpha", 255);
  BOOST_TEST(tree.size() == 4);
  hint = tree.lower_bound("green");
  BOOST_TEST(hint->first == "green");
  hint = tree.upper_bound("green");
  BOOST_TEST(hint->first == "red");
  tree.emplace_hint(tree.end(), "cyan", 0);
  BOOST_TEST(tree.size() == 5);
  auto it = tree.emplace_hint(tree.end(), "cyan", 1);
  BOOST_TEST(tree.size() == 5);
  BOOST_TEST(it->first == "cyan");
  BOOST_TEST(toString(tree) == "alpha 255 blue 230 cyan 0 green 50 red 200");

  zholobov::Tree< int, char > otherTree;
  auto otherTreeHint = otherTree.cend();
  for (int i = 0; i < 10; ++i) {
    otherTreeHint = otherTree.emplace_hint(otherTreeHint, i, 'a' + i);
  }
  BOOST_TEST(toString(otherTree) == "0 a 1 b 2 c 3 d 4 e 5 f 6 g 7 h 8 i 9 j");

  zholobov::Tree< int, char > otherTree2;
  otherTree2.insert(++otherTree.cbegin(), --otherTree.cend());
  BOOST_TEST(toString(otherTree2) == "1 b 2 c 3 d 4 e 5 f 6 g 7 h 8 i");
}

BOOST_AUTO_TEST_CASE(removing)
{
  zholobov::Tree< int, char > tree;
  auto hint = tree.cend();
  for (int i = 0; i < 10; ++i) {
    hint = tree.emplace_hint(hint, i, 'a' + i);
  }
  auto it = tree.cbegin();
  std::advance(it, 4);
  it = tree.erase(it);
  BOOST_TEST(toString(tree) == "0 a 1 b 2 c 3 d 5 f 6 g 7 h 8 i 9 j");
  auto from = it;
  std::advance(from, -2);
  tree.erase(from, ++it);
  BOOST_TEST(toString(tree) == "0 a 1 b 6 g 7 h 8 i 9 j");
  BOOST_TEST(tree.erase(100) == 0);
  BOOST_TEST(tree.erase(8) == 1);
  BOOST_TEST(toString(tree) == "0 a 1 b 6 g 7 h 9 j");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(Operations)

BOOST_AUTO_TEST_CASE(swap)
{
  zholobov::Tree< std::string, int > tree{
      {"this", 10}, {"that", 11}, {"zoom", 12}, {"these", 13}, {"ahead", 14}, {"those", 15}};
  zholobov::Tree< std::string, int > other_tree{{"something", 3}, {"anything", 5}};
  tree.swap(other_tree);
  std::cout << toString(tree) << '\n';
  std::cout << toString(other_tree) << '\n';
  BOOST_TEST(toString(tree) == "anything 5 something 3");
  BOOST_TEST(toString(other_tree) == "ahead 14 that 11 these 13 this 10 those 15 zoom 12");
}

BOOST_AUTO_TEST_CASE(access)
{
  zholobov::Tree< std::string, int > tree;
  tree["first"] = 1;
  tree["second"] = 2;
  tree["third"] = 3;
  tree["fourth"] = 4;
  BOOST_TEST(toString(tree) == "first 1 fourth 4 second 2 third 3");
  BOOST_TEST(tree["second"] == 2);
  BOOST_TEST(tree.at("third") == 3);
  try {
    tree.at("zero");
  } catch (const std::out_of_range& e) {
    BOOST_TEST(e.what() == "Key not found");
  }
}

BOOST_AUTO_TEST_CASE(find)
{
  zholobov::Tree< std::string, int > tree{
      {"this", 10}, {"that", 11}, {"zoom", 12}, {"these", 13}, {"ahead", 14}, {"those", 15}};
  BOOST_TEST(tree.find("zoom")->first == "zoom");
  BOOST_TEST(tree.find("zoom")->second == 12);
  bool notFound = (tree.find("NONE") == tree.end());
  BOOST_TEST(notFound);
}

BOOST_AUTO_TEST_CASE(count)
{
  zholobov::Tree< std::string, int > tree{
      {"this", 10}, {"that", 11}, {"zoom", 12}, {"these", 13}, {"ahead", 14}, {"those", 15}};
  BOOST_TEST(tree.count("zoom") == 1);
  BOOST_TEST(tree.count("NONE") == 0);
}

BOOST_AUTO_TEST_CASE(lower_upper_bound)
{
  zholobov::Tree< int, std::string > tree{{1, "one"}, {2, "two"}, {3, "three"}, {5, "five"}, {6, "six"}};
  BOOST_TEST(tree.lower_bound(3)->first == 3);
  BOOST_TEST(tree.lower_bound(4)->first == 5);
  BOOST_TEST(tree.lower_bound(5)->first == 5);
  bool equalToEnd = (tree.cend() == tree.lower_bound(7));
  BOOST_TEST(equalToEnd);

  BOOST_TEST(tree.upper_bound(3)->first == 5);
  BOOST_TEST(tree.upper_bound(4)->first == 5);
  BOOST_TEST(tree.upper_bound(5)->first == 6);
  equalToEnd = (tree.cend() == tree.upper_bound(6));
  BOOST_TEST(equalToEnd);
}

BOOST_AUTO_TEST_CASE(equal_range)
{
  zholobov::Tree< int, std::string > tree{{1, "one"}, {2, "two"}, {3, "three"}, {5, "five"}, {6, "six"}};
  auto range = tree.equal_range(3);
  BOOST_TEST(range.first->first == 3);
  BOOST_TEST(range.second->first == 5);
  range = tree.equal_range(4);
  BOOST_TEST(range.first->first == 5);
  BOOST_TEST(range.second->first == 5);
}

BOOST_AUTO_TEST_SUITE_END()
