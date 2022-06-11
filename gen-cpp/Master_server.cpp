#include "Master.h"
#include "../db/mysql_rsync.h"
#include "../conf/dbconf.h"
#include "../db/mysql_client.h"
#include "../client/slave_client_pool.h"
#include "log/elog.h"
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
using namespace ::rpc::client;

//接口类，继承自MaterIf
class MasterHandler : virtual public MasterIf {
public:
    MasterHandler() {
        // Your initialization goes here
    }
    
    void Get(GetResponse& _return, const GetRequest& getRequest) {
        // Your implementation goes here
//        std::cout << "Master Get begin" << std::endl;
        log_i("Get begin");
        //master获取slave连接
//        auto client = Slave_Conf::getInstance()->GetSlaveClient();
        log_i(("SlaveClient free_count: " + std::to_string(SlaveClientPool::get()->free_count())).c_str());
        auto client_iterator = SlaveClientPool::get()->getClient();
        auto client = *client_iterator;
    
        log_i(("SlaveClient free_count: " + std::to_string(SlaveClientPool::get()->free_count())).c_str());
        
        client->Get(_return, getRequest);
        log_i(("Get key=" + getRequest.key + " value=" + _return.message).c_str());

        SlaveClientPool::get()->delClient(client_iterator);
    
        log_i(("SlaveClient free_count: " + std::to_string(SlaveClientPool::get()->free_count())).c_str());
        log_i("Get end");
    }
    
    void Set(SetResponse& _return, const SetRequest& setRequest) {
        // Your implementation goes here
        log_i("Set Try begin");
//        auto client = Slave_Conf::getInstance()->GetSlaveClient();
        log_i(("SlaveClient free_count: " + std::to_string(SlaveClientPool::get()->free_count())).c_str());
        auto client_iterstor = SlaveClientPool::get()->getClient();
        auto client = *client_iterstor;
    
        log_i(("SlaveClient free_count: " + std::to_string(SlaveClientPool::get()->free_count())).c_str());
        
        TryResponse tryResponse;
        TryRequest tryRequest;
        tryRequest.key = setRequest.key;
        client->Try(tryResponse, tryRequest);
    
        const SetRequest setReq(setRequest.key, setRequest.value, set_func(tryResponse.check_key));
    
        log_i(("Set begin, func_call=" + set_func(tryResponse.check_key)).c_str());
        client->Set(_return, setReq);
        FinishRequest finishRequest;
        FinishResponse finishResponse;
        if (_return.message == "fail") {
            log_w(("Set key(" + setReq.key +") " + _return.message + " , " + finishRequest.call_func).c_str());
            finishRequest.call_func = "rollback";
        } else {
            log_i(("Set key(" + setReq.key +") " + _return.message + " , " + finishRequest.call_func).c_str());
            finishRequest.call_func = "commit";
        }
        
        log_i(("Set " + _return.message + " , " + finishRequest.call_func).c_str());
        finishRequest.connection_id = _return.connection_id;
        client->Finish(finishResponse, finishRequest);
        
        SlaveClientPool::get()->delClient(client_iterstor);
        log_i(("SlaveClient free_count: " + std::to_string(SlaveClientPool::get()->free_count())).c_str());
        log_i("Set end");
    }
    
    void Del(DelResponse& _return, const DelRequest& delRequest) {
        // Your implementation goes here
        log_i("Try begin");
//        auto client = Slave_Conf::getInstance()->GetSlaveClient();
        log_i(("SlaveClient free_count: " + std::to_string(SlaveClientPool::get()->free_count())).c_str());

        auto client_iterator = SlaveClientPool::get()->getClient();
        auto client = *client_iterator;
    
    
        log_i(("SlaveClient free_count: " + std::to_string(SlaveClientPool::get()->free_count())).c_str());
        
        TryResponse tryResponse;
        TryRequest tryRequest;
        tryRequest.key = delRequest.key;
        client->Try(tryResponse, tryRequest);
        
        if (!tryResponse.check_key) {
            log_w(("try fail, key(" + tryRequest.key + ") is not exist").c_str());
            _return.message = "fail";
            return;
        }
        log_i("Del begin");
        
        client->Del(_return, delRequest);
        FinishRequest finishRequest;
        FinishResponse finishResponse;
        if (_return.message == "fail") {
            log_w(("Del key(" + delRequest.key +") " + _return.message + " , " + finishRequest.call_func).c_str());
            finishRequest.call_func = "rollback";
        } else {
            log_i(("Del key(" + delRequest.key +") " + _return.message + " , " + finishRequest.call_func).c_str());
            finishRequest.call_func = "commit";
        }
        
        finishRequest.connection_id = _return.connection_id;
        client->Finish(finishResponse, finishRequest);

        
        SlaveClientPool::get()->delClient(client_iterator);
        log_i(("SlaveClient free_count: " + std::to_string(SlaveClientPool::get()->free_count())).c_str());
        log_i("Del end");
    }
    
};

int main(int argc, char **argv) {
    //log init
    setbuf(stdout, NULL);
    elog_init();
    elog_set_fmt(ELOG_LVL_ASSERT, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
    elog_set_fmt(ELOG_LVL_ERROR, ELOG_FMT_ALL);
    elog_set_fmt(ELOG_LVL_WARN, ELOG_FMT_ALL);
    elog_set_fmt(ELOG_LVL_INFO, ELOG_FMT_ALL);
    elog_set_fmt(ELOG_LVL_DEBUG, ELOG_FMT_ALL & ~ELOG_FMT_FUNC);
    elog_set_fmt(ELOG_LVL_VERBOSE, ELOG_FMT_ALL & ~ELOG_FMT_FUNC);
    elog_start();
    
    auto client_iterator = SlaveClientPool::get()->getClient();
    auto client = *client_iterator;
    
    RsyncResponse rsyncResponse;
    RsyncRequest  rsyncRequest;
    rsyncRequest.database = MysqlConf::db_name;
    rsyncRequest.sql_file = mysql_rsync::get_sql();
    rsyncRequest.message = "rsync";
    client->Rsync(rsyncResponse, rsyncRequest);
    std::cout << "rsync response:" << rsyncResponse.message << std::endl;
    SlaveClientPool::get()->delClient(client_iterator);
    
    int port = 9090;
    //业务接口,暴露给client
    ::apache::thrift::stdcxx::shared_ptr<MasterHandler> handler(new MasterHandler());
    ::apache::thrift::stdcxx::shared_ptr<TProcessor> processor(new MasterProcessor(handler));
    //Master监听在9090端口
    ::apache::thrift::stdcxx::shared_ptr<TNonblockingServerSocket> serverTransport(new TNonblockingServerSocket(port));
    ::apache::thrift::stdcxx::shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());
    
    //任务处理线程池
    ::apache::thrift::stdcxx::shared_ptr<ThreadManager> threadManager = ThreadManager::newSimpleThreadManager(15);
    ::apache::thrift::stdcxx::shared_ptr<PlatformThreadFactory> threadFactory(new PlatformThreadFactory());
    
    threadManager->threadFactory(threadFactory);
    threadManager->start();
    
    TNonblockingServer server(processor, protocolFactory, serverTransport, threadManager);
    server.setNumIOThreads(5);//设置处理连接请求线程数
    server.serve();
    
    return 0;
}

