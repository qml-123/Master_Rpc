//
// Created by ByteDance on 2022/6/4.
//

#ifndef MASTER_RPC_SLAVE_CLIENT__H
#define MASTER_RPC_SLAVE_CLIENT__H


#include "../gen-cpp/Slave.h"
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
                    socket->setConnTimeout(1000);
                    socket->setSendTimeout(1000);
                    socket->setRecvTimeout(1000);
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
        
        class slave_client {
        public:
            typedef std::shared_ptr<slave_client> ptr;
            std::shared_ptr<SlaveClient> GetSlaveClient() {
                socket.reset((new TSocket("82.156.171.212", 9091)));
                socket->setConnTimeout(2000);
                socket->setSendTimeout(2000);
                socket->setRecvTimeout(2000);
                transport.reset((new TFramedTransport(socket)));
                protocol.reset((new TBinaryProtocol(transport)));
                transport->open();
                return std::shared_ptr<SlaveClient>(new SlaveClient(protocol));
            }
        private:
            ::apache::thrift::stdcxx::shared_ptr<TSocket> socket;
            ::apache::thrift::stdcxx::shared_ptr<TTransport> transport;
            ::apache::thrift::stdcxx::shared_ptr<TProtocol> protocol;
        };
        
        bool Slave_Conf::turnOn = false;
        std::shared_ptr<Slave_Conf> Slave_Conf::instance(new Slave_Conf());
        //master连接slave
        ::apache::thrift::stdcxx::shared_ptr<TSocket> Slave_Conf::socket(new TSocket("82.156.171.212", 9091));
        ::apache::thrift::stdcxx::shared_ptr<TTransport> Slave_Conf::transport(new TFramedTransport(socket));
        ::apache::thrift::stdcxx::shared_ptr<TProtocol> Slave_Conf::protocol(new TBinaryProtocol(transport));
        
    }}




#endif //MASTER_RPC_SLAVE_CLIENT__H
