#pragma once

template <typename T>
struct MyClassDeleter
{
  void operator()(T* t)
  {
    delete t;
  }
};

auto MyLambdaDeleter = [](auto* t) { delete t; };

template <typename T>
struct MyClassDeleter<T[]>
{
  void operator()(T* t)
  {
    delete[] t;
  }
};

auto MyLambdaDeleter2 = [](auto* t) { delete[] t; };
