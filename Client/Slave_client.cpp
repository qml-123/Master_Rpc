#include "../gen-cpp/Slave.h"
#include </usr/local/include/thrift/transport/TSocket.h>
#include </usr/local/include/thrift/transport/TBufferTransports.h>
#include </usr/local/include/thrift/protocol/TBinaryProtocol.h>
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using namespace ::rpc::slave;
using boost::shared_ptr;

int main(int argc, char **argv) {
    ::apache::thrift::stdcxx::shared_ptr<TSocket> socket(new TSocket("82.156.171.212", 9091)); //注意此处的ip和端口
    ::apache::thrift::stdcxx::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
    ::apache::thrift::stdcxx::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
    transport->open();
    
    SlaveClient client(protocol);
    
    TryRequest tryRequest;
    TryResponse tryResponse;
    client.Try(tryResponse, tryRequest);
    
    RsyncRequest rsyncRequest;
    RsyncResponse rsyncResponse;
    client.Rsync(rsyncResponse, rsyncRequest);
    
    transport->close();
    return 0;
}
