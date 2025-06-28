#ifndef UNORDERED_BASE_DECLARATION_HPP
#define UNORDERED_BASE_DECLARATION_HPP

#include <utility>

#include <boost/hash2/fnv1a.hpp>
#include <boost/hash2/hash_append.hpp>
#include <boost/hash2/get_integral_result.hpp>

#include <type_traits.hpp>
#include "iterator.hpp"

namespace rychkov
{
  template< class T = void >
  struct Hash
  {
    size_t operator()(const T& value) const
    {
      boost::hash2::fnv1a_64 hasher;
      boost::hash2::hash_append(hasher, {}, value);
      return boost::hash2::get_integral_result< size_t >(hasher);
    }
  };
  template<>
  struct Hash< void >
  {
    using is_transparent = void;
    template< class T >
    size_t operator()(const T& value) const
    {
      boost::hash2::fnv1a_64 hasher;
      boost::hash2::hash_append(hasher, {}, value);
      return boost::hash2::get_integral_result< size_t >(hasher);
    }
  };
  namespace details
  {
    template< class R, class K1, class H, class E, class... Exclude >
    using transparent_hash_key_t = std::enable_if_t< conjunction_v< !std::is_same< remove_cvref_t< K1 >,
              Exclude >::value... > && is_transparent_v< H > && is_transparent_v< E >, R >;
  }
  template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
  class UnorderedBase
  {
  public:
    using key_type = K;
    using mapped_type = T;
    using hasher = H;
    using key_equal = E;

    using value_type = std::conditional_t< IsSet, key_type, std::pair< const key_type, mapped_type > >;
    using size_type = size_t;
    using difference_type = ptrdiff_t;

    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;

    using iterator = UnorderedBaseIterator< value_type, false >;
    using const_iterator = UnorderedBaseIterator< value_type, true >;

  private:
    static constexpr bool noexcept_default = std::is_nothrow_default_constructible< hasher >::value
        && std::is_nothrow_default_constructible< key_equal >::value;
    static constexpr bool noexcept_move = std::is_nothrow_move_constructible< hasher >::value
        && std::is_nothrow_move_constructible< key_equal >::value;

  public:
    UnorderedBase() noexcept(noexcept_default);
    UnorderedBase(const UnorderedBase& rhs);
    UnorderedBase(UnorderedBase&& rhs) noexcept(noexcept_move);
    explicit UnorderedBase(size_type cnt, hasher hash = {}, key_equal eq = {});
    UnorderedBase(std::initializer_list< value_type > init, hasher hash = {}, key_equal eq = {});
    template< class InputIt >
    UnorderedBase(InputIt from, InputIt to, hasher hash = {}, key_equal eq = {});
    ~UnorderedBase();
    UnorderedBase& operator=(const UnorderedBase& rhs);
    UnorderedBase& operator=(UnorderedBase&& rhs) noexcept(noexcept(swap(std::declval< UnorderedBase& >())));
    UnorderedBase& operator=(std::initializer_list< value_type > init);

    bool empty() const noexcept;
    size_type size() const noexcept;
    size_type maxsize() const noexcept;
    float load_factor() const noexcept;
    float max_load_factor() const noexcept;
    void max_load_factor(float new_factor) noexcept;

    size_type bucket_count() const noexcept;
    size_type max_bucket_count() const noexcept;
    size_type bucket_size() const noexcept;
    size_type bucket(size_type index) const noexcept;

    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;

    iterator find(const key_type& key);
    const_iterator find(const key_type& key) const;
    bool contains(const key_type& key) const;
    size_type count(const key_type& key) const;

  private:
    template< class R, class K1 >
    using transparent_hash_key_t = details::transparent_hash_key_t< R, K1, H, E, iterator, const_iterator >;

  public:
    template< class K1 >
    transparent_hash_key_t< iterator, K1 > find(const K1& key);
    template< class K1 >
    transparent_hash_key_t< const_iterator, K1 > find(const K1& key) const;
    template< class K1 >
    transparent_hash_key_t< bool, K1 > contains(const K1& key) const;
    template< class K1 >
    transparent_hash_key_t< size_type, K1 > count(const K1& key) const;

    template< bool IsSet2 = IsSet, bool IsMulti2 = IsMulti >
    std::enable_if_t< !IsSet && !IsSet2 && !IsMulti && !IsMulti2, mapped_type& > at(const key_type& key);
    template< bool IsSet2 = IsSet, bool IsMulti2 = IsMulti >
    std::enable_if_t< !IsSet && !IsSet2 && !IsMulti && !IsMulti2, const mapped_type& > at(const key_type& key) const;
    template< class K1, bool IsSet2 = IsSet, bool IsMulti2 = IsMulti >
    transparent_hash_key_t< std::enable_if_t< !IsSet && !IsSet2 && !IsMulti && !IsMulti2, const mapped_type& >, K1 >
        at(const K1& key) const;
    template< class K1, bool IsSet2 = IsSet, bool IsMulti2 = IsMulti >
    transparent_hash_key_t< std::enable_if_t< !IsSet && !IsSet2 && !IsMulti && !IsMulti2, mapped_type& >, K1 >
        at(const K1& key);

    template< bool IsSet2 = IsSet, bool IsMulti2 = IsMulti >
    std::enable_if_t< !IsSet && !IsSet2 && !IsMulti && !IsMulti2, mapped_type& > operator[](const key_type& key);
    template< bool IsSet2 = IsSet, bool IsMulti2 = IsMulti >
    std::enable_if_t< !IsSet && !IsSet2 && !IsMulti && !IsMulti2, mapped_type& > operator[](key_type&& key);
    template< class K1, bool IsSet2 = IsSet, bool IsMulti2 = IsMulti >
    transparent_hash_key_t< std::enable_if_t< !IsSet && !IsSet2 && !IsMulti && !IsMulti2, mapped_type& >, K1 >
        operator[](K1&& key);

    void clear() noexcept;
    void swap(UnorderedBase& rhs) noexcept(is_nothrow_swappable_v< hasher > && is_nothrow_swappable_v< key_equal >);

    void reserve(size_type new_capacity);
    void rehash(size_type cnt);

    iterator erase(const_iterator pos);
    iterator erase(const_iterator from, const_iterator to);
    size_type erase(const key_type& key);
    template< class K1 >
    transparent_hash_key_t< size_type, K1 > erase(const K1& key);

    template< class... Args >
    std::pair< iterator, bool > emplace(Args&&... args);
    template< class... Args >
    iterator emplace_hint(const_iterator hint, Args&&... args);

    std::pair< iterator, bool > insert(const value_type& value);
    std::pair< iterator, bool > insert(value_type&& value);
    template< class V >
    std::enable_if_t< std::is_constructible< value_type, V&& >::value, std::pair< iterator, bool > >
        insert(V&& value);
    iterator insert(const_iterator hint, const value_type& value);
    iterator insert(const_iterator hint, value_type&& value);
    template< class V >
    std::enable_if_t< std::is_constructible< value_type, V&& >::value, iterator >
        insert(const_iterator hint, V&& value);
    template< class InputIter >
    void insert(InputIter from, InputIter to);
    void insert(std::initializer_list< value_type > list);

    hasher hash_function() const;
    key_equal key_eq() const;

    template< bool IsSet2 = IsSet, class... Args >
    std::enable_if_t< !IsSet && !IsSet2, std::pair< iterator, bool > > try_emplace
        (const key_type& key, Args&&... args);
    template< bool IsSet2 = IsSet, class... Args >
    std::enable_if_t< !IsSet && !IsSet2, std::pair< iterator, bool > > try_emplace(key_type&& key, Args&&... args);
    template< bool IsSet2 = IsSet, class K1, class... Args >
    transparent_hash_key_t< std::enable_if_t< !IsSet && !IsSet2, std::pair< iterator, bool > >, K1 >
        try_emplace(K1&& key, Args&&... args);
    template< bool IsSet2 = IsSet, class... Args >
    std::enable_if_t< !IsSet && !IsSet2, iterator > try_emplace(const_iterator hint, const key_type& key, Args&&... args);
    template< bool IsSet2 = IsSet, class... Args >
    std::enable_if_t< !IsSet && !IsSet2, iterator > try_emplace(const_iterator hint, key_type&& key, Args&&... args);
    template< bool IsSet2 = IsSet, class K1, class... Args >
    transparent_hash_key_t< std::enable_if_t< !IsSet && !IsSet2, std::pair< iterator, bool > >, K1 >
        try_emplace(const_iterator hint, K1&& key, Args&&... args);

  private:
    using stored_value = std::pair< size_type, value_type >;
    using temp_value = std::conditional_t< IsSet, key_type, std::pair< key_type, mapped_type > >;
    using temp_stored = std::pair< size_type, temp_value >;
    static constexpr float default_max_factor = 0.5;

    size_type capacity_, size_;
    float max_factor_;
    stored_value* data_;
    unsigned char* raw_;
    stored_value* cached_begin_;
    hasher hash_;
    key_equal equal_;

    void allocate(size_type new_capacity);
    bool extend(size_type new_capacity);

    template< class K1 >
    size_type count_impl(const K1& key) const;
    template< class K1 >
    const_iterator find_impl(const K1& key) const;

    template< class K1 >
    std::pair< const_iterator, bool > find_hint_pair(const K1& key) const;
    template< class K1 >
    std::pair< const_iterator, bool > correct_hint(const_iterator hint, const K1& key);
    template< class... Args >
    std::pair< iterator, bool > emplace_hint_impl(std::pair< const_iterator, bool > hint, Args&&... args);

    template< class V = value_type >
    static const key_type& get_key(const V& value);
    static const key_type& get_key(const key_type& key);
  };
}

#endif
