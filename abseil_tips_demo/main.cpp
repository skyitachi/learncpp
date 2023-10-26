//
// Created by Shiping Yao on 2023/3/5.
//
#include <iostream>
#include <string_view>
#include <absl/strings/str_cat.h>
#include <absl/strings/str_join.h>
#include <absl/strings/str_replace.h>
#include <absl/strings/substitute.h>
#include <list>
#include <optional>
void TakesStringView(std::string_view s) {

}

void AlreadyHasString(const std::string& s) {
  TakesStringView(s);
}

void AlreadyHasCharStar(const char* s) {
  TakesStringView(s);
}


// name lookup first and then ADL
//namespace b {
//  void func(const std::string&);  // b::func
//  namespace internal {
//    void func(int);  // b::internal::func
//    namespace deep {
//      void test() {
//        std::string s("hello");
//        // no argument dependent lookup, just name lookup
//        func(s);  // error: finds only b::internal::func(int).
//      }
//    }  // b::internal::deep
//  }  // b::internal
//}  // b

namespace aspace {
  struct A {};
  void func(const A&);  // found by ADL name lookup on 'a'.
}  // namespace aspace

namespace bspace {
  void func(int);  // found by lexical scope name lookup
  void test() {
    aspace::A a;
    func(a);  // aspace::func(const aspace::A&) due to ADL
  }
}  // namespace bspace

class Foo{
public:
  explicit Foo(int i) {}
  void PrintDebugString() {}
};

std::unique_ptr<Foo> NewFoo() {
  return std::unique_ptr<Foo>(new Foo(1));
}

void AcceptFoo(std::unique_ptr<Foo> f) { f->PrintDebugString(); }

void Simple() {
  AcceptFoo(NewFoo());
}

void DoesNotBuild() {
  std::unique_ptr<Foo> g = NewFoo();
//  AcceptFoo(g); // DOES NOT COMPILE!
  AcceptFoo(std::move(g)); // DOES NOT COMPILE!
}

void SmarterThanTheCompilerButNot() {
  Foo* j = new Foo(2);
  // Compiles, BUT VIOLATES THE RULE and will double-delete at runtime.
  std::unique_ptr<Foo> k(j);
  std::unique_ptr<Foo> l(j);
}

int main() {
  std::string_view sv{"hello world"};
  std::cout << sv << std::endl;

  {
    auto ret = absl::StrCat("hello world", " hello abseil");
    std::cout << ret << std::endl;
  }
  {
    std::cout << absl::Substitute("Hello substitute $0 ($1)", "abseil", 42) << std::endl;
  }

  {
    std::vector<std::string> v = {"a", "b", "c"};
    std::string s = absl::StrJoin(v, "-");
    std::cout << "first join: " << s << std::endl;

    std::vector<std::string_view> v2 = {"a", "b", "c"};
    std::string s2 = absl::StrJoin(v, "-");

    std::cout << "join string_view: " << s2 << std::endl;

    std::vector<int> iv = {1, 2, 3};
    std::string s3 = absl::StrJoin(iv, "-");
    std::cout << "join int vector: " << s3 << std::endl;

    const int a[] = {1, 2, 3};
    std::string s4 = absl::StrJoin(a, "-");
    std::cout << "join int array: " << s4 << std::endl;

    std::map<std::string, int> m = {{"a", 1}, {"b", 2}, {"c", 3}};
    std::string s5 = absl::StrJoin(m, ";", absl::PairFormatter("="));
    std::cout << "join map: " << s5 << std::endl;
  }

  {
    std::vector<std::string_view> v = absl::StrSplit("a,b,c", ',');
    for(auto& item: v) {
      std::cout << " " << item;
    }
    std::cout << std::endl;

    std::vector<absl::string_view> v1 = absl::StrSplit("a,b,c", ',');
    for(auto& item: v1) {
      std::cout << " " << item;
    }
    std::cout << std::endl;

    std::set<std::string> s = absl::StrSplit("a,b,c", ',');
    std::cout << "split to sets: " << s.size() << std::endl;

    std::list<std::string> s1= absl::StrSplit("a,b,c", ",");
    std::cout << "split to list: " << s1.size() << std::endl;

  }
}
