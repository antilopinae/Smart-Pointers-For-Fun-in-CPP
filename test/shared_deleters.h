#pragma once

template <typename T>
struct MyClassDeleterShared
{
  void operator()(T* t)
  {
    delete t;
  }
};

auto MyLambdaDeleterShared = [](auto* t) { delete t; };

template <typename T>
struct MyClassDeleterShared<T[]>
{
  void operator()(T* t)
  {
    delete[] t;
  }
};

auto MyLambdaDeleter2Shared = [](auto* t) { delete[] t; };
