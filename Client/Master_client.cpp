#include "../gen-cpp/Master.h"
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/protocol/TBinaryProtocol.h>
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using namespace ::rpc::master;
using boost::shared_ptr;

int main(int argc, char **argv) {
    ::apache::thrift::stdcxx::shared_ptr<TSocket> socket(new TSocket("121.40.40.117", 9090)); //注意此处的ip和端口
    ::apache::thrift::stdcxx::shared_ptr<TTransport> transport(new TFramedTransport(socket));
    ::apache::thrift::stdcxx::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
    transport->open();
    
    MasterClient client(protocol);
    
    GetRequest getRequest;
    GetResponse getResponse;
    client.Get(getResponse, getRequest);
    
    SetRequest setRequest;
    SetResponse setResponse;
    client.Set(setResponse, setRequest);
    
    DelRequest delRequest;
    DelResponse delResponse;
    client.Del(delResponse, delRequest);
    
    transport->close();
    return 0;
}
