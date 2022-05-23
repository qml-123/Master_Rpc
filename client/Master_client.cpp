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


//client连master
int main(int argc, char **argv) {
    //master地址
    ::apache::thrift::stdcxx::shared_ptr<TSocket> socket(new TSocket("121.40.40.117", 9090)); //注意此处的ip和端口
    ::apache::thrift::stdcxx::shared_ptr<TTransport> transport(new TFramedTransport(socket));
    ::apache::thrift::stdcxx::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
    transport->open();
    
    MasterClient client(protocol);
    
    SetRequest setRequest;
    SetResponse setResponse;
    setRequest.key = "b";
    setRequest.value = "qmlqml";
    client.Set(setResponse, setRequest);
    std::cout << "setResponse:" + setResponse.message << std::endl;
    
    GetRequest getRequest;
    GetResponse getResponse;
    //client发出Get请求，发送key
    getRequest.key="b";
    client.Get(getResponse, getRequest);
    std::cout << "getResponse:[message]:" + getResponse.message << " " << "[key]= " + getRequest.key << " value=" << getResponse.value << std::endl;
    
    DelRequest delRequest;
    DelResponse delResponse;
    delRequest.key = "a";
    client.Del(delResponse, delRequest);
    std::cout << "delResponse:" + delResponse.message << std::endl;
    
    transport->close();
    return 0;
}
