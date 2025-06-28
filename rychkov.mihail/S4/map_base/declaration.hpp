#ifndef MAP_BASE_DECLARATION_HPP
#define MAP_BASE_DECLARATION_HPP

#include <utility>
#include <type_traits>
#include <memory>
#include "node.hpp"
#include "iterator.hpp"

namespace rychkov
{
  template< class Member, class Base >
  size_t offset_of(const Member Base::* member)
  {
    return reinterpret_cast< const char* >(std::addressof(static_cast< Base* >(nullptr)->*member))
        - static_cast< const char* >(nullptr);
  }
  template< class Member, class Base >
  Base* get_fake_pointer(const Member& fake_member, const Member Base::* member)
  {
    static size_t offset = offset_of(member);
    auto clear_p = const_cast< Member* >(std::addressof(fake_member));
    return reinterpret_cast< Base* >(reinterpret_cast< char* >(clear_p) - offset);
  }
  template< class Cmp, class = void >
  struct is_transparent: std::false_type
  {};
  template< class Cmp >
  struct is_transparent< Cmp, void_t< typename Cmp::is_transparent > >: std::true_type
  {};
  template< class Cmp >
  constexpr bool is_transparent_v = is_transparent< Cmp >::value;
  namespace details
  {
    template< class R, class K1, class C, class... Exclude >
    using transparent_compare_key_t = std::enable_if_t< conjunction_v< !std::is_same< remove_cvref_t< K1 >,
              Exclude >::value... > && is_transparent_v< C >, R >;
  }

  template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
  class MapBase
  {
  public:
    using key_type = K;
    using mapped_type = T;
    using key_compare = C;
  private:
    using real_value_type = std::conditional_t< IsSet, key_type, std::pair< key_type, mapped_type > >;
  public:
    using value_type = std::conditional_t< IsSet, key_type, std::pair< const key_type, mapped_type > >;
    using size_type = size_t;
    using difference_type = ptrdiff_t;

    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;

    using iterator = MapBaseIterator< value_type, N, real_value_type, false, false >;
    using const_iterator = MapBaseIterator< value_type, N, real_value_type, true, false >;
    using reverse_iterator = MapBaseIterator< value_type, N, real_value_type, false, true >;
    using const_reverse_iterator = MapBaseIterator< value_type, N, real_value_type, true, true >;

    struct map_value_compare
    {
      using value_type = std::pair< const key_type, mapped_type >;

      key_compare comp;
      template< bool IsSet2 = IsSet >
      bool operator()(const std::enable_if_t< IsSet2 && IsSet, value_type >& lhs, const value_type& rhs) const
      {
        return comp(lhs.first, rhs.first);
      }
    };

    using value_compare = std::conditional_t< IsSet, key_compare, map_value_compare >;

    MapBase() noexcept(std::is_nothrow_default_constructible< value_compare >::value);
    MapBase(value_compare compare) noexcept(std::is_nothrow_move_constructible< value_compare >::value);
    MapBase(const MapBase& rhs);
    MapBase(MapBase&& rhs) noexcept(std::is_nothrow_move_constructible< value_compare >::value);
    MapBase(std::initializer_list< value_type > init, value_compare compare = {});
    template< class InputIt >
    MapBase(InputIt from, InputIt to, value_compare compare = {});
    ~MapBase();
    MapBase& operator=(const MapBase& rhs);
    MapBase& operator=(MapBase&& rhs) noexcept(noexcept(swap(std::declval< MapBase& >())));
    MapBase& operator=(std::initializer_list< value_type > init);

    bool empty() const noexcept;
    size_type size() const noexcept;
    size_type maxsize() const noexcept;

    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;

    reverse_iterator rbegin() noexcept;
    const_reverse_iterator rbegin() const noexcept;
    const_reverse_iterator crbegin() const noexcept;
    reverse_iterator rend() noexcept;
    const_reverse_iterator rend() const noexcept;
    const_reverse_iterator crend() const noexcept;

    iterator lower_bound(const key_type& key);
    const_iterator lower_bound(const key_type& key) const;
    iterator upper_bound(const key_type& key);
    const_iterator upper_bound(const key_type& key) const;
    iterator find(const key_type& key);
    const_iterator find(const key_type& key) const;
    bool contains(const key_type& key) const;
    std::pair< iterator, iterator > equal_range(const key_type& key);
    std::pair< const_iterator, const_iterator > equal_range(const key_type& key) const;
    size_type count(const key_type& key) const;

  private:
    template< class R, class K1 >
    using transparent_compare_key_t = details::transparent_compare_key_t< R, K1, C, iterator, const_iterator >;

  public:
    template< class K1 >
    transparent_compare_key_t< iterator, K1 > lower_bound(const K1& key);
    template< class K1 >
    transparent_compare_key_t< const_iterator, K1 > lower_bound(const K1& key) const;
    template< class K1 >
    transparent_compare_key_t< iterator, K1 > upper_bound(const K1& key);
    template< class K1 >
    transparent_compare_key_t< const_iterator, K1 > upper_bound(const K1& key) const;
    template< class K1 >
    transparent_compare_key_t< iterator, K1 > find(const K1& key);
    template< class K1 >
    transparent_compare_key_t< const_iterator, K1 > find(const K1& key) const;
    template< class K1 >
    transparent_compare_key_t< bool, K1 > contains(const K1& key) const;
    template< class K1 >
    transparent_compare_key_t< std::pair< iterator, iterator >, K1 > equal_range(const K1& key);
    template< class K1 >
    transparent_compare_key_t< std::pair< const_iterator, const_iterator >, K1 > equal_range(const K1& key) const;
    template< class K1 >
    transparent_compare_key_t< size_type, K1 > count(const K1& key) const;

    template< bool IsSet2 = IsSet, bool IsMulti2 = IsMulti >
    std::enable_if_t< !IsSet && !IsSet2 && !IsMulti && !IsMulti2, mapped_type& > at(const key_type& key);
    template< bool IsSet2 = IsSet, bool IsMulti2 = IsMulti >
    std::enable_if_t< !IsSet && !IsSet2 && !IsMulti && !IsMulti2, const mapped_type& > at(const key_type& key) const;
    template< class K1, bool IsSet2 = IsSet, bool IsMulti2 = IsMulti >
    transparent_compare_key_t< std::enable_if_t< !IsSet && !IsSet2 && !IsMulti && !IsMulti2, const mapped_type& >, K1 >
        at(const K1& key) const;
    template< class K1, bool IsSet2 = IsSet, bool IsMulti2 = IsMulti >
    transparent_compare_key_t< std::enable_if_t< !IsSet && !IsSet2 && !IsMulti && !IsMulti2, mapped_type& >, K1 >
        at(const K1& key);
    template< bool IsSet2 = IsSet, bool IsMulti2 = IsMulti >
    std::enable_if_t< !IsSet && !IsSet2 && !IsMulti && !IsMulti2, mapped_type& > operator[](const key_type& key);
    template< bool IsSet2 = IsSet, bool IsMulti2 = IsMulti >
    std::enable_if_t< !IsSet && !IsSet2 && !IsMulti && !IsMulti2, mapped_type& > operator[](key_type&& key);
    template< class K1, bool IsSet2 = IsSet, bool IsMulti2 = IsMulti >
    transparent_compare_key_t< std::enable_if_t< !IsSet && !IsSet2 && !IsMulti && !IsMulti2, mapped_type& >, K1 >
        operator[](K1&& key);

    void clear() noexcept;
    void swap(MapBase& rhs) noexcept(is_nothrow_swappable_v< value_compare >);

    iterator erase(const_iterator pos);
    iterator erase(const_iterator from, const_iterator to);
    size_type erase(const key_type& key);
    template< class K1 >
    transparent_compare_key_t< size_type, K1 > erase(const K1& key);

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

    template< bool IsSet2 = IsSet >
    std::enable_if_t< IsSet2, key_compare > key_comp() const;
    template< bool IsSet2 = IsSet >
    std::enable_if_t< !IsSet2, key_compare > key_comp() const;
    value_compare value_comp() const;

    template< bool IsSet2 = IsSet, class... Args >
    std::enable_if_t< !IsSet && !IsSet2, std::pair< iterator, bool > > try_emplace
        (const key_type& key, Args&&... args);
    template< bool IsSet2 = IsSet, class... Args >
    std::enable_if_t< !IsSet && !IsSet2, std::pair< iterator, bool > > try_emplace(key_type&& key, Args&&... args);
    template< bool IsSet2 = IsSet, class K1, class... Args >
    transparent_compare_key_t< std::enable_if_t< !IsSet && !IsSet2, std::pair< iterator, bool > >, K1 >
        try_emplace(K1&& key, Args&&... args);
    template< bool IsSet2 = IsSet, class... Args >
    std::enable_if_t< !IsSet && !IsSet2, iterator > try_emplace(const_iterator hint, const key_type& key, Args&&... args);
    template< bool IsSet2 = IsSet, class... Args >
    std::enable_if_t< !IsSet && !IsSet2, iterator > try_emplace(const_iterator hint, key_type&& key, Args&&... args);
    template< bool IsSet2 = IsSet, class K1, class... Args >
    transparent_compare_key_t< std::enable_if_t< !IsSet && !IsSet2, std::pair< iterator, bool > >, K1 >
        try_emplace(const_iterator hint, K1&& key, Args&&... args);

  private:
    static constexpr size_t node_capacity = N;
    static constexpr size_t node_middle = (node_capacity + 1) / 2;
    using node_type = MapBaseNode< real_value_type, node_capacity >;
    using node_size_type = typename node_type::size_type;

    value_compare comp_;
    node_type* cached_begin_;
    node_type* cached_rbegin_;
    size_t size_;

    node_type*const fake_parent_ = nullptr;
    node_type* fake_children_[node_capacity + 1];
    const node_size_type fake_size_;

    node_type* fake_root() const noexcept;
    void devide(node_type& left, node_type& right, node_size_type ins_point, node_type& to_insert);
    static void correct_emplace_result(node_type& left, node_type& right,
        node_size_type ins_point, const_iterator& hint);
    static void correct_erase_result(const_iterator to, const_iterator from, iterator& result, bool will_be_replaced);

    template< class K1 >
    std::pair< const_iterator, const_iterator > lower_bound_impl(const K1& key) const;
    template< class K1 >
    const_iterator upper_bound_impl(const K1& key) const;
    template< class K1 >
    std::pair< const_iterator, bool > find_hint_pair(const K1& key) const;
    template< class K1 >
    std::pair< const_iterator, bool > correct_hint(const_iterator hint, const K1& key) const;
    template< class K1 >
    std::pair< const_iterator, bool > correct_hint(const_reverse_iterator hint, const K1& key) const;

    template< class... Args >
    std::pair< iterator, bool > emplace_hint_impl(std::pair< const_iterator, bool > hint, Args&&... args);
    template< bool IsSet2 = IsSet, class K1, class... Args >
    std::enable_if_t< IsSet && IsSet2, std::pair< iterator, bool > > emplace_impl
        (const_iterator hint, K1&& key, Args&&... args);

    template< bool IsSet2 = IsSet >
    bool compare_with_key (std::enable_if_t< !IsSet && !IsSet2, const value_type >& lhs,
          const key_type& rhs) const;
    template< bool IsSet2 = IsSet >
    bool compare_with_key(const key_type& lhs,
          std::enable_if_t< !IsSet && !IsSet2, const value_type >& rhs) const;
    template< bool IsSet2 = IsSet >
    std::enable_if_t< IsSet && IsSet2, bool > compare_with_key(const key_type& lhs, const key_type& rhs) const;
    template< bool IsSet2 = IsSet >
    std::enable_if_t< !IsSet && !IsSet2, bool > compare_with_key(const key_type& lhs, const key_type& rhs) const;

    template< class V = value_type >
    static const key_type& get_key(const V& value);
    static const key_type& get_key(const key_type& key);
  };
}

#endif
