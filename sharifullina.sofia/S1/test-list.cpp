леш эти модульные тесты говно или пойдет?

#include "list.hpp"
#include <boost/test/included/unit_test.hpp>
#include <sstream>
#include <string>

namespace sharifullina
{
  template<typename T>
  std::string listToString(const List<T>& list)
  {
    std::ostringstream oss;
    for (auto it = list.begin(); it != list.end(); ++it)
    {
      if (it != list.begin())
      {
        oss << " ";
      }
      oss << *it;
    }
    return oss.str();
  }

  void testDefaultConstructor()
  {
    List<int> list;
    BOOST_TEST(list.empty());
    BOOST_TEST(list.size() == 0);
  }

  void testPushBack()
  {
    List<int> list;
    list.pushBack(1);
    BOOST_TEST(!list.empty());
    BOOST_TEST(list.size() == 1);
    BOOST_TEST(list.front() == 1);
    BOOST_TEST(list.back() == 1);

    list.pushBack(2);
    BOOST_TEST(list.size() == 2);
    BOOST_TEST(list.front() == 1);
    BOOST_TEST(list.back() == 2);

    list.pushBack(3);
    BOOST_TEST(list.size() == 3);
    BOOST_TEST(list.front() == 1);
    BOOST_TEST(list.back() == 3);
  }

  void testPushFront()
  {
    List<int> list;
    list.pushFront(1);
    BOOST_TEST(!list.empty());
    BOOST_TEST(list.size() == 1);
    BOOST_TEST(list.front() == 1);
    BOOST_TEST(list.back() == 1);

    list.pushFront(2);
    BOOST_TEST(list.size() == 2);
    BOOST_TEST(list.front() == 2);
    BOOST_TEST(list.back() == 1);

    list.pushFront(3);
    BOOST_TEST(list.size() == 3);
    BOOST_TEST(list.front() == 3);
    BOOST_TEST(list.back() == 1);
  }

  void testPopBack()
  {
    List<int> list;
    list.pushBack(1);
    list.pushBack(2);
    list.pushBack(3);

    list.popBack();
    BOOST_TEST(list.size() == 2);
    BOOST_TEST(list.back() == 2);

    list.popBack();
    BOOST_TEST(list.size() == 1);
    BOOST_TEST(list.back() == 1);

    list.popBack();
    BOOST_TEST(list.empty());
    BOOST_TEST(list.size() == 0);

    list.popBack(); // Should not crash
    BOOST_TEST(list.empty());
  }

  void testPopFront()
  {
    List<int> list;
    list.pushBack(1);
    list.pushBack(2);
    list.pushBack(3);

    list.popFront();
    BOOST_TEST(list.size() == 2);
    BOOST_TEST(list.front() == 2);

    list.popFront();
    BOOST_TEST(list.size() == 1);
    BOOST_TEST(list.front() == 3);

    list.popFront();
    BOOST_TEST(list.empty());
    BOOST_TEST(list.size() == 0);

    list.popFront(); // Should not crash
    BOOST_TEST(list.empty());
  }

  void testClear()
  {
    List<int> list;
    for (int i = 0; i < 5; ++i)
    {
      list.pushBack(i);
    }
    BOOST_TEST(list.size() == 5);

    list.clear();
    BOOST_TEST(list.empty());
    BOOST_TEST(list.size() == 0);

    list.clear(); // Should not crash
    BOOST_TEST(list.empty());
  }

  void testFrontBack()
  {
    List<int> list;
    list.pushBack(1);
    BOOST_TEST(list.front() == 1);
    BOOST_TEST(list.back() == 1);

    list.pushBack(2);
    BOOST_TEST(list.front() == 1);
    BOOST_TEST(list.back() == 2);

    list.pushFront(0);
    BOOST_TEST(list.front() == 0);
    BOOST_TEST(list.back() == 2);
  }

  void testIterators()
  {
    List<int> list;
    list.pushBack(1);
    list.pushBack(2);
    list.pushBack(3);

    auto it = list.begin();
    BOOST_TEST(*it == 1);
    ++it;
    BOOST_TEST(*it == 2);
    it++;
    BOOST_TEST(*it == 3);
    ++it;
    BOOST_TEST(it == list.end());

    const List<int>& constList = list;
    auto cit = constList.begin();
    BOOST_TEST(*cit == 1);
    ++cit;
    BOOST_TEST(*cit == 2);

    auto rit = list.end();
    --rit;
    BOOST_TEST(*rit == 3);
    --rit;
    BOOST_TEST(*rit == 2);
    --rit;
    BOOST_TEST(*rit == 1);
  }

  void testCopyConstructor()
  {
    List<int> list1;
    list1.pushBack(1);
    list1.pushBack(2);
    list1.pushBack(3);

    List<int> list2(list1);
    BOOST_TEST(list2.size() == 3);
    BOOST_TEST(list2.front() == 1);
    BOOST_TEST(list2.back() == 3);

    auto it1 = list1.begin();
    auto it2 = list2.begin();
    while (it1 != list1.end() && it2 != list2.end())
    {
      BOOST_TEST(*it1 == *it2);
      ++it1;
      ++it2;
    }
  }

  void testMoveConstructor()
  {
    List<int> list1;
    list1.pushBack(1);
    list1.pushBack(2);
    list1.pushBack(3);

    List<int> list2(std::move(list1));
    BOOST_TEST(list1.empty());
    BOOST_TEST(list2.size() == 3);
    BOOST_TEST(list2.front() == 1);
    BOOST_TEST(list2.back() == 3);
  }

  void testCopyAssignment()
  {
    List<int> list1;
    list1.pushBack(1);
    list1.pushBack(2);
    list1.pushBack(3);

    List<int> list2;
    list2 = list1;
    BOOST_TEST(list2.size() == 3);
    BOOST_TEST(list2.front() == 1);
    BOOST_TEST(list2.back() == 3);

    list2 = list2;
    BOOST_TEST(list2.size() == 3);
  }

  void testMoveAssignment()
  {
    List<int> list1;
    list1.pushBack(1);
    list1.pushBack(2);
    list1.pushBack(3);

    List<int> list2;
    list2 = std::move(list1);
    BOOST_TEST(list1.empty());
    BOOST_TEST(list2.size() == 3);
    BOOST_TEST(list2.front() == 1);
    BOOST_TEST(list2.back() == 3);
  }

  void testSwap()
  {
    List<int> list1;
    list1.pushBack(1);
    list1.pushBack(2);

    List<int> list2;
    list2.pushBack(3);
    list2.pushBack(4);
    list2.pushBack(5);

    list1.swap(list2);
    BOOST_TEST(list1.size() == 3);
    BOOST_TEST(list2.size() == 2);
    BOOST_TEST(list1.front() == 3);
    BOOST_TEST(list2.front() == 1);
  }

  void testListOutput()
  {
    List<int> list;
    list.pushBack(1);
    list.pushBack(2);
    list.pushBack(3);

    BOOST_TEST(listToString(list) == "1 2 3");

    list.clear();
    list.pushFront(3);
    list.pushFront(2);
    list.pushFront(1);
    BOOST_TEST(listToString(list) == "1 2 3");
  }

  void testComplexOperations()
  {
    List<int> list;
    list.pushBack(1);
    list.pushFront(0);
    list.pushBack(2);
    list.pushBack(3);
    list.popFront();
    list.popBack();

    BOOST_TEST(listToString(list) == "1 2");
    BOOST_TEST(list.size() == 2);

    list.clear();
    list.pushBack(5);
    list.pushBack(6);
    BOOST_TEST(listToString(list) == "5 6");
  }
}
