#ifndef FWD_LIST_DECLARATION_HPP
#define FWD_LIST_DECLARATION_HPP

#include <cstddef>
#include <initializer_list>
#include "fwd_list_node.hpp"
#include "fwd_iterator.hpp"

namespace maslevtsov {
  template< class T >
  class FwdList
  {
  public:
    using iterator = FwdIterator< T, false >;
    using const_iterator = FwdIterator< T, true >;

    FwdList() noexcept;
    FwdList(const FwdList& rhs);
    FwdList(FwdList&& rhs) noexcept;
    FwdList(std::size_t count, const T& value);
    template< class InputIt >
    FwdList(InputIt first, InputIt last);
    FwdList(std::initializer_list< T > init);
    ~FwdList();

    FwdList& operator=(const FwdList& rhs);
    FwdList& operator=(FwdList&& rhs) noexcept;
    FwdList& operator=(std::initializer_list< T > ilist);

    void assign(std::size_t count, const T& value);
    template< class InputIt >
    void assign(InputIt first, InputIt last);
    void assign(std::initializer_list< T > ilist);

    T& front() noexcept;
    const T& front() const noexcept;
    T& back() noexcept;
    const T& back() const noexcept;

    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;

    bool empty() const noexcept;
    std::size_t size() const noexcept;

    void clear() noexcept;
    iterator insert_after(const_iterator pos, const T& value);
    iterator insert_after(const_iterator pos, T&& value);
    iterator insert_after(const_iterator pos, std::size_t count, const T& value);
    template< class InputIt >
    iterator insert_after(const_iterator pos, InputIt first, InputIt last);
    iterator insert_after(const_iterator pos, std::initializer_list< T > ilist);
    iterator erase_after(const_iterator pos) noexcept;
    iterator erase_after(const_iterator first, const_iterator last) noexcept;
    void push_front(const T& value);
    void push_front(T&& value);
    void push_back(const T& value);
    void push_back(T&& value);
    void pop_front() noexcept;
    void swap(FwdList& other) noexcept;

    void splice_after(const_iterator pos, FwdList& other);
    void splice_after(const_iterator pos, FwdList&& other) noexcept;
    void splice_after(const_iterator pos, FwdList& other, const_iterator it) noexcept;
    void splice_after(const_iterator pos, FwdList& other, const_iterator first, const_iterator last) noexcept;
    void remove(const T& value) noexcept;
    template< class UnaryPredicate >
    void remove_if(UnaryPredicate condition);

  private:
    FwdListNode< T >* tail_;
    std::size_t size_;

    template< class InputIt >
    void copy(InputIt first, InputIt last);
    void emplace_front(FwdListNode< T >* node) noexcept;
    void emplace_back(FwdListNode< T >* node) noexcept;
  };
}

#endif
