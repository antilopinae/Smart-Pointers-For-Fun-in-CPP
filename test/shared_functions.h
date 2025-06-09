#pragma once

#include "../shared_ptr.h"
#include "my_res.h"

smrtptrs::shared_ptr<MyRes>&& foo_shared(smrtptrs::shared_ptr<MyRes>&& x)
{
  x->use();
  return std::move(x);
}

decltype(auto) buz_shared()
{
  smrtptrs::shared_ptr<MyRes> t{new MyRes{17}};
  return t;
}

decltype(auto) getSome_shared = [](int p) { return smrtptrs::shared_ptr<MyRes>{new MyRes{p}}; };

auto buz2_shared(int p) -> decltype(getSome_shared(p))
{
  return getSome_shared(p);
}
