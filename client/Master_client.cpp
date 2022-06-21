#include "gen-cpp/Master.h"
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
//    setbuf(stdout, NULL);
//    elog_init();
//    elog_set_fmt(ELOG_LVL_ASSERT, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
//    elog_set_fmt(ELOG_LVL_ERROR, ELOG_FMT_ALL);
//    elog_set_fmt(ELOG_LVL_WARN, ELOG_FMT_ALL);
//    elog_set_fmt(ELOG_LVL_INFO, ELOG_FMT_ALL);
//    elog_set_fmt(ELOG_LVL_DEBUG, ELOG_FMT_ALL & ~ELOG_FMT_FUNC);
//    elog_set_fmt(ELOG_LVL_VERBOSE, ELOG_FMT_ALL & ~ELOG_FMT_FUNC);
//    elog_start();
    
    //master地址
    ::apache::thrift::stdcxx::shared_ptr<TSocket> socket(new TSocket("121.40.40.117", 9090)); //注意此处的ip和端口
    socket->setConnTimeout(1000);
    socket->setSendTimeout(3000);
    socket->setRecvTimeout(3000);
    ::apache::thrift::stdcxx::shared_ptr<TTransport> transport(new TFramedTransport(socket));
    ::apache::thrift::stdcxx::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
    transport->open();
    MasterClient client(protocol);
    
    int ext = 0;
    while (!ext) {
        std::vector<std::string> vv;
        std::string s;
        getline(std::cin, s);
        size_t i = 0;
        std::string x;
        for (; i < s.size(); i++) {
            while (s[i] == ' ' && i < s.size()) {
                i++;
            }
            if (i >= s.size())
                break;
            while (i < s.size() && s[i] != ' ') {
                x += s[i];
                i++;
            }
            vv.push_back(x);
            x = "";
        }
        std::cout << std::endl;
        ext = 0;
        switch (vv.size()) {
            case 1:
                if (vv[0] == "exit") {
                    ext = 1;
                }
                break;
            case 2:
                if (vv[0] == "get") {
                    GetRequest getRequest;
                    GetResponse getResponse;
                    //client发出Get请求，发送key
                    getRequest.key= vv[1];
                    try {
                        client.Get(getResponse, getRequest);
                    } catch (const ::apache::thrift::transport::TTransportException& ttx) {
                        std::cout << ttx.what() << std::endl;
                    }
                    std::cout << ("{getResponse:[message]:" + getResponse.message + " " + "[key]= " + getRequest.key + " value=" + getResponse.value + "}") << std::endl;
                } else if (vv[0] == "del") {
                    DelRequest delRequest;
                    DelResponse delResponse;
                    delRequest.key = vv[1];
                    try {
                        client.Del(delResponse, delRequest);
                    } catch (const ::apache::thrift::transport::TTransportException& ttx) {
                        std::cout << ttx.what() << std::endl;
                    }
                    std::cout << "{delResponse:" + delResponse.message + "}" << std::endl;
                }else {
                    std::cout << "error input" << std::endl;
                }
                break;
            case 3:
                if (vv[0] == "set") {
                    SetRequest setRequest;
                    SetResponse setResponse;
                    setRequest.key = vv[1];
                    setRequest.value = vv[2];
                    try {
                        client.Set(setResponse, setRequest);
                    } catch (const ::apache::thrift::transport::TTransportException& ttx) {
                        std::cout << ttx.what() << std::endl;
                    }
                    std::cout << "{setResponse:" + setResponse.message + "}" << std::endl;
                } else {
                    std::cout << "error input" << std::endl;
                }
                break;
            default:
                std::cout << "error input" << std::endl;
                break;
        }
    }
    transport->close();
    return 0;
}
