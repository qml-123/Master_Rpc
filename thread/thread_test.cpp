//
// Created by ByteDance on 2022/5/25.
//

#include "threadpool.h"
#include "thread.h"
#include "gen-cpp/Master.h"
#include "log/elog.h"
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/protocol/TBinaryProtocol.h>
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using namespace ::rpc::master;
using boost::shared_ptr;

void func1(int x, int y, void* xx) {
    print("hello1");
    
}

void func2() {
    print("hello2");
}

class A {
public:
    A() {}
    A(int c, int d):a(c), b(d) {}
    int a,b;
    ~A() {std::cout << "!" << std::endl;}
};
class AA :public A{
public:
    AA(int a,int b,int _c,int _d) :A(a, b), c(_c), d(_d) {}
    int c,d;
};

int main() {
//    rpc::thread::Thread::ptr thread1(new rpc::thread::Thread(&func1, "1"));
//    rpc::thread::Thread::ptr thread2(new rpc::thread::Thread(&func2, "2"));
//    thread1->join();
//    thread2->join();
    std::vector<A*>v;
    v.push_back(new AA(1, 2, 3, 4));
    
    v.push_back(new AA(7, 6, 5, 4));
    for (auto p : v) {
        auto pp = (AA*)p;
        std::cout << p->a << " " << p->b << " " << pp->c << " " << pp->d << "\n";
    }
    
    std::shared_ptr<A> a(new A());
    while (1) {
    
    }
    a = nullptr;
    std::cout << "11" << std::endl;
    return 0;
}
