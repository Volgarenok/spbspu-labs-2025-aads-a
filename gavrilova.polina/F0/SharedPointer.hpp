
#ifndef SHARED_POINTER_HPP
#define SHARED_POINTER_HPP

#include <cstddef>
#include <utility>

namespace gavrilova {

  template < typename T >
  class SharedPtr {
  public:
    SharedPtr() noexcept;
    SharedPtr(T* ptr);
    SharedPtr(const SharedPtr& other) noexcept;
    SharedPtr(SharedPtr&& other) noexcept;
    template < class U >
    SharedPtr(const SharedPtr< U >& other) noexcept;
    ~SharedPtr();

    SharedPtr& operator=(const SharedPtr& other);
    SharedPtr& operator=(SharedPtr&& other) noexcept;

    T* get() const noexcept;
    T& operator*() const noexcept;
    T* operator->() const noexcept;
    operator bool() const noexcept;

  private:
    void release();

    T* ptr_;
    size_t* ref_count_;

    template < class U >
    friend class SharedPtr;
  };

  template < typename T, typename... Args >
  SharedPtr< T > make_shared(Args&&... args);

  template < typename T >
  SharedPtr< T >::SharedPtr() noexcept:
    ptr_(nullptr),
    ref_count_(nullptr)
  {}

  template < typename T >
  template < class U >
  SharedPtr< T >::SharedPtr(const SharedPtr< U >& other) noexcept:
    ptr_(other.get()),
    ref_count_(other.ref_count_)
  {
    if (ref_count_) {
      (*ref_count_)++;
    }
  }

  template < typename T >
  SharedPtr< T >::SharedPtr(T* ptr):
    ptr_(ptr),
    ref_count_(new size_t(1))
  {}

  template < typename T >
  SharedPtr< T >::SharedPtr(const SharedPtr& other) noexcept:
    ptr_(other.ptr_),
    ref_count_(other.ref_count_)
  {
    if (ref_count_) {
      (*ref_count_)++;
    }
  }

  template < typename T >
  SharedPtr< T >::SharedPtr(SharedPtr&& other) noexcept:
    ptr_(other.ptr_),
    ref_count_(other.ref_count_)
  {
    other.ptr_ = nullptr;
    other.ref_count_ = nullptr;
  }

  template < typename T >
  SharedPtr< T >::~SharedPtr()
  {
    release();
  }

  template < typename T >
  SharedPtr< T >& SharedPtr< T >::operator=(const SharedPtr& other)
  {
    if (this != &other) {
      release();
      ptr_ = other.ptr_;
      ref_count_ = other.ref_count_;
      if (ref_count_) {
        (*ref_count_)++;
      }
    }
    return *this;
  }

  template < typename T >
  SharedPtr< T >& SharedPtr< T >::operator=(SharedPtr&& other) noexcept
  {
    if (this != &other) {
      release();
      ptr_ = other.ptr_;
      ref_count_ = other.ref_count_;
      other.ptr_ = nullptr;
      other.ref_count_ = nullptr;
    }
    return *this;
  }

  template < typename T >
  T* SharedPtr< T >::get() const noexcept
  {
    return ptr_;
  }

  template < typename T >
  T& SharedPtr< T >::operator*() const noexcept
  {
    return *ptr_;
  }

  template < typename T >
  T* SharedPtr< T >::operator->() const noexcept
  {
    return ptr_;
  }

  template < typename T >
  SharedPtr< T >::operator bool() const noexcept
  {
    return ptr_ != nullptr;
  }

  template < typename T >
  void SharedPtr< T >::release()
  {
    if (ref_count_ && --(*ref_count_) == 0) {
      delete ptr_;
      delete ref_count_;
    }
    ptr_ = nullptr;
    ref_count_ = nullptr;
  }

  template < typename T, typename... Args >
  SharedPtr< T > make_shared(Args&&... args)
  {
    return SharedPtr< T >(new T(std::forward< Args >(args)...));
  }
}

#endif
