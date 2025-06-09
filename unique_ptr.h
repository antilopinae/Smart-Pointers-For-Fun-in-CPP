#pragma once

#include <cstddef>
#include <iostream>

#include "smrtptrs.h"

namespace smrtptrs
{

template <typename T, typename D = default_delete<T>>
class unique_ptr
{
public:
  using element_type = typename std::conditional<std::is_array<T>::value, typename std::remove_extent<T>::type, T>::type;
  using pointer_type = typename std::conditional<std::is_array<T>::value, element_type*, T*>::type;
  using deleter_type = D;

private:
  pointer_type ptr_;
  deleter_type deleter_;

public:
  unique_ptr(pointer_type ptr = nullptr, deleter_type d = deleter_type()) noexcept : ptr_(ptr), deleter_(d) {}

  template <typename U>
  unique_ptr(U* p, deleter_type d) = delete;

  unique_ptr(unique_ptr&& u) noexcept;

  // destructor
  ~unique_ptr();

  // assignment
  unique_ptr& operator=(unique_ptr&& u) noexcept;
  template <typename U, typename E>
  unique_ptr& operator=(unique_ptr<U, E>&& u) noexcept;

  element_type& operator[](size_t i) const;
  pointer_type get() const noexcept;
  deleter_type& get_deleter() noexcept;
  const deleter_type& get_deleter() const noexcept;
  explicit operator bool() const noexcept
  {
    return ptr_ != nullptr;
  }

  element_type operator*() const
  {
    return *ptr_;
  }
  pointer_type operator->() const
  {
    return ptr_;
  }

  pointer_type release() noexcept;
  void reset(pointer_type p = pointer_type()) noexcept;

  template <typename U>
  void reset(U*) = delete;
  void swap(unique_ptr& u) noexcept;

  void swap(unique_ptr& first, unique_ptr& second) noexcept;

  // disable copy from lvalue
  unique_ptr(const unique_ptr&) = delete;
  unique_ptr& operator=(const unique_ptr&) = delete;
};

template <typename U, typename E>
unique_ptr<U, E>::unique_ptr(unique_ptr<U, E>&& u) noexcept : ptr_(u.ptr_),
                                                              deleter_(u.deleter_)
{
  u.ptr_ = nullptr;
};

// ********* destructor *********
template <typename U, typename E>
unique_ptr<U, E>::~unique_ptr()
{
  deleter_(ptr_);
}

// ********* assignment *********
template <typename U, typename E>
unique_ptr<U, E>& unique_ptr<U, E>::operator=(unique_ptr<U, E>&& u) noexcept
{
  delete this->ptr_;
  this->ptr_ = nullptr;
  swap(*this, u);
  return *this;
}

template <typename U, typename E>
typename unique_ptr<U, E>::element_type& unique_ptr<U, E>::operator[](size_t i) const
{
  return ptr_[i];
}

template <typename U, typename E>
typename unique_ptr<U, E>::pointer_type unique_ptr<U, E>::get() const noexcept
{
  return ptr_;
}

template <typename U, typename E>
typename unique_ptr<U, E>::deleter_type& unique_ptr<U, E>::get_deleter() noexcept
{
  return deleter_;
}

template <typename U, typename E>
const typename unique_ptr<U, E>::deleter_type& unique_ptr<U, E>::get_deleter() const noexcept
{
  return deleter_;
}

template <typename U, typename E>
typename unique_ptr<U, E>::pointer_type unique_ptr<U, E>::release() noexcept
{
  typename unique_ptr<U, E>::pointer_type ptr = ptr_;
  ptr_ = nullptr;
  return ptr;
}

template <typename U, typename E>
void unique_ptr<U, E>::reset(typename unique_ptr<U, E>::pointer_type p) noexcept
{
  deleter_(ptr_);
  ptr(p);
}

template <typename U, typename E>
void unique_ptr<U, E>::swap(unique_ptr<U, E>& u) noexcept
{
  swap(*this, u);
}

template <typename U, typename E>
void unique_ptr<U, E>::swap(unique_ptr<U, E>& first, unique_ptr<U, E>& second) noexcept
{
  std::swap(first.ptr_, second.ptr_);
}

// ********* make_unique *********
template <typename U, typename... Args>
typename std::enable_if<!std::is_array<U>::value, unique_ptr<U>>::type make_unique(Args&&... args)
{
  return {new U(std::forward<Args>(args)...)};
}

template <typename U, typename... Args>
typename std::enable_if<std::is_array<U>::value, unique_ptr<U>>::type make_unique(size_t size, Args&&... args)
{
  using element_type = typename std::remove_extent<U>::type;
  return {new element_type[size]{args...}};
}

// ********* comparisons *********
template <class U, class E>
inline bool operator==(const unique_ptr<U>& l, const unique_ptr<E>& r) throw()
{
  return (l.get() == r.get());
}
template <class U, class E>
inline bool operator!=(const unique_ptr<U>& l, const unique_ptr<E>& r) throw()
{
  return (l.get() != r.get());
}
template <class U, class E>
inline bool operator<=(const unique_ptr<U>& l, const unique_ptr<E>& r) throw()
{
  return (l.get() <= r.get());
}
template <class U, class E>
inline bool operator<(const unique_ptr<U>& l, const unique_ptr<E>& r) throw()
{
  return (l.get() < r.get());
}
template <class U, class E>
inline bool operator>=(const unique_ptr<U>& l, const unique_ptr<E>& r) throw()
{
  return (l.get() >= r.get());
}
template <class U, class E>
inline bool operator>(const unique_ptr<U>& l, const unique_ptr<E>& r) throw()
{
  return (l.get() > r.get());
}

}  // namespace smrtptrs
