#pragma once

#include "../unique_ptr.h"
#include "my_res.h"

smrtptrs::unique_ptr<MyRes>&& foo(smrtptrs::unique_ptr<MyRes>&& x)
{
  x->use();
  return std::move(x);
}

decltype(auto) buz()
{
  smrtptrs::unique_ptr<MyRes> t{new MyRes{17}};
  return t;
}

decltype(auto) getSome = [](int p) { return smrtptrs::unique_ptr<MyRes>{new MyRes{p}}; };

auto buz2(int p) -> decltype(getSome(p))
{
  return getSome(p);
}
