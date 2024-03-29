//
// Created by skyitachi on 2020/1/8.
//
#include <optional>
#include <string>
#include <iostream>
#include <map>
#include <type_traits>
#include <vector>
#include <algorithm>
#include <initializer_list>
#include "defer.h"
#include <jemalloc/jemalloc.h>
#include <functional>

#define JEMALLOC_NO_DEMANGLE
// std::optional has a some memory overhead
class UserName {
public:
  explicit UserName(const std::string &str) : mName(str) {
    std::cout << "UserName constructor( " << str << ")\n";
  }

  ~UserName() {
    std::cout << "UserName destructor (" << mName << ")\n";
  }

private:
  std::string mName;
};

struct Point {
  int x, y;
};

void foo(char *);

void foo(int);

int len_foo() {
  int i = 2;
  return i;
}

constexpr int len_foo_constexpr() {
  return 5;
}

constexpr int fibonacci(const int n) {
  return n == 1 || n == 2 ? 1 : fibonacci(n - 1) + fibonacci(n - 2);
}

class MagicFoo {
public:
  std::vector<int> vec;

  MagicFoo(std::initializer_list<int> list) {
    for (auto it = list.begin(); it != list.end(); ++it) {
      vec.push_back(*it);
    }
  }
};

struct UDFWrapper {
  // NOTE: important
  template<typename TR, typename... Args>
  static TR createBinaryFunction(TR (*udf_func)(Args...)) {
    std::cout << "should be ignored in the binary" << std::endl;
  }

  template<typename TR, typename TA1, typename TA2>
  static TR createBinaryFunction(TR (*udf_func)(TA1, TA2)) {
    std::cout << "in the binary" << std::endl;
  }

  template<typename TR, typename... Args>
  static TR createUnaryFunction(TR (*udf_func)(Args...)) {
    std::cout << "should be ignored in the unary" << std::endl;
  }

  template<typename TR, typename TA>
  static TR createUnaryFunction(TR (*udf_func)(TA)) {
    std::cout << "in the unary" << std::endl;
  }


// NOTE: it does not work
  template<typename TR, typename... Args>
  static TR registerCallback(TR (*udf_func)(Args...)) {
    const std::size_t num_args = sizeof... (Args);
    std::cout << "args size: " << num_args << std::endl;
    switch (num_args) {
      case 1:
        return createUnaryFunction<TR, Args...>(udf_func);
      case 2:
        return createBinaryFunction<TR, Args...>(udf_func);
      default:
        std::cout << "unexpect args" << std::endl;
    }
  }

};

int sum(int a, int b) {
  return a + b;
}

int id(int n) {
  return n;
}

void registerCallBackDirect(std::function<int(int, int)> f) {

}

void test_register_callback() {
//  registerCallback<int, int, int>([=](int a, int b)->int {
//
//  });
//
  UDFWrapper::registerCallback<int, int, int>(sum);
  UDFWrapper::registerCallback<int, int>(id);
  registerCallBackDirect(sum);
}

// function template overload?
struct BaseOperator {
  template <class TR>
  static inline TR OperationForValue(int input) {
  }
};

struct DerivedOperator: BaseOperator {
  template <class TR>
  static inline TR Operation(int input) {
    std::cout << "in the second operation" << std::endl;
    TR a = (TR)(input + 10);
    return a;
  }
};

struct UnaryExecutor: BaseOperator {
  template <class TR>
  static inline TR Operation(int input) {
    std::cout << "in the second operation" << std::endl;
    TR a = (TR)(input + 10);
    return a;
  }

  template <class TA, class TR>
  static inline TR Operation(TA input) {
    std::cout << "in the first operation" << std::endl;
    TR a = (TR) input;
    return a;
  }


};

void template_function_demo() {
  auto v = UnaryExecutor::Operation<double>(10);

  auto v2 = UnaryExecutor::Operation<double, double>(10.0);

  std::cout << v << ", " << v2 << std::endl;

  UnaryExecutor::OperationForValue<int>(10);
}

//　只有枚举类型的可以这么搞
enum class TypeKind: int8_t {
  BOOLEAN = 0,
  TINYINT = 1,
  SMALLINT = 2,
};

class ChildType {};

template<TypeKind kind>
class FlatmapColumnWriter {

};

//template<ChildType ch>
//class NestedVector {
//
//};

void test_specific_template() {
  FlatmapColumnWriter<TypeKind::BOOLEAN> t1;

//  FlatmapColumnWriter<int> t2;
}

void string_literal_demo() {

  std::string my_string = R"foo(This contains quoted parens "()")foo";

  std::cout << my_string << std::endl;
}


int main() {

  string_literal_demo();
  template_function_demo();
  DEFER([] { std::cout << "in the defer block" << std::endl; });
  std::optional<std::string> s = std::nullopt;
  if (s) {
    std::cout << "wrong answer\n";
  } else {
    std::cout << "right \n";
  }

  std::optional<int> oint = 10;
  std::cout << "oint: " << *oint << std::endl;
  std::cout << "oint: " << oint.value() << std::endl;

  std::optional<double> oduble;
  std::cout << "odouble " << oduble.value_or(10.0) << std::endl;

  std::optional<UserName> oEmpty;
  oEmpty.emplace("ysp");

  oEmpty.emplace("skyitachi");

  // NOTE: it will not call destructor
  oEmpty = UserName("abc");
  oEmpty.reset();
  std::cout << "end of program\n";
  // great
  if (auto a = true; a) {
    std::cout << "go like if condition\n";
  }
  // structured binding
  Point p = {1, 2};
  auto [x, y] = p;
  std::cout << "x is " << x << " y is " << y << std::endl;

  {
    int arr[3] = {1, 2, 3};
    auto [x, y, z] = arr;
    std::cout << x << " " << y << " " << z << std::endl;
  }
  {
    // great
    std::map<std::string, std::string> m;
    m.insert({"a", "1"});
    m.insert({"b", "2"});
    for (auto &[k, v]: m) {
      printf("key=%s, value=%s\n", k.c_str(), v.c_str());
    }
  }

  {
    // NULL 和 nullptr的区别
    // g++ 对null进行了__null的定义
    if (std::is_same<decltype(NULL), decltype(0)>::value) {
      std::cout << "NULL == 0" << std::endl;
    }

    if (std::is_same<decltype(NULL), decltype((void *) 0)>::value) {
      std::cout << "NULL == (void *) 0" << std::endl;
    }

    if (std::is_same<decltype(NULL), std::nullptr_t>::value) {
      std::cout << "NULL == nullptr" << std::endl;
    }

//    foo(0);
//    foo(nullptr);
//    foo(null);
  }
  {
    // constexpr
    char arr_1[10];
    int len = 10;
    // clang 10.0.1 会报错
    char arr_3[len];

    const int len_2 = len + 1;
    constexpr int len_2_constexpr = 1 + 2 + 3;

    char arr_4[len_2_constexpr];
  }

  {
    std::vector<int> vec = {1, 2, 3, 4};
    if (const std::vector<int>::iterator itr = std::find(vec.begin(), vec.end(), 3);
        itr != vec.end()) {
      *itr = 4;
    }
  }
  {
    MagicFoo magic_foo = {1, 2, 3, 4, 5};
    std::cout << "magicFoo: ";
    for (auto it = magic_foo.vec.begin(); it != magic_foo.vec.end(); ++it) {
      std::cout << *it << " ";
    }
    std::cout << std::endl;
  }
  {
    // type inference
    auto x = 1;
    auto y = 2;
    decltype(x + y) z;

    if (std::is_same<decltype(x), int>::value) {
      std::cout << "type x == int" << std::endl;
    }

    if (std::is_same<decltype(x), float>::value) {
      std::cout << "type x == float" << std::endl;
    }

    if (std::is_same<decltype(x), decltype(z)>::value) {
      std::cout << "type z == type x" << std::endl;
    }
  }

  {
    std::function<int(int)> f;

    if (f == nullptr) {
      std::cout << "function is nullptr" << std::endl;
    } else {
      std::cout << "empty function is not nullptr" << std::endl;
    }

    f = [&](int a)->int {
      return a + 10;
    };
    if (f == nullptr) {
      std::cout << "lambda is nullptr" << std::endl;
    } else {
      std::cout << "lambda is not nullptr: " << f(10) << std::endl;
    }
  }

  {
    int x = 42;
    std::reference_wrapper<int> ref = x;
    x = 99;
    std::cout << "ref value: " << ref << std::endl;

    ref.get() = 100;
    std::cout << "direct value: " << x << std::endl;
  }
}

// 尾返回类型推导
template<typename T, typename U>
auto add2(T x, U y) -> decltype(x + y) {
  return x + y;
}