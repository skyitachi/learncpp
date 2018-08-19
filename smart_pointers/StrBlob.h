//
// Created by skyitachi on 2018/8/19.
//

#ifndef LEARNCPP_STRBLOB_H
#define LEARNCPP_STRBLOB_H

#include <vector>
#include <string>
#include <memory>
#include <initializer_list>

class StrBlobPtr;
class StrBlob {
public:
  friend class StrBlobPtr;
  typedef std::vector<std::string>::size_type size_type;
  StrBlob();
  StrBlob(std::initializer_list<std::string> il);
  size_type size() const { return data->size(); }
  bool empty() const { return data->empty(); }
  void push_back(const std::string &t) { data->push_back(t) ;}
  void pop_back();

  std::string &front();
  std::string &back();
  StrBlobPtr begin();
  StrBlobPtr end();
private:
  std::shared_ptr<std::vector<std::string>> data;
  void check(size_type i, const std::string &msg) const;
};

#endif //LEARNCPP_STRBLOB_H