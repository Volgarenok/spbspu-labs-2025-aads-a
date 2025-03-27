#ifndef SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_COMMON_QUEUE_HPP
#define SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_COMMON_QUEUE_HPP

#include <utility>
#include "buffer.hpp"
#include "type-utils.hpp"

namespace kizhin {
  template < typename T, typename Container = Buffer< T > >
  class Queue
  {
  public:
    using value_type = typename Container::value_type;
    using size_type = typename Container::size_type;
    using reference = typename Container::reference;
    using const_reference = typename Container::const_reference;

    Queue() noexcept(is_nothrow_default_constructible_v< Container >) = default;
    template < typename InputIt >
    Queue(InputIt, InputIt);

    size_type size() const noexcept;
    bool empty() const noexcept;
    const Container& container() const noexcept;

    reference front() noexcept;
    const_reference front() const noexcept;
    reference back() noexcept;
    const_reference back() const noexcept;
    void pop() noexcept;

    void push(const_reference);
    void push(value_type&&);
    template < typename... Args >
    void emplace(Args&&...);

    void swap(Queue&) noexcept(is_nothrow_swappable_v< Container >);

  private:
    Container container_;
  };

  template < typename T, typename C >
  template < typename InputIt >
  Queue< T, C >::Queue(InputIt first, InputIt last):
    container_(first, last)
  {}

  template < typename T, typename C >
  typename Queue< T, C >::size_type Queue< T, C >::size() const noexcept
  {
    return container_.size();
  }

  template < typename T, typename C >
  bool Queue< T, C >::empty() const noexcept
  {
    return container_.empty();
  }

  template < typename T, typename C >
  const C& Queue< T, C >::container() const noexcept
  {
    return container_;
  }

  template < typename T, typename C >
  typename Queue< T, C >::reference Queue< T, C >::front() noexcept
  {
    return container_.front();
  }

  template < typename T, typename C >
  typename Queue< T, C >::const_reference Queue< T, C >::front() const noexcept
  {
    return static_cast< const C& >(container_).front();
  }

  template < typename T, typename C >
  typename Queue< T, C >::reference Queue< T, C >::back() noexcept
  {
    return container_.back();
  }

  template < typename T, typename C >
  typename Queue< T, C >::const_reference Queue< T, C >::back() const noexcept
  {
    return static_cast< const C& >(container_).back();
  }

  template < typename T, typename C >
  void Queue< T, C >::pop() noexcept
  {
    container_.popFront();
  }

  template < typename T, typename C >
  void Queue< T, C >::push(const_reference value)
  {
    container_.pushBack(value);
  }

  template < typename T, typename C >
  void Queue< T, C >::push(value_type&& value)
  {
    container_.pushBack(std::move(value));
  }

  template < typename T, typename C >
  template < typename... Args >
  void Queue< T, C >::emplace(Args&&... args)
  {
    container_.emplaceBack(std::forward< Args >(args)...);
  }

  template < typename T, typename C >
  void Queue< T, C >::swap(Queue& rhs) noexcept(is_nothrow_swappable_v< C >)
  {
    using std::swap;
    swap(container_, rhs.container_);
  }

  template < typename T, typename C >
  bool operator==(const Queue< T, C >& lhs, const Queue< T, C >& rhs)
  {
    return lhs.container() == rhs.container();
  }

  template < typename T, typename C >
  bool operator!=(const Queue< T, C >& lhs, const Queue< T, C >& rhs)
  {
    return !(lhs == rhs);
  }

  template < typename T, typename C >
  bool operator<(const Queue< T, C >& lhs, const Queue< T, C >& rhs)
  {
    return lhs.container() < rhs.container();
  }

  template < typename T, typename C >
  bool operator>(const Queue< T, C >& lhs, const Queue< T, C >& rhs)
  {
    return rhs < lhs;
  }

  template < typename T, typename C >
  bool operator<=(const Queue< T, C >& lhs, const Queue< T, C >& rhs)
  {
    return !(rhs < lhs);
  }

  template < typename T, typename C >
  bool operator>=(const Queue< T, C >& lhs, const Queue< T, C >& rhs)
  {
    return !(lhs < rhs);
  }

  template < typename T, typename C >
  void swap(Queue< T, C >& lhs, Queue< T, C >& rhs) noexcept(noexcept(lhs.swap(rhs)))
  {
    lhs.swap(rhs);
  }
}

#endif

