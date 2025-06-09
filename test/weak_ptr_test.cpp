#include "../weak_ptr.h"

#include <gtest/gtest.h>

#include "../shared_ptr.h"
#include "my_res.h"

using namespace smrtptrs;

TEST(WEAK_TEST, CreationFromShared)
{
  shared_ptr<MyRes> shared_ptr(new MyRes(10));
  weak_ptr<MyRes> weak_ptr(shared_ptr);
}

TEST(WEAK_TEST, Expired)
{
  shared_ptr<MyRes> shared_ptr(new MyRes(10));
  weak_ptr<MyRes> weak_ptr(shared_ptr);
  if (weak_ptr.expired())
  {
    throw std::runtime_error("weak_ptr should not be expired.");
  }
  shared_ptr.reset();
  if (!weak_ptr.expired())
  {
    throw std::runtime_error("weak_ptr should be expired.");
  }
}

TEST(WEAK_TEST, USE_COUNT)
{
  shared_ptr<MyRes> s_ptr(new MyRes(10));
  weak_ptr<MyRes> weak_ptr1(s_ptr);
  if (weak_ptr1.use_count() != 1)
  {
    throw std::runtime_error("Incorrect use_count().");
  }
  {
    shared_ptr<MyRes> shared_ptr2 = s_ptr;
    weak_ptr<MyRes> weak_ptr2(s_ptr);
    if (weak_ptr1.use_count() != 2)
    {
      throw std::runtime_error("Incorrect use_count().");
    }
  }
  if (weak_ptr1.use_count() != 1)
  {
    throw std::runtime_error("Incorrect use_count() after shared_ptr destruction.");
  }
}
