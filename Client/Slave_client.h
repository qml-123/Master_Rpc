#include "../gen-cpp/Slave.h"
#include <memory>
#include </usr/local/include/thrift/transport/TSocket.h>
#include </usr/local/include/thrift/transport/TBufferTransports.h>
#include </usr/local/include/thrift/protocol/TBinaryProtocol.h>
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using namespace ::rpc::slave;
using boost::shared_ptr;

class Slave_Conf {
public:
    
    std::shared_ptr<SlaveClient> GetSlaveClient() {
        return std::shared_ptr<SlaveClient>(new SlaveClient(this->protocol));
    }
    
    static std::shared_ptr<Slave_Conf> getInstance()
    {
        if(!turnOn) {
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
::apache::thrift::stdcxx::shared_ptr<TSocket> Slave_Conf::socket(new TSocket("82.156.171.212", 9091));
::apache::thrift::stdcxx::shared_ptr<TTransport> Slave_Conf::transport(new TBufferedTransport(socket));
::apache::thrift::stdcxx::shared_ptr<TProtocol> Slave_Conf::protocol(new TBinaryProtocol(transport));