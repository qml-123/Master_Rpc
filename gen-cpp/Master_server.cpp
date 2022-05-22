#include "Master.h"
#include "../client/Slave_client.h"
#include "../db/mysql_rsync.h"
#include "../conf/dbconf.h"
#include <iostream>
#include <thrift/protocol/TBinaryProtocol.h>

#include <thrift/server/TSimpleServer.h>
#include <thrift/server/TNonblockingServer.h>
#include <thrift/server/TThreadPoolServer.h>
#include <thrift/server/TThreadedServer.h>

#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/transport/TNonblockingServerSocket.h>

#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/PosixThreadFactory.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;
using namespace ::apache::thrift::concurrency;

using namespace ::rpc::db;
using namespace ::rpc::conf::db;
using namespace ::rpc::master;

//接口类，继承自MaterIf
class MasterHandler : virtual public MasterIf {
public:
    MasterHandler() {
        // Your initialization goes here
    }
    
    void Get(GetResponse& _return, const GetRequest& getRequest) {
        // Your implementation goes here
        printf("Master Get\n");
        printf("Master receive request from client:%s\n",getRequest.key.c_str());
        //master获取slave连接
        auto client = Slave_Conf::getInstance()->GetSlaveClient();
        printf("getclient success\n");
        TryResponse tryResponse;
        TryRequest tryRequest;
        client->Try(tryResponse, tryRequest);
        client->Get(_return, getRequest);
    }
    
    void Set(SetResponse& _return, const SetRequest& setRequest) {
        // Your implementation goes here
        printf("Master Set\n");
        auto client = Slave_Conf::getInstance()->GetSlaveClient();
        TryResponse tryResponse;
        TryRequest tryRequest;
        client->Try(tryResponse, tryRequest);
        client->Set(_return, setRequest);
    }
    
    void Del(DelResponse& _return, const DelRequest& delRequest) {
        // Your implementation goes here
        printf("Master Del\n");
        auto client = Slave_Conf::getInstance()->GetSlaveClient();
        TryResponse tryResponse;
        TryRequest tryRequest;
        client->Try(tryResponse, tryRequest);
        client->Del(_return, delRequest);
    }
    
};

int main(int argc, char **argv) {
    auto client = Slave_Conf::getInstance()->GetSlaveClient();
    RsyncResponse rsyncResponse;
    RsyncRequest  rsyncRequest;
    rsyncRequest.database = MysqlConf::db_name;
    rsyncRequest.sql_file = mysql_rsync::get_sql();
    rsyncRequest.message = "rsync";
    client->Rsync(rsyncResponse, rsyncRequest);
    std::cout << "rsync response:" << rsyncResponse.message << std::endl;
    
    int port = 9090;
    //业务接口,暴露给client
    ::apache::thrift::stdcxx::shared_ptr<MasterHandler> handler(new MasterHandler());
    ::apache::thrift::stdcxx::shared_ptr<TProcessor> processor(new MasterProcessor(handler));
    //Master监听在9090端口
    ::apache::thrift::stdcxx::shared_ptr<TNonblockingServerSocket> serverTransport(new TNonblockingServerSocket(port));
    ::apache::thrift::stdcxx::shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());
    
    //任务处理线程池
    ::apache::thrift::stdcxx::shared_ptr<ThreadManager> threadManager = ThreadManager::newSimpleThreadManager(15);
    ::apache::thrift::stdcxx::shared_ptr<PosixThreadFactory> threadFactory(new PosixThreadFactory());
    
    threadManager->threadFactory(threadFactory);
    threadManager->start();
    
    TNonblockingServer server(processor, protocolFactory, serverTransport, threadManager);
    server.setNumIOThreads(5);//设置处理连接请求线程数
    server.serve();
    
    return 0;
}

