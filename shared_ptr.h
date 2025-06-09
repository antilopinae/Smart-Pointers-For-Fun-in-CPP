#pragma once

#include <iostream>
#include <type_traits>
#include <utility>

#include "smrtptrs.h"

namespace smrtptrs
{

template <typename T, typename D = default_delete<T>>
class weak_ptr;

template <typename T, typename D = default_delete<T>>
class shared_ptr
{
private:
  friend class weak_ptr<T, D>;

  template <typename U, typename W, typename... Args>
  friend shared_ptr<U, W> make_shared(Args&&... args);

  template <typename U, typename W, typename... Args>
  friend shared_ptr<U, W> make_shared(std::size_t size, Args&&... args);

  struct cntrl_block
  {
    size_t count;
    size_t weak_count;

    using element_type = typename std::conditional<std::is_array<T>::value, typename std::remove_extent<T>::type, T>::type;
    using pointer_type = typename std::conditional<std::is_array<T>::value, element_type*, T*>::type;

    pointer_type ptr;
    D deleter;

    cntrl_block(size_t cnt, size_t weak_cnt, pointer_type p, D d) : count(cnt), weak_count(weak_cnt), ptr(p), deleter(d) {}
  };

private:
  cntrl_block* block;

public:
  using element_type = typename std::conditional<std::is_array<T>::value, typename std::remove_extent<T>::type, T>::type;
  using pointer_type = typename std::conditional<std::is_array<T>::value, element_type*, T*>::type;
  using deleter_type = D;

private:
  explicit shared_ptr(const weak_ptr<T, D>& weak, bool) : block(weak.block)
  {
    increment();
  }

  void increment()
  {
    if (block)
    {
      ++block->count;
    }
  }

  void decrement()
  {
    if (block && --block->count == 0)
    {
      if (block->ptr)
      {
        block->deleter(block->ptr);
        block->ptr = nullptr;
      }
      if (block->weak_count == 0)
      {
        delete block;
      }
      block = nullptr;
    }
  }

public:
  explicit shared_ptr(pointer_type ptr = nullptr, deleter_type d = D()) : block(ptr ? new cntrl_block{1, 0, ptr, d} : nullptr) {}

  shared_ptr(const shared_ptr& other) : block(other.block)
  {
    increment();
  }

  shared_ptr(shared_ptr&& other) noexcept : block(other.block)
  {
    other.block = nullptr;
  }

public:
  shared_ptr& operator=(const shared_ptr& other)
  {
    if (this != &other)
    {
      reset();
      block = other.block;
      increment();
    }
    return *this;
  }

  shared_ptr& operator=(shared_ptr&& other) noexcept
  {
    if (this != &other)
    {
      reset();
      block = other.block;
      other.block = nullptr;
    }
    return *this;
  }

  shared_ptr& operator=(std::nullptr_t)
  {
    reset();
    return *this;
  }

public:
  ~shared_ptr()
  {
    reset();
  }

public:
  element_type& operator*() const
  {
    if (!block || !block->ptr)
    {
      throw std::runtime_error("Dereferencing null shared_ptr");
    }
    return *block->ptr;
  }

  pointer_type operator->() const
  {
    if (!block || !block->ptr)
    {
      throw std::runtime_error("Dereferencing null shared_ptr");
    }
    return block->ptr;
  }

  element_type& operator[](size_t i) const
  {
    return get()[i];
  }

  explicit operator bool() const
  {
    return block && block->ptr;
  }

public:
  pointer_type get() const
  {
    return block ? block->ptr : nullptr;
  }

public:
  bool operator==(std::nullptr_t) const
  {
    return get() == nullptr;
  }
  bool operator!=(std::nullptr_t) const
  {
    return get() != nullptr;
  }

  template <typename U, typename W>
  bool operator==(const shared_ptr<U, W>& other) const
  {
    return get() == other.get();
  }

  template <typename U, typename W>
  bool operator!=(const shared_ptr<U, W>& other) const
  {
    return !(*this == other);
  }

public:
  size_t use_count() const
  {
    return block ? block->count : 0;
  }

  void reset()
  {
    if (block)
    {
      decrement();
    }
  }

  template <typename U = T, typename = std::enable_if_t<!std::is_array_v<U>>>
  void reset(pointer_type def_ptr, D d = D())
  {
    reset();
    block = def_ptr ? new cntrl_block{1, 0, def_ptr, d} : nullptr;
  }
};

// ********* make_shared *********

template <typename U, typename D = default_delete<U>, typename... Args>
typename std::enable_if<!std::is_array<U>::value, shared_ptr<U>>::type make_shared(Args&&... args)
{
  return shared_ptr<U>(new U(std::forward<Args>(args)...), D{});
}

template <typename U, typename D = default_delete<U>, typename... Args>
typename std::enable_if<std::is_array<U>::value, shared_ptr<U>>::type make_shared(size_t size, Args&&... args)
{
  using element_type = typename std::remove_extent<U>::type;
  return shared_ptr<U>(new element_type[size]{args...}, D{});
}

}  // namespace smrtptrs
