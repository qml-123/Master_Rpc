//
// Created by ByteDance on 2022/6/19.
//

#ifndef MASTER_RPC_MASTER_CLIENT_BASE_H
#define MASTER_RPC_MASTER_CLIENT_BASE_H

#include "gen-cpp/Slave.h"
#include "gen-cpp/Master.h"
#include <memory>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/protocol/TBinaryProtocol.h>
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using namespace ::rpc::master;
using boost::shared_ptr;

namespace rpc{namespace client{
        
        class masterClient {
        public:
            typedef std::shared_ptr<masterClient> ptr;
            masterClient(std::string _Ip, int _Port)
                :Ip(_Ip), Port(_Port){
                socket.reset(new TSocket(Ip, Port));
                socket->setConnTimeout(1000);
                socket->setSendTimeout(1000);
                socket->setRecvTimeout(1000);
                transport.reset(new TFramedTransport(socket));
                protocol.reset(new TBinaryProtocol(transport));
                transport->open();
                master_client.reset(new MasterClient(protocol));
            }
            
            ~masterClient() {
                transport->close();
                master_client.reset();
                protocol.reset();
                transport.reset();
                socket.reset();
            }
            
            void SendLog(BinLogResponse& _return, const BinLogRequest& binLogRequest) {
                master_client->SendBinLog(_return, binLogRequest);
            }
            
            void Get(GetResponse& _return, const GetRequest& getRequest) {
                master_client->Get(_return, getRequest);
            }
            
            void Set(SetResponse& _return, const SetRequest& setRequest) {
                master_client->Set(_return, setRequest);
            }
            
            void Del(DelResponse& _return, const DelRequest& delRequest) {
                master_client->Del(_return, delRequest);
            }
            
            std::string getIp() {return Ip;}
            int getPort() {return Port;}
            
        private:
            std::string Ip;
            int Port;
            
            ::apache::thrift::stdcxx::shared_ptr<TSocket> socket;
            ::apache::thrift::stdcxx::shared_ptr<TTransport> transport;
            ::apache::thrift::stdcxx::shared_ptr<TProtocol> protocol;
            std::shared_ptr<MasterClient> master_client;
            
        };
        
    }}

#endif //MASTER_RPC_MASTER_CLIENT_BASE_H
