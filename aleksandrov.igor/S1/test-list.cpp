#include <boost/test/unit_test.hpp>
#include <vector>
#include "list.hpp"

using aleksandrov::List;

BOOST_AUTO_TEST_SUITE(list_construction);

BOOST_AUTO_TEST_CASE(default_construction)
{
  List< std::vector< List < int > >* > list;
  BOOST_TEST(list.size() == 0);
}

BOOST_AUTO_TEST_CASE(copy_construction)
{
  List< int > list{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  List< int > numbers(list);
  BOOST_TEST(numbers.size() == 10);
}

BOOST_AUTO_TEST_CASE(move_construction)
{
  List< std::vector< short > > list{{1, 2}, {3, 4, 5}};
  List< std::vector< short > > numbers(std::move(list));
  BOOST_TEST(numbers.size() == 2);
  BOOST_TEST(numbers.front().size() == 2);
  BOOST_TEST(numbers.back().size() == 3);
  BOOST_TEST(list.empty());
}

BOOST_AUTO_TEST_CASE(fill_construction)
{
  List< std::string > strings(10, "fill");
  BOOST_TEST(strings.size() == 10);
}

BOOST_AUTO_TEST_CASE(initializer_list_construction)
{
  List< std::pair< int, std::string > > pairs = {{1, "1"}, {2, "2"}, {3, "3"}};
  BOOST_TEST(pairs.size() == 3);
  BOOST_TEST(pairs.front().first == 1);
  BOOST_TEST(pairs.back().second == "3");
}

BOOST_AUTO_TEST_CASE(range_construction)
{
  std::vector< char > vector{'+', '-', '*', '/', '%'};
  List< char > operations(vector.begin() + 1, vector.end());
  BOOST_TEST(operations.size() == 4);
  BOOST_TEST(operations.front() == '-');
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(list_assignment);

BOOST_AUTO_TEST_CASE(copy_assignment)
{
  List< List< int > > matrix = {{1, 2}, {3, 4}};
  List< List< int > > yaMatrix;
  yaMatrix = matrix;
  BOOST_TEST(yaMatrix.size() == 2);
  BOOST_TEST(yaMatrix.front().front() == 1);
  matrix.front().front() = 5;
  BOOST_TEST(yaMatrix.front().front() == 1);
}

BOOST_AUTO_TEST_CASE(move_assignment)
{
  List< std::vector< int > > matrix{{1, 2}, {3, 4}};
  List< std::vector< int > > newMatrix;
  newMatrix = std::move(matrix);
  BOOST_TEST(newMatrix.front()[0] == 1);
  BOOST_TEST(matrix.empty());
}

BOOST_AUTO_TEST_CASE(initializer_list_assignment)
{
  List< int > primes;
  primes = {2, 3, 5, 7};
  BOOST_TEST(primes.size() == 4);
  BOOST_TEST(primes.back() == 7);
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(iterators);

BOOST_AUTO_TEST_CASE(begin)
{
  List< int > list;
  list = {10, 20, 30};
  BOOST_TEST(*list.cbegin() == 10);
  *list.begin() = 30;
  BOOST_TEST(*list.cbegin() == 30);

  list.emplaceBack(40);
  auto min = std::min_element(list.begin(), list.end());
  BOOST_TEST(*min == 20);
}

BOOST_AUTO_TEST_CASE(end)
{
  List< int > list;
  list = {11, 22, 33};
  auto it = list.cbegin();
  std::advance(it, 3);
  BOOST_TEST(true);
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(list_element_access);

BOOST_AUTO_TEST_CASE(front)
{
  List< std::string > labels(1, "comedy artist");
  BOOST_TEST(labels.front() == "comedy artist");
  labels.emplaceFront("9/9");
  BOOST_TEST(labels.front() == "9/9");
}

BOOST_AUTO_TEST_CASE(back)
{
  List< std::string > labels(2, "fine");
  BOOST_TEST(labels.back() == "fine");
  labels.emplaceBack("trash");
  BOOST_TEST(labels.back() == "trash");
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(list_capacity)

BOOST_AUTO_TEST_CASE(empty)
{
  List< unsigned char > functions;
  BOOST_TEST(functions.empty());
  functions = {'x', 'y', 'z', 'w'};
  BOOST_TEST(!functions.empty());
  functions.clear();
  BOOST_TEST(functions.empty());
}

BOOST_AUTO_TEST_CASE(size)
{
  List< double > list;
  BOOST_TEST(list.size() == 0);
  list.emplaceFront(3.14159);
  BOOST_TEST(list.size() != 0);
  list.emplaceFront(2.71828);
  BOOST_TEST(list.size() == 2);
  list.clear();
  BOOST_TEST(list.size() == 0);
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(list_modifiers);

BOOST_AUTO_TEST_CASE(push_front)
{
  List< float > list;
  float value = 1.0f;
  list.pushFront(value);
  BOOST_TEST(list.front() == 1.0f);
  list.pushFront(3.14f);
  BOOST_TEST(list.front() == 3.14f);
}

BOOST_AUTO_TEST_CASE(emplace_front)
{
  List< std::pair< int, std::string > > list;
  std::pair< int, std::string > pair(1, "banana");
  list.emplaceFront(pair);
  BOOST_TEST(list.front().first == 1);
  BOOST_TEST(list.front().second == "banana");

  list.emplaceFront(std::make_pair(2, "avocado"));
  BOOST_TEST(list.front().first == 2);
}

BOOST_AUTO_TEST_CASE(push_back)
{
  List< float > list;
  float value = 2.5f;
  list.pushBack(value);
  BOOST_TEST(list.back() == 2.5f);
  list.pushBack(5.0f);
  BOOST_TEST(list.back() == 5.0f);
}

BOOST_AUTO_TEST_CASE(emplace_back)
{
  List< std::pair< char, char > > list;
  std::pair< char, char > pair('A', 'B');
  list.emplaceBack(pair);
  BOOST_TEST(list.front().first == 'A');
  BOOST_TEST(list.front().second == 'B');
  list.emplaceBack(std::make_pair('C', 'D'));
  BOOST_TEST(list.back().first == 'C');
}

BOOST_AUTO_TEST_CASE(pop_front)
{
  List< int > list{123, 456};
  list.popFront();
  BOOST_TEST(list.front() == 456);
}

BOOST_AUTO_TEST_CASE(pop_back)
{
  List< double > list{1000};
  list.popBack();
  BOOST_TEST(list.empty());
  list.pushBack(1);
  list.pushBack(2);
  list.popBack();
  BOOST_TEST(list.size() == 1);
}

BOOST_AUTO_TEST_CASE(swap)
{
  List< int > a{1, 2};
  List< int > b{3, 4, 5};
  a.swap(b);
  BOOST_TEST(a.size() == 3);
  BOOST_TEST(b.size() == 2);
}

BOOST_AUTO_TEST_CASE(clear)
{
  List< std::vector< char > > buffers;
  buffers.clear();
  buffers.clear();
  buffers.pushBack({'a', 'b'});
  buffers.clear();
  BOOST_TEST(buffers.empty());
}

BOOST_AUTO_TEST_CASE(assign_fill)
{
  List< char > list1(2, 'a');
  list1.assign(5, 'b');
  BOOST_TEST(list1.size() == 5);
}

BOOST_AUTO_TEST_CASE(assign_range)
{
  List< int > list{5, 6, 7};
  List< int > numbers;
  numbers.assign(list.cbegin(), list.cend());
  BOOST_TEST(numbers.size() == 3);
}

BOOST_AUTO_TEST_CASE(assign_initializer_list)
{
  List< char > list(1, 'd');
  list.assign({'e', 'f', 'g'});
  BOOST_TEST(list.size() == 3);
}

BOOST_AUTO_TEST_CASE(insert)
{
  List< int > list{100};

  const int value1 = 200;
  auto it1 = list.insertAfter(list.cbegin(), value1);
  BOOST_TEST(*it1 == 200);

  auto it2 = list.insertAfter(std::next(list.cbegin(), 1), 300);
  BOOST_TEST(*it2 == 300);

  auto it3 = list.insertAfter(list.cbegin(), 2ull, 150);
  BOOST_TEST(*it3 == 150);

  List< int > numbers{175, 225};
  auto it4 = list.insertAfter(std::next(list.cbegin(), 3), numbers.begin(), numbers.end());
  BOOST_TEST(*it4 == 225);

  auto it5 = list.insertAfter(std::next(list.cbegin(), 5), {250, 275});
  BOOST_TEST(*it5 == 275);

  List< int > empty;
  auto it6 = list.insertAfter(list.cbegin(), empty.begin(), empty.end());
  BOOST_TEST(*it6 == list.front());

  auto it7 = list.insertAfter(list.cbegin(), {});
  BOOST_TEST(*it7 == list.front());

  auto it8 = list.insertAfter(std::next(list.cbegin(), 5), 400);
  BOOST_TEST(*it8 == 400);

  auto it9 = list.insertAfter(list.cbegin(), 0ull, 999);
  BOOST_TEST(*it9 == list.front());

  std::vector< int > expected{100, 150, 150, 200, 175, 225, 300, 250, 275, 400};
  BOOST_TEST(list.size() == expected.size());
  BOOST_TEST(std::equal(list.begin(), list.end(), expected.begin()));
}

BOOST_AUTO_TEST_CASE(emplace)
{
  List< char > brackets{'(', '}', '['};
  auto it = brackets.emplaceAfter(brackets.cbegin(), ')');
  BOOST_TEST(*it == ')');
  BOOST_TEST(brackets.front() == '(');
  BOOST_TEST(brackets.size() == 4);
}

BOOST_AUTO_TEST_CASE(erase)
{
  List< int > list1{1, 2, 3, 4, 5};
  auto it1 = list1.eraseAfter(list1.cbegin());
  BOOST_TEST(*it1 == 3);
  BOOST_TEST(list1.size() == 4);

  auto first = list1.cbegin();
  auto last = std::next(first, 2);
  it1 = list1.eraseAfter(first, last);
  BOOST_TEST(*it1 == 4);
  BOOST_TEST(list1.size() == 3);

  List< int > list2{6, 7, 8, 9};
  list2.eraseAfter(std::next(list2.cbegin(), 3));
  BOOST_TEST(list2.size() == 4);

  List< int > list3{10, 11, 12, 13, 14};
  auto it3 = list3.eraseAfter(list3.cbegin(), std::next(list3.cbegin(), 4));
  BOOST_TEST(*it3 == 14);
  BOOST_TEST(list3.size() == 3);

  List< int > list4{15, 16};
  list4.eraseAfter(list4.cbegin());
  BOOST_TEST(list4.size() == 1);
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_CASE(list_bool_operators)
{
  List< int > empty;
  List< int > nums1{1, 2, 3};
  List< int > nums2{1, 2, 3};
  List< int > nums3{1, 2};
  List< int > nums4{1, 2, 4};
  BOOST_TEST(empty != nums1);
  BOOST_TEST(nums1 == nums2);
  BOOST_TEST(nums3 < nums1);
  BOOST_TEST(nums1 <= nums2);
  BOOST_TEST(nums4 > nums1);
  BOOST_TEST(nums1 >= nums3);
}

BOOST_AUTO_TEST_SUITE(list_operations);

BOOST_AUTO_TEST_CASE(remove)
{
  List< unsigned int > list{1, 3, 3, 2, 3, 4};
  list.remove(3);
  BOOST_TEST(list.size() == 3);
  BOOST_TEST(list.front() == 1);
  BOOST_TEST(list.back() == 4);

  list.remove(0);
  BOOST_TEST(list.size() == 3);
}

BOOST_AUTO_TEST_CASE(remove_if)
{
  List< unsigned int > list{1, 2, 4};
  auto even = [](unsigned int value)
  {
    return value % 2 == 0;
  };
  list.removeIf(even);
  BOOST_TEST(list.size() == 1);
  BOOST_TEST(list.front() == 1);
}

BOOST_AUTO_TEST_CASE(splice)
{
  List< int > list1{1, 2};
  List< int > list2{3, 4};
  list1.spliceAfter(list1.cend(), list2);
  BOOST_TEST(list1.size() == 4);
  BOOST_TEST(list2.empty());

  List< int > temp1{5, 6};
  list1.spliceAfter(list1.cend(), std::move(temp1));
  BOOST_TEST(list1.size() == 6);
  BOOST_TEST(temp1.empty());

  List< int > list3{7, 8};
  list1.spliceAfter(list1.cend(), list3, list3.cbegin());
  BOOST_TEST(list1.size() == 7);
  BOOST_TEST(list3.size() == 1);

  List< int > temp2{9, 10};
  list1.spliceAfter(list1.cend(), std::move(temp2), temp2.cbegin());
  BOOST_TEST(list1.size() == 8);
  BOOST_TEST(temp2.size() == 1);

  List< int > list4{11, 12, 13};
  list1.spliceAfter(list1.cend(), list4, list4.cbegin(), std::next(list4.cbegin(), 2));
  BOOST_TEST(list1.size() == 9);
  BOOST_TEST(list4.size() == 2);

  List< int > temp3{14, 15, 16};
  list1.spliceAfter(list1.cend(), std::move(temp3), temp3.cbegin(), temp3.cend());
  BOOST_TEST(list1.size() == 11);
  BOOST_TEST(temp3.size() == 1);

  std::vector< int > expected{15, 16, 12, 10, 8, 5, 6, 3, 4, 1, 2};
  BOOST_TEST(std::equal(list1.begin(), list1.end(), expected.begin()));
}

BOOST_AUTO_TEST_CASE(reverse)
{
  List< std::pair< float, float > > coords{{1.0f, 1.0f}, {2.0f, 2.0f}, {3.0f, 3.0f}};
  coords.reverse();
  BOOST_TEST(coords.front().first == 3.0f);
  coords.reverse();
  BOOST_TEST(coords.back().first == 3.0f);
}

BOOST_AUTO_TEST_CASE(sort)
{
  List< double > numbers{3.0, 1.0, 4.0, 2.0};
  numbers.sort();
  std::vector< double > expected{1.0, 2.0, 3.0, 4.0};
  BOOST_TEST(std::equal(numbers.begin(), numbers.end(), expected.begin()));
}

BOOST_AUTO_TEST_CASE(sort_comp)
{
  struct LastDigitComparator
  {
    bool operator()(int a, int b) const
    {
      return (a % 10) < (b % 10);
    }
  };
  List< int > list{25, 17, 32, 141, 59, 263, 178, 86, 94, 110};
  list.sort(LastDigitComparator());
  std::vector< int > expected{110, 141, 32, 263, 94, 25, 86, 17, 178, 59};
  BOOST_TEST(std::equal(list.begin(), list.end(), expected.begin()));
}

BOOST_AUTO_TEST_CASE(merge)
{
  List< int > list1{1, 3, 7};
  List< int > list2{2, 4, 6};
  list1.merge(list2);
  BOOST_TEST(list2.empty());
  std::vector< int > expected{1, 2, 3, 4, 6, 7};
  BOOST_TEST(std::equal(list1.begin(), list1.end(), expected.begin()));
}

BOOST_AUTO_TEST_CASE(merge_comp)
{
  List< int > list1{7, 3, 1};
  List< int > list2{6, 4, 2};
  list1.merge(list2, std::greater< int >());
  BOOST_TEST(list2.empty());
  std::vector< int > expected{7, 6, 4, 3, 2, 1};
  BOOST_TEST(std::equal(list1.begin(), list1.end(), expected.begin()));
}

BOOST_AUTO_TEST_CASE(unique)
{
  List< int > list{1, 1, 2, 3, 3, 3, 4};
  list.unique();
  std::vector< int > expected{1, 2, 3, 4};
  BOOST_TEST(std::equal(list.begin(), list.end(), expected.begin()));
}

BOOST_AUTO_TEST_CASE(unique_pred)
{
  struct EvenEqual
  {
    bool operator()(int a, int b) const
    {
      return (a % 2 == 0) && (b % 2 == 0);
    }
  };
  List< int > numbers{1, 4, 2, 5, 7, 8, 10, 13};
  numbers.unique(EvenEqual());
  std::vector< int > expected{1, 4, 5, 7, 8, 13};
  BOOST_TEST(std::equal(numbers.begin(), numbers.end(), expected.begin()));
}

BOOST_AUTO_TEST_SUITE_END();

