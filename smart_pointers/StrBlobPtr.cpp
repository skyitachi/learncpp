//
// Created by skyitachi on 2018/8/19.
//

#include "StrBlob.h"
#include "StrBlobPtr.h"

std::shared_ptr<std::vector<std::string>>
StrBlobPtr::check(std::size_t i, const std::string &msg) const {
  auto ret = wptr.lock();
  if (!ret)
    throw std::runtime_error("unbound StrBlobPtr");
  if (i >= ret->size())
    throw std::out_of_range(msg);
  return ret;
}

std::string& StrBlobPtr::deref() const {
  auto p = check(curr, "no data");
  return (*p)[curr];
}

StrBlobPtr& StrBlobPtr::incr() {
  check(curr, "to much data");
  ++curr;
  return *this;
}

StrBlobPtr::StrBlobPtr(StrBlob &a, size_t sz): wptr(a.data), curr(sz) {}

