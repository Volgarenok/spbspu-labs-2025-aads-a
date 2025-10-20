#include <boost/test/unit_test.hpp>
#include "fwd_list.hpp"

BOOST_AUTO_TEST_CASE(test_constructors_and_basic_operations)
{
  karnauhova::FwdList< int > empty_list;
  BOOST_TEST(empty_list.empty());
  BOOST_TEST(empty_list.size() == 0);

  karnauhova::FwdList< int > init_list{1, 2, 3};
  BOOST_TEST(init_list.size() == 3);
  BOOST_TEST(init_list.front() == 1);

  karnauhova::FwdList< int > copy_list(init_list);
  BOOST_TEST(copy_list == init_list);

  karnauhova::FwdList< int > temp_list{1, 2, 3};
  karnauhova::FwdList< int > moved_list(std::move(temp_list));
  BOOST_TEST(moved_list.size() == 3);
  BOOST_TEST(temp_list.empty());
}

BOOST_AUTO_TEST_CASE(test_element_access_and_modification)
{
  karnauhova::FwdList< int > list;

  BOOST_CHECK_THROW(list.front(), std::out_of_range);

  list.push_front(42);
  BOOST_TEST(list.front() == 42);

  list.push_front(1);
  list.push_front(2);
  BOOST_TEST(list.front() == 2);
  list.pop_front();
  BOOST_TEST(list.front() == 1);
  list.pop_front();
  BOOST_TEST(list.front() == 42);

  list.clear();
  BOOST_TEST(list.empty());
}

BOOST_AUTO_TEST_CASE(test_iterators_and_comparisons)
{
  karnauhova::FwdList< int > list{1, 2, 3};

  auto it = list.begin();
  BOOST_TEST(*it == 1);
  ++it;
  BOOST_TEST(*it == 2);
  BOOST_TEST((it != list.end()));

  auto cit = list.cbegin();
  BOOST_TEST(*cit == 1);
  ++cit;
  BOOST_TEST(*cit == 2);

  karnauhova::FwdList< int > equal_list{1, 2, 3};
  karnauhova::FwdList< int > greater_list{1, 2, 4};
  karnauhova::FwdList< int > shorter_list{1, 2};

  BOOST_TEST(list == equal_list);
  BOOST_TEST(list != greater_list);
  BOOST_TEST(list < greater_list);
  BOOST_TEST(greater_list > list);
  BOOST_TEST(shorter_list <= list);
  BOOST_TEST(list >= shorter_list);
}

BOOST_AUTO_TEST_CASE(test_removal_operations)
{
  karnauhova::FwdList< int > list{1, 2, 3, 2, 4, 2};

  list.remove(2);
  BOOST_TEST(list.size() == 3);
  BOOST_TEST(*list.begin() == 1);

  struct IsEven
  {
    bool operator()(int x) const
    {
      return x % 2 == 0;
    }
  };
  list = {1, 2, 3, 4, 5};
  list.remove_if(IsEven());
  BOOST_TEST(list.size() == 3);
}

BOOST_AUTO_TEST_CASE(test_assignment_operations)
{
  karnauhova::FwdList< int > list1{1, 2, 3};
  karnauhova::FwdList< int > list2;

  list2 = {4, 5, 6};
  BOOST_TEST(list2.size() == 3);
  BOOST_TEST(list2.front() == 4);

  list2 = list1;
  BOOST_TEST(list1 == list2);
  list2.pop_front();
  BOOST_TEST(list2.front() == 2);
  BOOST_TEST(list1.front() == 1);

  list1 = list1;
  BOOST_TEST(list1.size() == 3);
  BOOST_TEST(list1.front() == 1);

  karnauhova::FwdList< int > src{4, 5, 6};
  karnauhova::FwdList< int > dst;
  dst = std::move(src);
  BOOST_TEST(dst.size() == 3);
  BOOST_TEST(dst.front() == 4);
  BOOST_TEST(src.empty());
}

BOOST_AUTO_TEST_CASE(test_insert_and_erase)
{
  karnauhova::FwdList< int > list{1, 3};

  auto it1 = list.begin();
  ++it1;
  list.insert(it1, 2);
  BOOST_TEST(list.size() == 3);

  auto it2 = list.begin();
  list.insert(it2, 0);
  BOOST_TEST(list.size() == 4);

  auto it3 = list.begin();
  list.erase(it3);
  BOOST_TEST(list.size() == 3);
}

BOOST_AUTO_TEST_CASE(test_splice_and_assign)
{
  karnauhova::FwdList< int > list1{1, 2};
  karnauhova::FwdList< int > list2{3, 4, 5};

  auto pos = list1.begin();
  ++pos;
  list1.splice(pos, list2);
  BOOST_TEST(list1.size() == 5);
  BOOST_TEST(list2.empty());
  BOOST_TEST(list1.front() == 1);

  std::vector<int> vec{11, 12, 13};
  list1.assign(vec.begin(), vec.end());
  BOOST_TEST(list1.size() == 3);
  BOOST_TEST(list1.front() == 11);

  list1.assign({14, 15, 16, 17});
  BOOST_TEST(list1.size() == 4);
  BOOST_TEST(list1.front() == 14);
  BOOST_TEST(list1.back() == 17);
}

BOOST_AUTO_TEST_CASE(test_reverse_and_swap)
{
  karnauhova::FwdList< int > list{1, 2, 3};

  list.reverse();
  auto it = list.begin();
  BOOST_TEST(*it == 3);
  BOOST_TEST(*++it == 2);
  BOOST_TEST(*++it == 1);

  karnauhova::FwdList< int > other{4, 5};
  list.swap(other);
  BOOST_TEST(list.size() == 2);
  BOOST_TEST(other.size() == 3);
  BOOST_TEST(list.front() == 4);
  BOOST_TEST(other.front() == 3);
}
