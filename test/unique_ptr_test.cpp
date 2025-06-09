#include "../unique_ptr.h"

#include <gtest/gtest.h>

#include "my_res.h"
#include "unique_deleters.h"
#include "unique_functions.h"

TEST(UNIQUE_TEST, CreateCtor)
{
  smrtptrs::unique_ptr<MyRes> ui0(new MyRes(3));
  auto ui1 = std::move(ui0);
  ui1->use();

  const smrtptrs::unique_ptr<MyRes> ui2(new MyRes(980));
}

TEST(UNIQUE_TEST, MakeUnique)
{
  auto ui0 = smrtptrs::make_unique<MyRes>(80);
  ui0->use();

  auto ui1 = smrtptrs::make_unique<int>(12);
  *ui1;
}

TEST(UNIQUE_TEST, MoveAssignment)
{
  smrtptrs::unique_ptr<MyRes> ui_01(new MyRes(10));
  ui_01->use();

  smrtptrs::unique_ptr<MyRes> ui_10(std::move(ui_01));
  ui_10->use();

  ui_01 = std::move(ui_10);
  ui_01->use();
}

TEST(UNIQUE_TEST, UniqueWithFunctions)
{
  smrtptrs::unique_ptr<MyRes> ui_1(new MyRes(5));
  smrtptrs::unique_ptr<MyRes> ui_2(new MyRes(1));

  ui_1 = foo(std::move(ui_2));
  ui_1->use();

  {
    auto res = *ui_1;
    (res).use();
  }

  ui_2 = buz();
  ui_2->use();

  auto ui_3 = buz2(100);
  ui_3->use();
}

TEST(UNIQUE_TEST, UniqueDeleters)
{
  smrtptrs::unique_ptr<MyRes, MyClassDeleter<MyRes>> ui1(new MyRes(15));
  smrtptrs::unique_ptr<MyRes, decltype(MyLambdaDeleter)> ui2(new MyRes(14), MyLambdaDeleter);
}

TEST(UNIQUE_TEST, TestingUniqueArray)
{
  auto ui1 = smrtptrs::make_unique<int[]>(1000);
  ui1[88];
  *ui1;

  smrtptrs::unique_ptr<int[]> ui2(new int[1000]{});
  ui2[999];
  *ui2;

  auto ui3 = smrtptrs::make_unique<MyRes[]>(20);
  ui3->use();
  (*ui3).use();
  ui3[1].use();

  smrtptrs::unique_ptr<MyRes[]> ui4(new MyRes[30]);
  ui4[28].use();
  (*ui4).use();
  ui4[28].use();
  ui4->use();

  auto ui5 = smrtptrs::make_unique<int[]>(5, 1, 2, 3, 4, 5);
  ui5[4];
  *ui5;
}
