#include "../gen-cpp/Master.h"
#include "log/elog.h"
#include <iostream>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/transport/TTransportException.h>
#include <thrift/protocol/TBinaryProtocol.h>
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using namespace ::rpc::master;
using boost::shared_ptr;


//client连master
int main(int argc, char **argv) {
    setbuf(stdout, NULL);
    elog_init();
    elog_set_fmt(ELOG_LVL_ASSERT, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
    elog_set_fmt(ELOG_LVL_ERROR, ELOG_FMT_ALL);
    elog_set_fmt(ELOG_LVL_WARN, ELOG_FMT_ALL);
    elog_set_fmt(ELOG_LVL_INFO, ELOG_FMT_ALL);
    elog_set_fmt(ELOG_LVL_DEBUG, ELOG_FMT_ALL & ~ELOG_FMT_FUNC);
    elog_set_fmt(ELOG_LVL_VERBOSE, ELOG_FMT_ALL & ~ELOG_FMT_FUNC);
    elog_start();
    
    //master地址
    ::apache::thrift::stdcxx::shared_ptr<TSocket> socket(new TSocket("121.40.40.117", 9090)); //注意此处的ip和端口
//    socket->setConnTimeout(2000);
//    socket->setSendTimeout(2000);
//    socket->setRecvTimeout(2000);
    ::apache::thrift::stdcxx::shared_ptr<TTransport> transport(new TFramedTransport(socket));
    ::apache::thrift::stdcxx::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
    transport->open();
    MasterClient client(protocol);
    
    SetRequest setRequest;
    SetResponse setResponse;
    setRequest.key = "b";
    setRequest.value = "qml";
    try {
        client.Set(setResponse, setRequest);
    } catch (const ::apache::thrift::transport::TTransportException& ttx) {
        log_e(ttx.what());
    }
    log_i(("setResponse:" + setResponse.message).c_str());
    
    GetRequest getRequest;
    GetResponse getResponse;
    //client发出Get请求，发送key
    getRequest.key="b";
    try {
        client.Get(getResponse, getRequest);
    } catch (const ::apache::thrift::transport::TTransportException& ttx) {
        log_e(ttx.what());
    }
    
    log_i(("getResponse:[message]:" + getResponse.message + " " + "[key]= " + getRequest.key + " value=" + getResponse.value).c_str());
    
    DelRequest delRequest;
    DelResponse delResponse;
    delRequest.key = "c";
    try {
        client.Del(delResponse, delRequest);
    } catch (const ::apache::thrift::transport::TTransportException& ttx) {
        log_e(ttx.what());
    }
    log_i(("delResponse:" + delResponse.message).c_str());
    
    transport->close();
    return 0;
}
