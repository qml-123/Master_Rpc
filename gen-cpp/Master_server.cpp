#include "Master.h"
#include "../client/Slave_client.h"
#include "../db/mysql_rsync.h"
#include "../conf/dbconf.h"
#include "../db/mysql_client.h"
#include <iostream>
#include <memory>
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
        std::cout << "Master Get begin" << std::endl;
        //master获取slave连接
        auto client = Slave_Conf::getInstance()->GetSlaveClient();
        client->Get(_return, getRequest);
        std::cout << "Master Get key=" + getRequest.key + " value=" + _return.message << std::endl;
        std::cout << std::endl;
    }
    
    void Set(SetResponse& _return, const SetRequest& setRequest) {
        // Your implementation goes here
        std::cout << "Master Try begin" << std::endl;
        auto client = Slave_Conf::getInstance()->GetSlaveClient();
        TryResponse tryResponse;
        TryRequest tryRequest;
        tryRequest.key = setRequest.key;
        client->Try(tryResponse, tryRequest);
        std::cout << "Master Try " << (tryResponse.check_key ? "exist" : "not exist") << std::endl;
        if (tryResponse.check_key) {
            _return.message = "fail";
            return;
        }
        std::cout << "Master Set begin" << std::endl;
        client->Set(_return, setRequest);
        FinishRequest finishRequest;
        FinishResponse finishResponse;
        if (_return.message == "fail") {
            finishRequest.call_func = "rollback";
        } else {
            finishRequest.call_func = "commit";
        }
        std::cout << "Master Set " + _return.message << std::endl;
        finishRequest.connection_id = _return.connection_id;
        client->Finish(finishResponse, finishRequest);
        std::cout << std::endl;
    }
    
    void Del(DelResponse& _return, const DelRequest& delRequest) {
        // Your implementation goes here
        std::cout << "Master Try begin" << std::endl;
        auto client = Slave_Conf::getInstance()->GetSlaveClient();
        TryResponse tryResponse;
        TryRequest tryRequest;
        tryRequest.key = delRequest.key;
        client->Try(tryResponse, tryRequest);
        std::cout << "Master Try " << (tryResponse.check_key ? "exist" : "not exist") << std::endl;
        if (!tryResponse.check_key) {
            _return.message = "fail";
            return;
        }
        std::cout << "Master Del begin" << std::endl;
        client->Del(_return, delRequest);
        FinishRequest finishRequest;
        FinishResponse finishResponse;
        if (_return.message == "fail") {
            finishRequest.call_func = "rollback";
        } else {
            finishRequest.call_func = "commit";
        }
        std::cout << "Master Del " + _return.message << std::endl;
        finishRequest.connection_id = _return.connection_id;
        client->Finish(finishResponse, finishRequest);
        std::cout << std::endl;
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

