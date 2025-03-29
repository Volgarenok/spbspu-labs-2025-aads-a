#ifndef MEM_CHECKER_HPP
#define MEM_CHECKER_HPP

#include <cstddef>
#include <utility>
#include <type_traits>
#include <boost/test/unit_test.hpp>

namespace rychkov
{
  template< class Base >
  size_t active_tracked();
  template< class Base >
  class Counter
  {
  public:
    Counter()
    {
      active++;
    }
    Counter(const Counter&)
    {
      active++;
    }
    ~Counter()
    {
      active--;
    }

    Counter& operator=(const Counter&) = default;
  private:
    static size_t active;
    friend size_t active_tracked< Base >();
  };
  template< class Base >
  class MemChecker: public Base, private Counter< Base >
  {
  public:
    MemChecker() = default;
    template< class... Args, class = std::enable_if_t< std::is_constructible< Base, Args&&... >::value > >
    MemChecker(Args&&... args):
      Base(std::forward< Args >(args)...)
    {}
    template< class T, class = std::enable_if_t< std::is_assignable< Base, T&& >::value > >
    MemChecker& operator=(T&& value)
    {
      static_cast< Base& >(*this) = std::forward< T >(value);
      return *this;
    }
  };
  template< class Base >
  size_t Counter< Base >::active = 0;
  template< class Base >
  size_t active_tracked()
  {
    return Counter< Base >::active;
  }
  template< class... Types >
  struct MemTrack
  {
    MemTrack() = default;
    MemTrack(const MemTrack&) = delete;
    ~MemTrack()
    {
      size_t sizes[] = {active_tracked< Types >()...};
      for (size_t i: sizes)
      {
        BOOST_TEST(i == 0);
      }
    }
  };
}

#endif
