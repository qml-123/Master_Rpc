//
// Created by ByteDance on 2022/5/25.
//

#include "threadpool.h"
#include "thread.h"
#include "../gen-cpp/Master.h"
#include <iostream>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/protocol/TBinaryProtocol.h>
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using namespace ::rpc::master;
using boost::shared_ptr;


void fun1(void* x,const void* y) {
    print("begin");
    sleep(10);
    print("check");
    ::apache::thrift::stdcxx::shared_ptr<TSocket> socket(new TSocket("121.40.40.117", 9090)); //注意此处的ip和端口
    ::apache::thrift::stdcxx::shared_ptr<TTransport> transport(new TFramedTransport(socket));
    ::apache::thrift::stdcxx::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
    transport->open();
    
    print("open end");
    MasterClient client(protocol);
    
    SetRequest setRequest;
    SetResponse setResponse;
    setRequest.key = "b";
    setRequest.value = "qmlqml";
    client.Set(setResponse, setRequest);
    print("setResponse:" + setResponse.message);
    
    GetRequest getRequest;
    GetResponse getResponse;
    //client发出Get请求，发送key
    getRequest.key="b";
    client.Get(getResponse, getRequest);
    print("getResponse:[message]:" + getResponse.message + " " + "[key]= " + getRequest.key + " value=" + getResponse.value);
    
    DelRequest delRequest;
    DelResponse delResponse;
    delRequest.key = "b";
    client.Del(delResponse, delRequest);
    print("delResponse:" + delResponse.message);
    
    transport->close();
    
}

void fun2(void* x, const void* y) {
    print("hello");
    sleep(3);
}

int main() {
//    rpc::Thread::ThreadPool::ptr threadPool(new rpc::Thread::ThreadPool(1, "12"));
//    std::cout << threadPool->getName();
    rpc::thread::ThreadPool::ptr threadPool(new rpc::thread::ThreadPool(5, "qml"));
    std::function<void(void*,const void*)> cb(&fun1);
    rpc::thread::ThreadPool::ThreadTask task(cb, 1);
    threadPool->addTask(task);
    threadPool->addTask(task);
    while (1) {
        print("count:" + std::to_string(threadPool->hasTaskCount()));
        sleep(10);
    }
    return 0;
}
