#include <boost/test/unit_test.hpp>
#include <algorithm>
#include <vector>
#include "tree.hpp"

using aleksandrov::Tree;

BOOST_AUTO_TEST_SUITE(tree_construction)

BOOST_AUTO_TEST_CASE(default_construction)
{
  Tree< Tree< int, char >, char* > tree;
  BOOST_TEST(tree.size() == 0);
}

BOOST_AUTO_TEST_CASE(comparator_construction)
{
  Tree< int, char, std::greater< int > > tree(std::greater< int >{});
  BOOST_TEST(tree.size() == 0);
}

BOOST_AUTO_TEST_CASE(copy_construction)
{
  Tree< int, int > oak1 = { { 1, 1 }, { 2, 2 }, { 3, 3 } };

  Tree< int, int > oak2(oak1);
  BOOST_TEST(std::equal(oak1.begin(), oak1.end(), oak2.begin()));
}

BOOST_AUTO_TEST_CASE(range_construction)
{
  std::vector< std::pair< int, char > > data;
  data = { { 1, 'a' }, { 2, 'b' }, { 3, 'c' }, { 4, 'd' }, { 5, 'e' } };

  Tree< int, char > ash(++data.begin(), std::next(data.begin(), 4));
  BOOST_TEST(ash.size() == 3);
  Tree< int, char > elm({ { 2, 'b' }, { 3, 'c' }, { 4, 'd' } });
  BOOST_TEST(std::equal(ash.begin(), ash.end(), elm.begin()));

  Tree< int, char > pine(data.begin(), data.end());
  BOOST_TEST(pine.size() == 5);
  BOOST_TEST(std::equal(pine.begin(), pine.end(), data.begin()));
}

BOOST_AUTO_TEST_CASE(move_construction)
{
  Tree< double, double > birch1({ { 1.0, 1.0 }, { 2.0, 2.0 }, { 3.0, 3.0 } });
  Tree< double, double > copy(birch1);

  Tree< double, double > birch2(std::move(birch1));
  BOOST_TEST(std::equal(birch2.begin(), birch2.end(), copy.begin()));
  BOOST_TEST(birch1.empty());
}

BOOST_AUTO_TEST_CASE(initializer_list_construction)
{
  Tree< int, char > tree({ {1, 'a'}, {2, 'b'}, {3, 'c'} });
  BOOST_TEST(tree.size() == 3);
  BOOST_TEST(tree[1] == 'a');
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(tree_assignment)

BOOST_AUTO_TEST_CASE(copy_assignment)
{
  Tree< char, char > oak1({ { 'a', 'a' }, { 'b', 'b' }, { 'c', 'c' } });
  Tree< char, char > oak2({ { 'd', 'd' } });
  oak2 = oak1;
  BOOST_TEST(std::equal(oak1.begin(), oak1.end(), oak2.begin()));
}

BOOST_AUTO_TEST_CASE(move_assignment)
{
  Tree< int, int > pine1({ { 1, 1 }, { 2, 2 }, { 3, 3 }, { 4, 4 } });
  Tree< int, int > copy(pine1);

  Tree< int, int > pine2({ { 5, 5 } });
  pine2 = std::move(pine1);
  BOOST_TEST(std::equal(pine2.begin(), pine2.end(), copy.begin()));
  BOOST_TEST(pine1.empty());
}

BOOST_AUTO_TEST_CASE(initializer_list_assignment)
{
  Tree< unsigned int, int > maple({ { 5, 3 } });
  Tree< unsigned int, int > oak({ { 1, 0 }, { 2, -1 }, { 3, 1 }, { 4, -2 } });
  maple = { { 1, 0 }, { 2, -1 }, { 3, 1 }, { 4, -2 } };
  BOOST_TEST(std::equal(maple.begin(), maple.end(), oak.begin()));
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(iterators);

BOOST_AUTO_TEST_CASE(begin)
{
  Tree< unsigned int, char > larch;
  BOOST_TEST((larch.begin() == larch.end()));

  larch.insert({ 1, 'a' });
  BOOST_TEST(larch.begin()->first == 1);

  larch.insert({ 2, 'b' });
  BOOST_TEST(larch.cbegin()->first == 1);

  larch.clear();
  BOOST_TEST((larch.cbegin() == larch.cend()));
}

BOOST_AUTO_TEST_CASE(end)
{
  Tree< char, int > beech;
  beech = { { 'x', 1 }, { 'y', 2 }, { 'z', 3 } };
  auto it = beech.cbegin();
  std::advance(it, 3);
  BOOST_TEST((it == beech.cend()));
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(tree_element_access)

BOOST_AUTO_TEST_CASE(square_brackets_operator)
{
  Tree< int, int > birch({ { 1, 1 }, { 2, 2 }, { 3, 3 } });
  BOOST_TEST(birch[1] == 1);
  BOOST_TEST(birch[2] == 2);

  birch.erase(1);
  BOOST_TEST(birch[3] == 3);
}

BOOST_AUTO_TEST_CASE(at)
{
  Tree< int, int > maple({ { 1, 4 }, { 2, 5 }, { 3, 6 } });
  BOOST_TEST(maple.at(1) == 4);
  BOOST_TEST(maple.at(2) == 5);

  maple.erase(2);
  BOOST_TEST(maple.at(1) == 4);

  BOOST_CHECK_THROW(maple.at(2), std::out_of_range);
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(tree_capacity)

BOOST_AUTO_TEST_CASE(empty)
{
  Tree< int, int > fir;
  BOOST_TEST(fir.empty());

  fir.insert({ 0, 0 });
  BOOST_TEST(!fir.empty());

  fir.insert({ 1, 0 });
  BOOST_TEST(!fir.empty());

  fir.erase(fir.begin(), fir.end());
  BOOST_TEST(fir.empty());
}

BOOST_AUTO_TEST_CASE(size)
{
  Tree< int, int > spruce;
  BOOST_TEST(spruce.size() == 0);

  spruce.insert({ 0, 0 });
  BOOST_TEST(spruce.size() == 1);

  spruce.insert({ 1, 0 });
  BOOST_TEST(spruce.size() == 2);

  spruce.erase(1);
  BOOST_TEST(spruce.size() == 1);
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(tree_modifiers)

BOOST_AUTO_TEST_CASE(insert)
{
  Tree< int, int > acacia;

  acacia.insert({ 1, 1 });
  BOOST_TEST(acacia[1] == 1);

  auto pair = acacia.insert({ 1, 1 });
  BOOST_TEST(acacia[1] == 1);
  BOOST_TEST((pair.first == acacia.find(1)));
  BOOST_TEST(pair.second == false);

  pair = acacia.insert({ 2, 2 });
  BOOST_TEST(acacia[2] == 2);
  BOOST_TEST((pair.first == acacia.find(2)));
  BOOST_TEST(pair.second == true);

  pair = acacia.insert({ 2, 3 });
  BOOST_TEST(acacia[2] == 2);
  BOOST_TEST((pair.first == acacia.find(2)));
  BOOST_TEST(pair.second == false);

  int n = acacia[3] = 3;
  BOOST_TEST(acacia.size() == 3);
  BOOST_TEST(n == 3);

  std::vector< int > keys(7);
  keys = { 1, 2, 3, 7, 6, 4, 5 };
  Tree< int, int > aspen;
  do
  {
    for (size_t i = 0; i < keys.size(); ++i)
    {
      aspen.insert({ keys[i], 0 });
    }
    aspen.clear();
  }
  while (std::next_permutation(keys.begin(), keys.end()));
}

BOOST_AUTO_TEST_CASE(insert_hint)
{
  Tree< int, char > birch;
  birch = { { 1, 'a' }, { 3, 'c' }, { 4, 'd' }, { 7, 'g' } };
  Tree< int, char > oak = { { 1, 'a' }, { 3, 'c' }, { 4, 'd' }, { 7, 'g' } };

  auto it = birch.insert(++birch.begin(), { 1, 'a' });
  BOOST_TEST(birch.size() == 4);
  BOOST_TEST(std::equal(birch.begin(), birch.end(), oak.begin()));

  it = birch.insert(birch.find(7), { 5, 'e' });
  BOOST_TEST(birch.size() == 5);
  BOOST_TEST((it == birch.find(5)));

  it = birch.insert(birch.end(), { 2, 'b' });
  BOOST_TEST(birch.size() == 6);
  BOOST_TEST((it == birch.find(2)));

  it = birch.insert(birch.end(), { 2, 'f' });
  BOOST_TEST(birch.size() == 6);
  BOOST_TEST((it == birch.find(2)));
}

BOOST_AUTO_TEST_CASE(insert_range)
{
  Tree< int, char > linden;
  Tree< int, char > willow;
  willow = { { 1, 'a' }, { 2, 'b' }, { 3, 'c' }, { 4, 'd' }, { 5, 'e' } };

  linden.insert(++willow.begin(), std::next(willow.begin(), 4));
  Tree< int, char > elm = { { 2, 'b' }, { 3, 'c' }, { 4, 'd' } };
  BOOST_TEST((linden == elm));

  linden.insert(willow.begin(), willow.end());
  BOOST_TEST((linden == willow));
}

BOOST_AUTO_TEST_CASE(insert_initializer_list)
{
  Tree< int, char > rowan = { { 3, 'c' } };

  rowan.insert({ { 1, 'a' }, { 2, 'b' }, { 4, 'd' } });
  Tree< int, char > larch = { { 1, 'a' }, { 2, 'b' }, { 3, 'c' }, { 4, 'd' } };
  BOOST_TEST(rowan.size() == 4);
  BOOST_TEST(std::equal(rowan.begin(), rowan.end(), larch.begin()));
}

BOOST_AUTO_TEST_CASE(emplace)
{
  Tree< char, float > chestnut;

  auto pair = chestnut.emplace('A', 1.0f);
  BOOST_TEST(chestnut.size() == 1);
  BOOST_TEST((pair.first == chestnut.find('A')));
  BOOST_TEST(pair.second == true);

  pair = chestnut.emplace('A', 2.0f);
  BOOST_TEST((pair.first == chestnut.find('A')));
  BOOST_TEST(pair.second == false);

  pair = chestnut.emplace(std::pair< int, char >('B', 2.0f));
  BOOST_TEST(chestnut.size() == 2);
  BOOST_TEST((pair.first == std::next(chestnut.begin(), 1)));
}

BOOST_AUTO_TEST_CASE(emplace_hint)
{
  Tree< double, int > alder;
  alder = { { 1.0, 1 }, { 3.0, 3 }, { 4.0, 4 }, { 5.0, 5 } };

  auto it = alder.emplaceHint(++alder.begin(), 2.0, 2);
  BOOST_TEST(alder.size() == 5);
  BOOST_TEST((it == alder.find(2.0)));

  it = alder.emplaceHint(++alder.begin(), 2.0, 9);
  BOOST_TEST(alder.size() == 5);
  BOOST_TEST((it == alder.find(2.0)));
}

BOOST_AUTO_TEST_CASE(erase)
{
  Tree< short, short > linden = { { 1, 1 }, { 2, 2 }, { 3, 3 } };

  auto it = linden.erase(linden.find(2));
  BOOST_TEST(linden.size() == 2);
  BOOST_TEST((it == linden.find(3)));

  size_t erased = linden.erase(3);
  BOOST_TEST(linden.size() == 1);
  BOOST_TEST(erased == 1);

  erased = linden.erase(3);
  BOOST_TEST(linden.size() == 1);
  BOOST_TEST(erased == 0);

  it = linden.erase(linden.begin());
  BOOST_TEST(linden.empty());
  BOOST_TEST((it == linden.end()));

  Tree< short, short > poplar;
  std::vector< short > keys(7);
  keys = { 1, 2, 3, 7, 6, 4, 5 };
  do
  {
    for (size_t i = 0; i < keys.size(); ++i)
    {
      poplar.insert({ keys[i], 0 });
    }
    for (size_t i = 0; i < keys.size(); ++i)
    {
      poplar.erase(keys[i]);
    }
  }
  while (std::next_permutation(keys.begin(), keys.end()));
}

BOOST_AUTO_TEST_CASE(erase_range)
{
  Tree< int, int > juniper;
  juniper = { { 1, 1 }, { 2, 2 }, { 3, 3 }, { 4, 4 }, { 5, 5 } };

  auto it1 = juniper.erase(juniper.find(2), juniper.find(5));
  BOOST_TEST(juniper.size() == 2);
  BOOST_TEST((it1 == juniper.find(5)));

  auto it2 = juniper.erase(juniper.cbegin(), juniper.cend());
  BOOST_TEST(juniper.empty());
  BOOST_TEST((it2 == juniper.end()));
}

BOOST_AUTO_TEST_CASE(clear)
{
  Tree< int, char > willow;

  willow.clear();
  BOOST_TEST(willow.empty());

  willow = { { 1, 'm' }, { 2, 'n' }, { 3, 'k' } };
  willow.clear();
  BOOST_TEST(willow.empty());

  willow.clear();
  BOOST_TEST(willow.empty());
}

BOOST_AUTO_TEST_CASE(swap)
{
  Tree< int, char > cedar;
  Tree< int, char > cypress;

  cypress.swap(cedar);
  cedar.swap(cypress);

  cedar = { { 1, 'a' }, { 2, 'b' } };
  Tree< int, char > cedarCopy(cedar);
  cypress.insert({ 3, 'c' });
  Tree< int, char > cypressCopy(cypress);
  cedar.swap(cypress);
  BOOST_TEST(std::equal(cedar.begin(), cedar.end(), cypressCopy.begin()));
  BOOST_TEST(std::equal(cypress.begin(), cypress.end(), cedarCopy.begin()));

  cedar.swap(cypress);
  BOOST_TEST(std::equal(cedar.begin(), cedar.end(), cedarCopy.begin()));
  BOOST_TEST(std::equal(cypress.begin(), cypress.end(), cypressCopy.begin()));
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(tree_lookup)

BOOST_AUTO_TEST_CASE(count)
{
  Tree< int, char > lilac;
  BOOST_TEST(lilac.count(100) == 0);

  lilac = { { 1, 'a' }, { 2, 'b' } };
  BOOST_TEST(lilac.count(1) == 1);
  BOOST_TEST(lilac.count(2) == 1);
  BOOST_TEST(lilac.count(100) == 0);

  lilac.erase(2);
  BOOST_TEST(lilac.count(1) == 1);
  BOOST_TEST(lilac.count(2) == 0);
}

BOOST_AUTO_TEST_CASE(find)
{
  Tree< unsigned int, unsigned int > sequoia;
  BOOST_TEST((sequoia.find(99) == sequoia.end()));

  sequoia = { { 1, 1 }, { 2, 2 }, { 3, 3 }, { 4, 4 }, { 5, 5 } };
  BOOST_TEST((sequoia.find(2) != sequoia.end()));
}

BOOST_AUTO_TEST_CASE(equal_range)
{
  Tree< double, double > palm;
  BOOST_TEST((palm.equalRange(2).first == palm.end()));
  BOOST_TEST((palm.equalRange(2).second == palm.end()));

  palm = { { 1.0, 1.0 }, { 2.0, 2.0 }, { 4.0, 4.0 } };
  BOOST_TEST((palm.equalRange(2).first != palm.equalRange(2).second));
  BOOST_TEST((palm.equalRange(3).first == palm.equalRange(3).second));
  BOOST_TEST((palm.equalRange(3).second == std::next(palm.begin(), 2)));
}

BOOST_AUTO_TEST_CASE(lower_upper_bound)
{
  Tree< int, int > spruce;
  BOOST_TEST((spruce.lowerBound(99) == spruce.end()));
  BOOST_TEST((spruce.upperBound(99) == spruce.end()));

  std::vector< int > keys = { 1, 2, 4, 6, 10, 11, 12, 14 };
  for (size_t i = 0; i < keys.size(); ++i)
  {
    spruce.insert({ keys[i], 0 });
  }
  BOOST_TEST(spruce.lowerBound(3)->first == 4);
  BOOST_TEST(spruce.lowerBound(4)->first == 4);
  BOOST_TEST(spruce.upperBound(13)->first == 14);
  BOOST_TEST(spruce.upperBound(12)->first == 14);
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_CASE(equal_not_equal_operators)
{
  Tree< int, int > aspen;
  Tree< int, int > maple;
  BOOST_TEST((aspen == maple));

  aspen = { { 1, 1 }, { 2, 2 } };
  BOOST_TEST((aspen != maple));

  maple = aspen;
  BOOST_TEST((maple == aspen));
  BOOST_TEST((aspen == maple));

  maple.insert({ 3, 3 });
  BOOST_TEST((maple != aspen));
  BOOST_TEST((aspen != maple));
}

