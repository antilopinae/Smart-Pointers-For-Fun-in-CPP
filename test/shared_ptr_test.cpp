#include "../shared_ptr.h"

#include <gtest/gtest.h>

#include "my_res.h"
#include "shared_deleters.h"
#include "shared_functions.h"

using namespace smrtptrs;

TEST(SHARED_TEST, CreateCtor)
{
  shared_ptr<MyRes> ui0(new MyRes(3));
  auto ui1 = std::move(ui0);
  ui1->use();

  const shared_ptr<MyRes> ui2(new MyRes(980));
}

TEST(SHARED_TEST, MakeShared)
{
  auto ui0 = make_shared<MyRes>(80);
  ui0->use();

  auto ui1 = make_shared<int>(12);
  *ui1;
}

TEST(SHARED_TEST, MoveAssignment)
{
  shared_ptr<MyRes> ui_01(new MyRes(10));
  ui_01->use();

  shared_ptr<MyRes> ui_10(std::move(ui_01));
  ui_10->use();

  ui_01 = std::move(ui_10);
  ui_01->use();
}

TEST(SHARED_TEST, SharedWithFunctions)
{
  shared_ptr<MyRes> ui_1(new MyRes(5));
  shared_ptr<MyRes> ui_2(new MyRes(1));

  ui_1 = foo_shared(std::move(ui_2));
  ui_1->use();

  {
    auto res = *ui_1;
    (res).use();
  }

  ui_2 = buz_shared();
  ui_2->use();

  auto ui_3 = buz2_shared(100);
  ui_3->use();
}

TEST(SHARED_TEST, SharedDeleters)
{
  shared_ptr<MyRes, MyClassDeleterShared<MyRes>> ui1(new MyRes(15));
  shared_ptr<MyRes, decltype(MyLambdaDeleterShared)> ui2(new MyRes(14), MyLambdaDeleterShared);
}

TEST(SHARED_TEST, SharedArray)
{
  auto ui1 = make_shared<int[]>(1000);
  ui1[88];
  *ui1;

  shared_ptr<int[]> ui2(new int[1000]{});
  ui2[999];
  *ui2;

  auto ui3 = make_shared<MyRes[]>(20);
  ui3->use();
  (*ui3).use();
  ui3[1].use();

  shared_ptr<MyRes[]> ui4(new MyRes[30]);
  ui4[28].use();
  (*ui4).use();
  ui4[28].use();
  ui4->use();

  auto ui5 = make_shared<int[]>(5, 1, 2, 3, 4, 5);
  ui5[4];
  *ui5;
}

TEST(SHARED_TEST, UseCount)
{
  shared_ptr<int> ptr1(new int(10));
  {
    shared_ptr<int> ptr2 = ptr1;
    if (ptr1.use_count() != 2 || ptr2.use_count() != 2)
    {
      throw std::runtime_error("Incorrect reference count.");
    }
  }
  if (ptr1.use_count() != 1)
  {
    throw std::runtime_error("Incorrect reference count after destruction.");
  }
}

TEST(SHARED_TEST, Get)
{
  shared_ptr<int> ptr(new int(10));
  int* rawPtr = ptr.get();
  if (*rawPtr != 10)
  {
    throw std::runtime_error("Incorrect raw pointer access.");
  }
}

TEST(SHARED_TEST, Reset)
{
  shared_ptr<int> ptr(new int(10));
  ptr.reset();
  if (ptr.get() != nullptr || ptr.use_count() != 0)
  {
    throw std::runtime_error("Incorrect reset behavior.");
  }
}

TEST(SHARED_TEST, ResetWithNewPointer)
{
  shared_ptr<int> ptr(new int(10));
  ptr.reset(new int(20));
  if (*ptr != 20 || ptr.use_count() != 1)
  {
    throw std::runtime_error("Incorrect reset with new pointer behavior.");
  }
}

TEST(SHARED_TEST, CopyCtor)
{
  shared_ptr<int> ptr1(new int(10));
  shared_ptr<int> ptr2(ptr1);
  if (*ptr1 != 10 || *ptr2 != 10 || ptr1.use_count() != 2 || ptr2.use_count() != 2)
  {
    throw std::runtime_error("Incorrect copy constructor behavior.");
  }
}

TEST(SHARED_TEST, AssignmentOperator)
{
  shared_ptr<int> ptr1(new int(10));
  shared_ptr<int> ptr2;
  ptr2 = ptr1;
  if (*ptr1 != 10 || *ptr2 != 10 || ptr1.use_count() != 2 || ptr2.use_count() != 2)
  {
    throw std::runtime_error("Incorrect assignment operator behavior.");
  }
}

TEST(SHARED_TEST, Make_Shared2)
{
  auto ptr = make_shared<int>(30);
  if (*ptr != 30 || ptr.use_count() != 1)
  {
    throw std::runtime_error("Incorrect make_shared behavior (single object).");
  }
}
