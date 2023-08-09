#include <setjmp.h>
#include <iostream>

struct A {
        A(int v) { v_ = v; }
        ~A() { std::cout << v_ << " die.\n"; }
        int v_;
};

jmp_buf env;

bool flag = true;
bool doSomething() {
        std::cout << "do something\n";
        flag = !flag;
        return flag;
}

void funInt(int depth) {
        A* a = new A(depth);
        A a1{depth + 10};
        if (depth < 1) {
                funInt(depth + 1);
        } else {
                if (!doSomething()) longjmp(env, 1);
        }
        delete a;
}

void fun() {
        auto v = setjmp(env);
        std::cout << "receive value from auto jump: " << v << std::endl;
        funInt(0);
}

int main() {
        fun();
}