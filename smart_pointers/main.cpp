//
// Created by skyitachi on 2018/8/19.
//

#include <stdio.h>
#include <iostream>
#include <memory>
#include <utility>
#include <thread>

#include "StrBlob.h"
#include "StrBlobPtr.h"

#include "managed_pointer.h"

int gCounter = 0;

template<class T>
class SmartPointer {
    T *ptr;
public:
    explicit SmartPointer(T *p = NULL) {
        ptr = p;
    }

    ~SmartPointer() {
        delete (ptr);
    }

    T &operator*() { return *ptr; }

    T *operator->() { return ptr; }
};

typedef void (*Callback)();

struct Unique {
    Unique() {
        id_ = gCounter++;
    }

    ~Unique() {
        std::cout << "Unique << " << id_ << " destroyed\n";
    }

    inline int Id() {
        return id_;
    }

private:
    int id_;
};

class Foo {
public:
    Foo() : uPtr(std::make_unique<Unique>()) {

    }

    ~Foo() {
        std::cout << "foo destructor called\n";
    }

    void sayHello() { std::cout << "hello \n"; }

private:
    std::unique_ptr<Unique> uPtr;
};


void test_in_callback(Callback cb) {
    cb();
}

void testArgUniquePtr(std::unique_ptr<Foo> fooPtr) {
    fooPtr->sayHello();
}

void testRefArgUniquePtr(std::unique_ptr<Foo> &fooPtr) {
    fooPtr->sayHello();
}

void testUniquePtr() {
    auto foo = std::make_unique<Foo>();
    testArgUniquePtr(std::move(foo));
    if (foo.get() == nullptr) {
        std::cout << "foo already destroyed\n";
    } else {
        std::cout << "unique ptr wrong" << std::endl;
    }

    auto foo2 = std::make_unique<Foo>();

    testRefArgUniquePtr(foo2);
    if (foo2.get() == nullptr) {
        std::cout << "foo2 already destroyed" << std::endl;
    } else {
        std::cout << "foo2 still alive" << std::endl;
    }
}

void receiveSharedPtr(std::shared_ptr<Foo> ptr) {
    std::cout << "in the receiveSharedPtr\n";
    ptr->sayHello();
    std::cout << "leave the receiveSharedPtr\n";
}

std::shared_ptr<Foo> testSharedPtrHasUniquePtrDataMember() {
    std::cout << "in the testSharedPtrHasUniquePtrDataMember\n";
    auto fooPtr = std::make_shared<Foo>();
    receiveSharedPtr(fooPtr);
    std::cout << "leave the testSharedPtrHasUniquePtrDataMember\n";
    return fooPtr;
}

class Test {
public:
    Test() : id_(gCounter++) {
        std::cout << "default constructor " << id_ << std::endl;
    }

    Test(Test &&t) {
        id_ = t.id_;
        std::cout << "move constructor" << id_ << std::endl;
    }

    ~Test() {
        std::cout << "destructor " << id_ << std::endl;
    }

private:
    int id_;
};

Test createTest() {
    return Test();
}

void threadFn(std::shared_ptr<Unique> ptr) {
    std::cout << "in the test thread \n";
//  std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << ptr->Id() << std::endl;
}

void deleter(Unique *ptr) {
    std::cout << "in the custom deleter\n";
    delete ptr;
}

// 测试shared_ptr的传引用和传值的区别
std::thread testSharedPtrPassingArgs() {
    std::shared_ptr<Unique> sPtr(new Unique(), deleter);
    std::thread t(threadFn, sPtr);
    std::cout << "sPtr should destructed\n";
    return std::move(t);
}

class Widget: public std::enable_shared_from_this<Widget> {
    public:
    Widget() {
        std::cout << "Widget constructor called" << std::endl;
    }
    ~Widget() {
        std::cout << "Widget destructor called" << std::endl;
    }
    std::shared_ptr<Widget> GetSharedObject() {
        // return std::shared_ptr<Widget>(this);
        return shared_from_this();
    } 
};


int main() {
    SmartPointer<int> ptr(new int());
    *ptr = 20;
    std::cout << *ptr << std::endl;
    StrBlob blob{"a", "b"};
    std::cout << blob.size() << std::endl;
    blob.push_back("hello");
    std::string s1 = "hello";
    blob.push_back(s1);
    test_in_callback([]() {
        // callback里分配的资源，如果不借助外力的话一定会被回收的
        auto uniqueFoo = std::make_unique<Foo>();
        uniqueFoo->sayHello();
    });
    testUniquePtr();
    std::cout << "-------------------------\n";
    auto sharedPtr = testSharedPtrHasUniquePtrDataMember();
    sharedPtr->sayHello();
    Foo *rawFooPtr = new Foo();
    std::cout << "sizeof(Foo*) = " << sizeof(rawFooPtr) << std::endl;
    std::cout << "sizeof(shared_ptr<Foo>) = " << sizeof(sharedPtr) << std::endl;

    std::cout << "-------------------------\n";
    Test t(createTest());

    std::cout << "-------------------------\n";
    {
        std::shared_ptr<Unique> sPtr(new Unique(), deleter);
        std::cout << "test shared_ptr scope\n";
    }

    std::cout << "-------------------------\n";
    std::thread t1(testSharedPtrPassingArgs());
    t1.join();

    {
        auto sp = std::make_shared<Unique>();
        auto sp2(sp);
        std::cout << "in the shared ptr test\n";
    }
    int v = 10;
    {
        learncpp::ManagedPointer<int> mp(&v);
        std::cout << "value is: " << *mp.Get() << std::endl;
    }

    {
        std::shared_ptr<Widget> p(new Widget());
        std::shared_ptr<Widget> q = p->GetSharedObject();
        std::cout << p.use_count() << std::endl;
        std::cout << q.use_count() << std::endl;
    }

    {
      std::unique_ptr<int> iv = std::make_unique<int>(10);
      if (iv) {
        std::cout << "unique_ptr is valid before moved\n";
      }
      auto iv2 = std::move(iv);
      if (!iv) {
        std::cout << "unique_ptr is invalid after moved\n";
      }
    }
    return 0;
}