//
// Created by skyitachi on 2019-06-10.
//

#ifndef LEARNCPP_MYSTRING_H
#define LEARNCPP_MYSTRING_H

#include <cstring>
#include <boost/log/trivial.hpp>

class MyString {
public:
  MyString(): data_(new char[1]) {
    BOOST_LOG_TRIVIAL(debug) << "in the default constructor";
    size_ = 0;
    data_[0] = '\0';
  }
  
  MyString(const char *src) {
    BOOST_LOG_TRIVIAL(debug) << "in the const char * constructor";
    size_ = strlen(src);
    data_ = new char[size_ + 1];
    strcpy(data_, src);
  }
  
  MyString(const MyString& rhs) {
    BOOST_LOG_TRIVIAL(debug) << "in the copy constructor";
    size_ = rhs.size();
    data_ = new char[rhs.size()  + 1];
    strcpy(data_, rhs.data_);
  }
  
//  MyString &operator=(const MyString &rhs) {
//    BOOST_LOG_TRIVIAL(debug) << "in the operator constructor";
////    size_ = rhs.size();
////    data_ = new char[rhs.size()  + 1];
////    strcpy(data_, rhs.data_);
//    // simpler
//    // 多调用一次拷贝构造
//    MyString tmp(rhs);
//    swap(tmp);
//    return *this;
//  }
  
  // just pass by value
  MyString &operator=(MyString rhs) {
    BOOST_LOG_TRIVIAL(debug) << "in the operator constructor";
    swap(rhs);
    return *this;
  }
 
  // operator <
  bool operator<(const MyString &rhs) const {
    return strcmp(data_, rhs.data_) < 0;
  }
  
  MyString(MyString&& rhs) {
    BOOST_LOG_TRIVIAL(debug) << "in the move constructor";
    data_ = rhs.data_;
    size_ = rhs.size_;
    // Important
    rhs.data_ = nullptr;
  }
  
  ~MyString() {
    BOOST_LOG_TRIVIAL(debug) << "in the destructor";
    delete []data_;
  }
  
  size_t size() const {
    return size_;
  }
  
  void swap(MyString& rhs) {
    std::swap(data_, rhs.data_);
    std::swap(size_, rhs.size_);
  }
private:
  char* data_;
  size_t size_;
};


#endif //LEARNCPP_MYSTRING_H
