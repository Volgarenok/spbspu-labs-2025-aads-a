#ifndef TEST_UTILS
#define TEST_UTILS

#include <functional>
#include <iterator>
#include <boost/test/unit_test.hpp>
#include <list.hpp>

namespace rychkov
{
  template< class T >
  void fullCompare(const List< T > left, const List< T > right)
  {
    fullCompare(left, right, std::equal_to<>());
  }
  template< class T, class P >
  void fullCompare(const List< T > left, const List< T > right, P predicate)
  {
    BOOST_TEST(left.size() == right.size());
    if (!left.empty() && !right.empty())
    {
      BOOST_TEST(left.front() == right.front());
      BOOST_TEST(left.back() == right.back());
    }
    {
      typename decltype(left)::const_iterator i = left.begin(), j = right.begin();
      for (; (i != left.end()) && (j != right.end()); ++i, ++j)
      {
        BOOST_TEST(predicate(*i, *j));
      }
      BOOST_TEST((i == left.end()));
      BOOST_TEST((j == right.end()));
    }
    {
      typename decltype(left)::const_reverse_iterator i = left.rbegin(), j = right.rbegin();
      for (; (i != left.rend()) && (j != right.rend()); ++i, ++j)
      {
        BOOST_TEST(predicate(*i, *j));
      }
      BOOST_TEST((i == left.rend()));
      BOOST_TEST((j == right.rend()));
    }
  }
  template< class Iter >
  Iter shift(Iter i, typename std::iterator_traits< Iter >::difference_type distance)
  {
    std::advance(i, distance);
    return i;
  }
}

#endif
