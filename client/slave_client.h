//
// Created by ByteDance on 2022/6/4.
//

#ifndef MASTER_RPC_SLAVE_CLIENT__H
#define MASTER_RPC_SLAVE_CLIENT__H


#include "gen-cpp/Slave.h"
#include "gen-cpp/Master.h"
#include <memory>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/protocol/TBinaryProtocol.h>
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using namespace ::rpc::slave;
using boost::shared_ptr;

namespace rpc{namespace client{
        //master连接slave
        class Slave_Conf {
        public:
            
            std::shared_ptr<SlaveClient> GetSlaveClient() {
                return std::shared_ptr<SlaveClient>(new SlaveClient(this->protocol));
            }
            
            static std::shared_ptr<Slave_Conf> getInstance()
            {
                if(!turnOn) {
                    socket->setConnTimeout(2000);
                    socket->setRecvTimeout(2000);
                    socket->setSendTimeout(2000);
                    transport->open();
                    turnOn = true;
                }
                return instance;
            }
        
        private:
            Slave_Conf() {};
            Slave_Conf(const Slave_Conf&);
            Slave_Conf& operator=(const Slave_Conf&);
            
            static std::shared_ptr<Slave_Conf> instance;
            static bool turnOn;
            static ::apache::thrift::stdcxx::shared_ptr<TSocket> socket;
            static ::apache::thrift::stdcxx::shared_ptr<TTransport> transport;
            static ::apache::thrift::stdcxx::shared_ptr<TProtocol> protocol;
        };
        
        bool Slave_Conf::turnOn = false;
        std::shared_ptr<Slave_Conf> Slave_Conf::instance(new Slave_Conf());
        //master连接slave
        ::apache::thrift::stdcxx::shared_ptr<TSocket> Slave_Conf::socket(new TSocket("82.156.171.212", 9091));
        ::apache::thrift::stdcxx::shared_ptr<TTransport> Slave_Conf::transport(new TFramedTransport(socket));
        ::apache::thrift::stdcxx::shared_ptr<TProtocol> Slave_Conf::protocol(new TBinaryProtocol(transport));
        
        class slaveClient {
        public:
            typedef std::shared_ptr<slaveClient> ptr;
            slaveClient(std::string _Ip, int _Port)
                    :Ip(_Ip), Port(_Port){
                socket.reset((new TSocket(Ip, Port)));
                socket->setConnTimeout(2000);
                socket->setSendTimeout(2000);
                socket->setRecvTimeout(2000);
                transport.reset((new TFramedTransport(socket)));
                protocol.reset((new TBinaryProtocol(transport)));
                transport->open();
                slave_client.reset(new SlaveClient(protocol));
            }
            
            ~slaveClient() {
                transport->close();
                slave_client.reset();
                protocol.reset();
                transport.reset();
                socket.reset();
            }
            
            void Try(TryResponse &_return, const TryRequest &tryRequest) {
                slave_client->Try(_return, tryRequest);
            }
            
            void Get(master::GetResponse &_return, const master::GetRequest &getRequest) {
                slave_client->Get(_return, getRequest);
            }
            
            void Set(master::SetResponse &_return, const master::SetRequest &setRequest) {
                slave_client->Set(_return, setRequest);
            }
            
            void Del(master::DelResponse &_return, const master::DelRequest &delRequest) {
                slave_client->Del(_return, delRequest);
            }
            
            void Finish(FinishResponse &_return, const FinishRequest &finishRequest) {
                slave_client->Finish(_return, finishRequest);
            }
            
            void Rsync(RsyncResponse &_return, const RsyncRequest &rsyncRequest) {
                slave_client->Rsync(_return, rsyncRequest);
            }
            
            std::string& getIp() {return Ip;}
            
            int getPort() {return Port;}
        private:
            std::string Ip;
            int Port;
            
            ::apache::thrift::stdcxx::shared_ptr<TSocket> socket;
            ::apache::thrift::stdcxx::shared_ptr<TTransport> transport;
            ::apache::thrift::stdcxx::shared_ptr<TProtocol> protocol;
            std::shared_ptr<SlaveClient> slave_client;
            
        };
        
        
        //
        class slave_client {
        public:
            typedef std::shared_ptr<slave_client> ptr;
            slaveClient::ptr GetSlaveClient(std::string Ip, int Port) {
                return slaveClient::ptr (new slaveClient(Ip, Port));
            }
            
            static ptr getPtr() {
                return instance;
            }
        
        private:
            slave_client() {};
            slave_client(const slave_client&);
            slave_client& operator=(const slave_client&);
            static ptr instance;
        };
        slave_client::ptr slave_client::instance(new slave_client());
        
    }}

#endif //MASTER_RPC_SLAVE_CLIENT__H
