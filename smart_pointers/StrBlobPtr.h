//
// Created by skyitachi on 2018/8/19.
//

#ifndef LEARNCPP_STRBLOBPTR_H
#define LEARNCPP_STRBLOBPTR_H


#include <vector>
#include <string>
#include <memory>

class StrBlob;

class StrBlobPtr {
public:
  StrBlobPtr(): curr(0) { }
  StrBlobPtr(StrBlob &a, size_t sz = 0);
  std::string& deref() const;
  StrBlobPtr& incr();

private:
  std::size_t curr;
  std::shared_ptr<std::vector<std::string>> check(std::size_t, const std::string &) const;
  std::weak_ptr<std::vector<std::string>> wptr;
};


#endif //LEARNCPP_STRBLOBPTR_H
