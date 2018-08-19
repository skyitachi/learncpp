//
// Created by skyitachi on 2018/8/19.
//

#include "StrBlob.h"
#include "StrBlobPtr.h"

StrBlob::StrBlob():data(std::make_shared<std::vector<std::string>>()) {}
StrBlob::StrBlob(std::initializer_list<std::string> il):
    data(std::make_shared<std::vector<std::string>>(il)) {}

void StrBlob::pop_back() {
  check(0, "no data");
  data->pop_back();
}

std::string& StrBlob::front() {
  check(0, "no data");
  return data->front();
}

std::string& StrBlob::back() {
  check(0, "no data");
  return data->back();
}

void StrBlob::check(size_type i, const std::string &msg) const {
  if (i >= data->size())
    throw std::out_of_range(msg);
}

StrBlobPtr StrBlob::begin() {
  return StrBlobPtr(*this);
}

StrBlobPtr StrBlob::end() {
  auto ret = StrBlobPtr(*this, data->size());
  return ret;
}

