#pragma once

class MyRes
{
  int content_;
  int valid_;

public:
  MyRes(int c = 0) : content_(c), valid_(1) {}

  MyRes(int c, double) : content_(c), valid_(1) {}

  MyRes(const MyRes &rhs) : content_(rhs.content_), valid_(rhs.valid_)
  {
    if (valid_ == 0)
      throw "Copying moved-from object";
  }

  MyRes(MyRes &&rhs) : content_(rhs.content_), valid_(rhs.valid_)
  {
    if (valid_ == 0)
      throw "Moving from moved-from object!";
    rhs.content_ = 0;
    rhs.valid_ = 0;
  }

  MyRes &operator=(const MyRes &rhs)
  {
    content_ = rhs.content_;
    valid_ = rhs.valid_;
    return *this;
  }

  MyRes &operator=(MyRes &&rhs)
  {
    if (&rhs != this)
    {
      valid_ = rhs.valid_;
      content_ = rhs.content_;
      rhs.valid_ = 0;
      rhs.content_ = 0;
    }
    if (valid_ == 0)
      throw "Moving from moved-from object!";
    return *this;
  }

  ~MyRes() {}

  void use()
  {
    if (valid_ == 0)
      throw "Using moved-from object";
  }

  void use() const
  {
    if (valid_ == 0)
      throw "Using moved-from object";
  }
};
