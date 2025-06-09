#pragma once

#include "shared_ptr.h"

namespace smrtptrs
{

template <typename T, typename D>
class shared_ptr;

template <typename T, typename D>
class weak_ptr
{
public:
  using element_type = typename std::conditional<std::is_array<T>::value, typename std::remove_extent<T>::type, T>::type;
  using pointer_type = typename std::conditional<std::is_array<T>::value, element_type*, T*>::type;
  using deleter_type = D;

private:
  friend class shared_ptr<T, D>;
  typename shared_ptr<T, D>::cntrl_block* block;

public:
  weak_ptr() : block(nullptr) {}

  explicit weak_ptr(const shared_ptr<T, D>& shared) : block(shared.block)
  {
    if (block)
    {
      ++block->weak_count;
    }
  }

public:
  weak_ptr(const weak_ptr& other) : block(other.block)
  {
    if (block)
    {
      ++block->weak_count;
    }
  }

  weak_ptr(weak_ptr&& other) noexcept : block(other.block)
  {
    other.block = nullptr;
  }

public:
  weak_ptr& operator=(const weak_ptr& other)
  {
    if (this != &other)
    {
      if (block)
      {
        if (block->weak_count != 0)
        {
          ++block->weak_count;
        }

        if (block->weak_count == 0 && block->count == 0)
        {
          delete block;
        }
      }
      block = other.block;
      if (block)
      {
        ++block->weak_count;
      }
      block = nullptr;
    }
    return *this;
  }

  weak_ptr& operator=(weak_ptr&& other) noexcept
  {
    if (this != &other)
    {
      block = other.block;
      other.block = nullptr;
    }
    return *this;
  }

  weak_ptr& operator=(std::nullptr_t)
  {
    block = nullptr;
    return *this;
  }

public:
  ~weak_ptr()
  {
    if (block)
    {
      if (block->weak_count > 0)
      {
        --block->weak_count;
      }
      if (block->weak_count == 0 && block->count == 0)
      {
        delete block;
        block = nullptr;
      }
    }
  }

public:
  bool expired() const
  {
    return block == nullptr || block->count == 0;
  }

  explicit operator bool() const
  {
    return !expired();
  }

  std::size_t use_count() const
  {
    return block ? block->count : 0;
  }

public:
  shared_ptr<T, D> lock() const
  {
    if (this->expired())
    {
      throw std::runtime_error("Trying to lock an expired weak_ptr");
    }
    return shared_ptr<T, D>(*this, true);
  }

  void swap(weak_ptr& other) noexcept
  {
    std::swap(block, other.block);
  }

  pointer_type getPtr() const
  {
    if (block)
    {
      return block->ptr;
    }
    else
    {
      return nullptr;
    }
  }
};

}  // namespace smrtptrs
