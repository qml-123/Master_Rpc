#include "log/elog.h"
#include "client/master_client_base.h"
#include "conf/clientconf.h"
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
    
    //master地址
    rpc::client::masterClient::ptr client(new rpc::client::masterClient("121.40.40.117", 9090));
    
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
                    label3:
                    GetRequest getRequest;
                    GetResponse getResponse;
                    //client发出Get请求，发送key
                    getRequest.key= vv[1];
                    int flag = 1;
                    try {
                        client->Get(getResponse, getRequest);
                    } catch (const ::apache::thrift::transport::TTransportException& ttx) {
                        flag = 0;
                        std::cout << "thrift error: " << ttx.what() << std::endl;
                        if (!strcmp(ttx.what(), "No more data to read.")) {
                            auto m = std::make_pair(rpc::conf::MasterConf::master_address.back().first, rpc::conf::MasterConf::master_address.back().second);
                            client.reset(new rpc::client::masterClient(m.first, m.second));
                            std::cout << "change master server to " << m.first << ":" << m.second << std::endl;
                            goto label3;
                        }
                    } catch (const ::apache::thrift::TApplicationException& ttx) {
                        flag = 0;
                        std::cout << "thrift error: " << ttx.what() << std::endl;
                    }
                    if (flag)
                        std::cout << ("{getResponse:[message]:" + getResponse.message + " " + "[key]= " + getRequest.key + " value=" + getResponse.value + "}") << std::endl;
                } else if (vv[0] == "del") {
                    label2:
                    DelRequest delRequest;
                    DelResponse delResponse;
                    delRequest.key = vv[1];
                    int flag = 1;
                    try {
                        client->Del(delResponse, delRequest);
                    } catch (const ::apache::thrift::transport::TTransportException& ttx) {
                        flag = 0;
                        std::cout << "thrift error: " << ttx.what() << std::endl;
                        if (!strcmp(ttx.what(), "No more data to read.")) {
                            auto m = std::make_pair(rpc::conf::MasterConf::master_address.back().first, rpc::conf::MasterConf::master_address.back().second);
                            client.reset(new rpc::client::masterClient(m.first, m.second));
                            std::cout << "change master server to " << m.first << ":" << m.second << std::endl;
                            goto label2;
                        }
                    } catch (const ::apache::thrift::TApplicationException& ttx) {
                        flag = 0;
                        std::cout << "thrift error: " << ttx.what() << std::endl;
                    }
                    if (flag)
                        std::cout << "{delResponse:" + delResponse.message + "}" << std::endl;
                }else {
                    std::cout << "error input" << std::endl;
                }
                break;
            case 3:
                if (vv[0] == "set") {
                    label1:
                    SetRequest setRequest;
                    SetResponse setResponse;
                    setRequest.key = vv[1];
                    setRequest.value = vv[2];
                    int flag = 1;
                    try {
                        client->Set(setResponse, setRequest);
                    } catch (const ::apache::thrift::transport::TTransportException& ttx) {
                        flag = 0;
                        std::cout << "thrift error: " << ttx.what() << std::endl;
                        if (!strcmp(ttx.what(), "No more data to read.")) {
                            auto m = std::make_pair(rpc::conf::MasterConf::master_address.back().first, rpc::conf::MasterConf::master_address.back().second);
                            client.reset(new rpc::client::masterClient(m.first, m.second));
                            std::cout << "change master server to " << m.first << ":" << m.second << std::endl;
                            goto label1;
                        }
                    } catch (const ::apache::thrift::TApplicationException& ttx) {
                        flag = 0;
                        std::cout << "thrift error: " << ttx.what() << std::endl;
                    }
                    if (flag)
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
    return 0;
}
