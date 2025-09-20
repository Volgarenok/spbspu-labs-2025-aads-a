#ifndef SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_COMMON_STACK_HPP
#define SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_COMMON_STACK_HPP

#include <utility>
#include "buffer.hpp"
#include "type-utils.hpp"

namespace kizhin {
  template < typename T, typename Container = Buffer< T > >
  class Stack
  {
  public:
    using value_type = typename Container::value_type;
    using size_type = typename Container::size_type;
    using reference = typename Container::reference;
    using const_reference = typename Container::const_reference;

    Stack() noexcept(is_nothrow_default_constructible_v< Container >) = default;
    template < typename InputIt >
    Stack(InputIt, InputIt);

    size_type size() const noexcept;
    bool empty() const noexcept;
    const Container& container() const noexcept;

    reference top() noexcept;
    const_reference top() const noexcept;
    void pop() noexcept;

    void push(const_reference);
    void push(value_type&&);
    template < typename... Args >
    void emplace(Args&&...);

    void swap(Stack&) noexcept(is_nothrow_swappable_v< Container >);

  private:
    Container container_;
  };

  template < typename T, typename C >
  template < typename InputIt >
  Stack< T, C >::Stack(InputIt first, InputIt last):
    container_(first, last)
  {}

  template < typename T, typename C >
  typename Stack< T, C >::size_type Stack< T, C >::size() const noexcept
  {
    return container_.size();
  }

  template < typename T, typename C >
  bool Stack< T, C >::empty() const noexcept
  {
    return container_.empty();
  }

  template < typename T, typename C >
  const C& Stack< T, C >::container() const noexcept
  {
    return container_;
  }

  template < typename T, typename C >
  typename Stack< T, C >::reference Stack< T, C >::top() noexcept
  {
    return container_.front();
  }

  template < typename T, typename C >
  typename Stack< T, C >::const_reference Stack< T, C >::top() const noexcept
  {
    return static_cast< const C& >(container_).front();
  }

  template < typename T, typename C >
  void Stack< T, C >::pop() noexcept
  {
    container_.popFront();
  }

  template < typename T, typename C >
  void Stack< T, C >::push(const_reference value)
  {
    container_.pushFront(value);
  }

  template < typename T, typename C >
  void Stack< T, C >::push(value_type&& value)
  {
    container_.pushFront(std::move(value));
  }

  template < typename T, typename C >
  template < typename... Args >
  void Stack< T, C >::emplace(Args&&... args)
  {
    container_.emplaceFront(std::forward< Args >(args)...);
  }

  template < typename T, typename C >
  void Stack< T, C >::swap(Stack& rhs) noexcept(is_nothrow_swappable_v< C >)
  {
    using std::swap;
    swap(container_, rhs.container_);
  }

  template < typename T, typename C >
  bool operator==(const Stack< T, C >& lhs, const Stack< T, C >& rhs)
  {
    return lhs.container() == rhs.container();
  }

  template < typename T, typename C >
  bool operator!=(const Stack< T, C >& lhs, const Stack< T, C >& rhs)
  {
    return !(lhs == rhs);
  }

  template < typename T, typename C >
  bool operator<(const Stack< T, C >& lhs, const Stack< T, C >& rhs)
  {
    return lhs.container() < rhs.container();
  }

  template < typename T, typename C >
  bool operator>(const Stack< T, C >& lhs, const Stack< T, C >& rhs)
  {
    return rhs < lhs;
  }

  template < typename T, typename C >
  bool operator<=(const Stack< T, C >& lhs, const Stack< T, C >& rhs)
  {
    return !(rhs < lhs);
  }

  template < typename T, typename C >
  bool operator>=(const Stack< T, C >& lhs, const Stack< T, C >& rhs)
  {
    return !(lhs < rhs);
  }

  template < typename T, typename C >
  void swap(Stack< T, C >& lhs, Stack< T, C >& rhs) noexcept(noexcept(lhs.swap(rhs)))
  {
    lhs.swap(rhs);
  }
}

#endif

