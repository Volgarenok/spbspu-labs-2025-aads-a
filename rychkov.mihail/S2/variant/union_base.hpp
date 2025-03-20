#ifndef UNION_BASE_HPP
#define UNION_BASE_HPP

#include <algorithm>
#include "tools.hpp"
#include "traits.hpp"

namespace rychkov
{
  namespace details
  {
    template< class... Types >
    struct UnionStorage
    {
      static constexpr size_t size = std::max({sizeof(Types)...});
      static constexpr size_t alignment = std::max({alignof(Types)...});
      using size_type = select_size_type_t< size, byte, unsigned short, size_t >;
      alignas(alignment) byte storage[size];
      size_type active = static_cast< size_type >(variant_npos);

      bool valueless() const noexcept
      {
        return active == static_cast< size_type >(variant_npos);
      }
      void setValueless() noexcept
      {
        active = static_cast< size_type >(variant_npos);
      }
      template< class T >
      remove_cvref_t< T >* get() noexcept
      {
        return reinterpret_cast< remove_cvref_t< T >* >(storage);
      }
      template< class T >
      const remove_cvref_t< T >* get() const noexcept
      {
        return reinterpret_cast< const remove_cvref_t< T >* >(storage);
      }
    };

    template< bool is_trivially_destructible, class... Types >
    struct DestructorBase: UnionStorage< Types... >
    {
      ~DestructorBase() = default;
      void destroy() noexcept
      {
        this->setValueless();
      }
    };
    template< class... Types >
    struct DestructorBase< false, Types... >: UnionStorage< Types... >
    {
      ~DestructorBase()
      {
        destroy();
      }
      void destroy() noexcept
      {
        using destructor_t = void(*)(byte*) noexcept;
        static constexpr destructor_t destructors[sizeof...(Types)] = {destructor< Types >...};
        destructors[this->active](this->storage);
        this->setValueless();
      }
    };
    template< class... Types >
    using DestructorBaseAlias = DestructorBase< variant_traits< Types... >::is_trivially_destructible, Types... >;

    template< EnableMethod, class... Types >
    struct CopyCtorBase: DestructorBaseAlias< Types... >
    {};
    template< class... Types >
    struct CopyCtorBase< DELETED, Types... >: DestructorBaseAlias< Types... >
    {
      CopyCtorBase() = default;
      CopyCtorBase(const CopyCtorBase& rhs) = delete;
      CopyCtorBase(CopyCtorBase&& rhs) = default;
      CopyCtorBase& operator=(const CopyCtorBase& rhs) = default;
      CopyCtorBase& operator=(CopyCtorBase&& rhs) = default;
    };
    template< class... Types >
    struct CopyCtorBase< USER_DEFINED, Types... >: DestructorBaseAlias< Types... >
    {
      static constexpr bool is_nothrow_copy_ctor = variant_traits< Types... >::is_nothrow_copy_ctor;

      CopyCtorBase() = default;
      CopyCtorBase(const CopyCtorBase& rhs) noexcept(is_nothrow_copy_ctor)
      {
        copy_construct(rhs);
      }
      CopyCtorBase(CopyCtorBase&& rhs) = default;
      CopyCtorBase& operator=(const CopyCtorBase& rhs) = default;
      CopyCtorBase& operator=(CopyCtorBase&& rhs) = default;

      void copy_construct(const CopyCtorBase& rhs) noexcept(is_nothrow_copy_ctor)
      {
        using copy_ctor_t = void(*)(byte*, const byte*) noexcept(is_nothrow_copy_ctor);
        static constexpr copy_ctor_t copy_ctors[sizeof...(Types)] = {copy_ctor< is_nothrow_copy_ctor, Types >...};
        if (!rhs.valueless())
        {
          copy_ctors[rhs.active](this->storage, rhs.storage);
          this->active = rhs.active;
        }
      }
    };
    template< class... Types >
    using CopyCtorBaseAlias = CopyCtorBase< enable(variant_traits< Types... >::is_triv_copy_ctor,
            variant_traits< Types... >::is_copy_ctor), Types... >;

    template< EnableMethod, class... Types >
    struct MoveCtorBase: CopyCtorBaseAlias< Types... >
    {};
    template< class... Types >
    struct MoveCtorBase< DELETED, Types... >: CopyCtorBaseAlias< Types... >
    {
      MoveCtorBase() = default;
      MoveCtorBase(const MoveCtorBase& rhs) = default;
      MoveCtorBase(MoveCtorBase&& rhs) = delete;
      MoveCtorBase& operator=(const MoveCtorBase& rhs) = default;
      MoveCtorBase& operator=(MoveCtorBase&& rhs) = default;
    };
    template< class... Types >
    struct MoveCtorBase< USER_DEFINED, Types... >: CopyCtorBaseAlias< Types... >
    {
      static constexpr bool is_nothrow_move_ctor = variant_traits< Types... >::is_nothrow_move_ctor;

      MoveCtorBase() = default;
      MoveCtorBase(const MoveCtorBase& rhs) = default;
      MoveCtorBase(MoveCtorBase&& rhs) noexcept(is_nothrow_move_ctor)
      {
        move_construct(std::move(rhs));
      }
      MoveCtorBase& operator=(const MoveCtorBase& rhs) = default;
      MoveCtorBase& operator=(MoveCtorBase&& rhs) = default;

      void move_construct(MoveCtorBase&& rhs) noexcept(is_nothrow_move_ctor)
      {
        using move_ctor_t = void(*)(byte*, byte*) noexcept(is_nothrow_move_ctor);
        static constexpr move_ctor_t move_ctors[sizeof...(Types)] = {move_ctor< is_nothrow_move_ctor, Types >...};
        if (!rhs.valueless())
        {
          move_ctors[rhs.active](this->storage, rhs.storage);
          this->active = rhs.active;
        }
      }
    };
    template< class... Types >
    using MoveCtorBaseAlias = MoveCtorBase< enable(variant_traits< Types... >::is_triv_move_ctor,
            variant_traits< Types... >::is_move_ctor), Types... >;

    template< EnableMethod, class... Types >
    struct CopyAssignBase: MoveCtorBaseAlias< Types... >
    {};
    template< class... Types >
    struct CopyAssignBase< DELETED, Types... >: MoveCtorBaseAlias< Types... >
    {
      CopyAssignBase() = default;
      CopyAssignBase(const CopyAssignBase& rhs) = default;
      CopyAssignBase(CopyAssignBase&& rhs) = default;
      CopyAssignBase& operator=(const CopyAssignBase& rhs) = delete;
      CopyAssignBase& operator=(CopyAssignBase&& rhs) = default;
    };
    template< class... Types >
    struct CopyAssignBase< USER_DEFINED, Types... >: MoveCtorBaseAlias< Types... >
    {
      static constexpr bool is_nothrow_copy_assign = variant_traits< Types... >::is_nothrow_copy_assign;

      CopyAssignBase() = default;
      CopyAssignBase(const CopyAssignBase& rhs) = default;
      CopyAssignBase(CopyAssignBase&& rhs) = default;
      CopyAssignBase& operator=(const CopyAssignBase& rhs) noexcept(is_nothrow_copy_assign)
      {
        using copy_assign_t = void(*)(byte*, const byte*) noexcept(is_nothrow_copy_assign);
        static constexpr copy_assign_t copy_assigns[sizeof...(Types)] = {copy_assign< is_nothrow_copy_assign, Types >...};
        if (!this->valueless() && (this->active != rhs.active))
        {
          this->destroy();
        }
        if (!rhs.valueless())
        {
          if (this->active != rhs.active)
          {
            this->copy_construct(rhs);
          }
          else
          {
            copy_assigns[this->active](this->storage, rhs.storage);
          }
        }
        return *this;
      }
      CopyAssignBase& operator=(CopyAssignBase&& rhs) = default;
    };
    template< class... Types >
    using CopyAssignBaseAlias = CopyAssignBase< enable(variant_traits< Types... >::is_triv_copy_assign,
            variant_traits< Types... >::is_copy_assign), Types... >;

    template< EnableMethod, class... Types >
    struct MoveAssignBase: CopyAssignBaseAlias< Types... >
    {};
    template< class... Types >
    struct MoveAssignBase< DELETED, Types... >: CopyAssignBaseAlias< Types... >
    {
      MoveAssignBase() = default;
      MoveAssignBase(const MoveAssignBase& rhs) = default;
      MoveAssignBase(MoveAssignBase&& rhs) = default;
      MoveAssignBase& operator=(const MoveAssignBase& rhs) = default;
      MoveAssignBase& operator=(MoveAssignBase&& rhs) = delete;
    };
    template< class... Types >
    struct MoveAssignBase< USER_DEFINED, Types... >: CopyAssignBaseAlias< Types... >
    {
      static constexpr bool is_nothrow_move_assign = variant_traits< Types... >::is_nothrow_move_assign;

      MoveAssignBase() = default;
      MoveAssignBase(const MoveAssignBase& rhs) = default;
      MoveAssignBase(MoveAssignBase&& rhs) = default;
      MoveAssignBase& operator=(const MoveAssignBase& rhs) = default;
      MoveAssignBase& operator=(MoveAssignBase&& rhs) noexcept(is_nothrow_move_assign)
      {
        using move_assign_t = void(*)(byte*, byte*) noexcept(is_nothrow_move_assign);
        static constexpr move_assign_t move_assigns[sizeof...(Types)] = {move_assign< is_nothrow_move_assign, Types >...};
        if (!this->valueless() && (this->active != rhs.active))
        {
          this->destroy();
        }
        if (!rhs.valueless())
        {
          if (this->active != rhs.active)
          {
            this->move_construct(std::move(rhs));
          }
          else
          {
            move_assigns[this->active](this->storage, rhs.storage);
          }
        }
        return *this;
      }
    };
    template< class... Types >
    using MoveAssignBaseAlias = MoveAssignBase< enable(variant_traits< Types... >::is_triv_move_assign,
            variant_traits< Types... >::is_move_assign), Types... >;

    template< bool is_default_constructible, class... Types >
    struct VariantBase: MoveAssignBaseAlias< Types... >
    {
      VariantBase() = delete;
      VariantBase(int)
      {}
    };
    template< class... Types >
    struct VariantBase< true, Types... >: MoveAssignBaseAlias< Types... >
    {
      static constexpr bool is_nothrow_def_ctor = variant_traits< Types... >::is_nothrow_def_ctor;

      VariantBase() noexcept(is_nothrow_def_ctor)
      {
        new (this->storage) variant_alternative_t< 0, Types... >();
        this->active = 0;
      }
      VariantBase(int)
      {}
    };
    template< class... Types >
    using VariantBaseAlias = VariantBase< variant_traits< Types... >::is_def_ctor, Types... >;
  }
}

#endif
