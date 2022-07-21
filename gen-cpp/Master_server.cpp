#include "Master.h"
#include "db/mysql_rsync.h"
#include "conf/dbconf.h"
#include "client/master_client_base.h"
#include "log/elog.h"
#include "thread/threadpool.h"
#include "conf/clientconf.h"
#include <memory>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TNonblockingServer.h>

#include <thrift/transport/TTransportException.h>
#include <thrift/transport/TNonblockingServerSocket.h>

#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/PosixThreadFactory.h>
#include <future>
#include <utility>

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
    
    void SendBinLog(BinLogResponse& _return, const BinLogRequest& binLogRequest) {
        // Your implementation goes here
        printf("SendBinLog\n");
        rpc::client::masterClient client("ip", 1);
    }
    
    void Get(GetResponse& _return, const GetRequest& getRequest) {
        //master获取slave连接
        MGet(_return, getRequest);
    
        log_i(("Get message: " + _return.message + ", value: " + _return.value).c_str());
    }
    
    void MGet(GetResponse& _return, const GetRequest& getRequest) {
        log_i("MGet begin");
        std::set<std::string> messages;
        std::set<std::string> values;
        std::vector<std::shared_future<GetResponse>> condObjs;
        std::vector<slaveClient::ptr> clients;
        int flag = 1;
        for (auto p : ::rpc::conf::SlaveConf::slave_address) {
            std::promise<GetResponse> promiseObj;
            condObjs.push_back({promiseObj.get_future()});
            
            std::string Ip = p.first;
            int Port = p.second;
            slaveClient::ptr client(nullptr);
            
            try {
                client = slave_client::getPtr()->GetSlaveClient(Ip, Port);
            } catch (const ::apache::thrift::transport::TTransportException& ttx) {
                flag = 0;
                log_e((std::string("thrift error: ") + ttx.what() + " from " + Ip + ":" + std::to_string(Port)).c_str());
            }
            if (flag == 0) {
                continue;
            }
            
            auto pool = rpc::thread::getThreadPool::get()->getPool(1);
            clients.push_back(client);
            
            rpc::thread::ThreadPool::ThreadTask::ptr task(new rpc::thread::ThreadPool::ThreadTask("get", getRequest, client, std::move(promiseObj)));
            task->get_cb = std::bind(&MGetFromSlaves, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
            pool->addTask(task);
        }
        
        //slave 关闭
        if (!flag) {
            _return.message = "fail";
            return;
        }
        
        for (size_t i = 0; i < condObjs.size(); i++) {
            GetResponse getResponse;
            auto& condObj = condObjs[i];
            try {
                getResponse = condObj.get();
            }catch (const std::future_error& err) {
                log_e(err.what());
            }
            
            log_i(("Get " + getResponse.message + ", value=" + getResponse.value + " from " + clients[i]->getIp() + ":" + std::to_string(clients[i]->getPort())).c_str());
            
            messages.insert(getResponse.message);
            values.insert(getResponse.value);
        }
        
        if ((int)messages.size() != 1 || (int)values.size() != 1) {
            _return.message = "fail";
            return;
        }
        
        _return.message = *messages.begin();
        _return.value = *values.begin();
    }
    
    static void MGetFromSlaves(const GetRequest& request, slaveClient::ptr client, std::promise<GetResponse>& obj) {
        GetResponse getResponse;
        try {
            client->Get(getResponse, request);
        }catch (const ::apache::thrift::transport::TTransportException& ttx) {
            log_e((std::string("Get ") + ttx.what()).c_str());
            getResponse.message = "fail";
        }
        
        try {
            obj.set_value(getResponse);
        }catch (const std::future_error& err) {
            log_e(err.what());
        }
    }
    
    std::string MTry(const std::string& key, std::vector<slaveClient::ptr>& clients) {
        //Try
        auto pool = rpc::thread::getThreadPool::get()->getPool(1);
        std::vector<std::shared_future<TryResponse>> condObjs;
        for (size_t i = 0; i < ::rpc::conf::SlaveConf::slave_address.size(); i++) {
            std::promise<TryResponse> promiseObj;
            condObjs.push_back({promiseObj.get_future()});
        
            TryRequest tryRequest;
            tryRequest.key = key;
        
            rpc::thread::ThreadPool::ThreadTask::ptr task(new rpc::thread::ThreadPool::ThreadTask("try", tryRequest, clients[i], std::move(promiseObj)));
            task->try_cb = std::bind(&MTryFromSlave, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
            pool->addTask(task);
        }
    
        std::string ret;
        for (size_t i = 0; i < condObjs.size(); i++)  {
            TryResponse tryResponse;
            std::shared_future<TryResponse>& condObj = condObjs[i];
        
            try {
                tryResponse = condObj.get();
            }catch (const std::future_error& err) {
                log_e(err.what());
            }
        
            ret += (tryResponse.check_key ? "1" : "0");
            log_i(("Try, key=" + key + (tryResponse.check_key ? " " : " not") + " exist from " + clients[i]->getIp() + ":" + std::to_string(clients[i]->getPort())).c_str());
        }
        return ret;
    }
    
    static void MTryFromSlave(const TryRequest& tryRequest, slaveClient::ptr& client, std::promise<TryResponse>& obj) {
        TryResponse tryResponse;
        try {
            client->Try(tryResponse, tryRequest);
        }catch (const ::apache::thrift::transport::TTransportException& ttx) {
            log_e((std::string("Try ") + ttx.what()).c_str());
            tryResponse.message = "fail";
        }
        try {
            obj.set_value(tryResponse);
        }catch (const std::future_error& err) {
            log_e(err.what());
        }
    }
    
    void Set(SetResponse& _return, const SetRequest& setRequest) {
        // Your implementation goes here
        MSet(_return, setRequest);
    }
    
    void MSet(SetResponse& _return, const SetRequest& setRequest) {
        std::set<std::string> messages;
        
        std::vector<slaveClient::ptr> clients;
        auto pool = rpc::thread::getThreadPool::get()->getPool(1);
        int flag = 1;
        for (size_t i = 0; i < ::rpc::conf::SlaveConf::slave_address.size(); i++) {
            std::string Ip = ::rpc::conf::SlaveConf::slave_address[i].first;
            int Port = ::rpc::conf::SlaveConf::slave_address[i].second;
    
            slaveClient::ptr client;
            try {
                client = slave_client::getPtr()->GetSlaveClient(Ip, Port);
            } catch (const ::apache::thrift::transport::TTransportException& ttx) {
                flag = 0;
                log_e((std::string("thrift error: ") + ttx.what() + " from " + Ip + ":" + std::to_string(Port)).c_str());
            }
            if (flag == 0) {
                continue;
            }
            clients.push_back(client);
        }
        
        if (!flag) {
            _return.message = "fail";
            return;
        }
        
        std::string exist = MTry(setRequest.key, ref(clients));
        log_i(("exist: " + exist).c_str());
        std::vector<std::shared_future<SetResponse>> setObjs;
        for (size_t i = 0; i < ::rpc::conf::SlaveConf::slave_address.size(); i++) {
            std::promise<SetResponse> promiseObj;
            setObjs.push_back({promiseObj.get_future()});
            SetRequest _setRequest(setRequest);
            _setRequest.func_call = set_func(exist[i]);
            
            rpc::thread::ThreadPool::ThreadTask::ptr task(new rpc::thread::ThreadPool::ThreadTask("set", _setRequest, clients[i], std::move(promiseObj)));
            task->set_cb = std::bind(&MSetFromSlave, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
            pool->addTask(task);
        }
        
        int success = 1;
        std::vector<FinishRequest> finishRequests;
        for (size_t i = 0; i < setObjs.size(); i++) {
            SetResponse setResponse;
            try {
                setResponse = setObjs[i].get();
            } catch (const std::future_error& err) {
                log_e(err.what());
            }
            
            success &= (setResponse.message == "success");
            log_i(("Set " + setResponse.message + " from " + clients[i]->getIp() + ":" + std::to_string(clients[i]->getPort())).c_str());
    
            FinishRequest finishRequest;
            finishRequest.connection_id = setResponse.connection_id;
            finishRequests.push_back(finishRequest);
        }
        
        std::string call_func;
        if (!success) {
            _return.message = "fail";
            call_func = "rollback";
        } else {
            _return.message = "success";
            call_func = "commit";
        }
        rpc::thread::ThreadPool::ThreadTask::ptr task(new rpc::thread::ThreadPool::ThreadTask("_finish", finishRequests, call_func, clients));
        task->_finish_cb = std::bind(&MMFinish, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
        pool->addTask(task);
        log_i("end set");
    }
    

    static void MSetFromSlave(const SetRequest& setRequest, slaveClient::ptr& client, std::promise<SetResponse>& obj) {
        SetResponse setResponse;
        try {
            client->Set(setResponse, setRequest);
        }catch (const ::apache::thrift::transport::TTransportException& ttx) {
            log_e((std::string("Set ") + ttx.what()).c_str());
            setResponse.message = "fail";
        }
        try {
            obj.set_value(setResponse);
        }catch (const std::future_error& err) {
            log_e(err.what());
        }
        log_i("MSetFromSlave end");
    }
    
    void Del(DelResponse& _return, const DelRequest& delRequest) {
        MDel(_return, delRequest);
    }
    
    void MDel(DelResponse& _return, const DelRequest& delRequest) {
        std::set<std::string> messages;
    
        std::vector<slaveClient::ptr> clients;
        auto pool = rpc::thread::getThreadPool::get()->getPool(1);
        int flag = 1;
        
        for (size_t i = 0; i < ::rpc::conf::SlaveConf::slave_address.size(); i++) {
            std::string Ip = ::rpc::conf::SlaveConf::slave_address[i].first;
            int Port = ::rpc::conf::SlaveConf::slave_address[i].second;
            slaveClient::ptr client;
            try {
                client = slave_client::getPtr()->GetSlaveClient(Ip, Port);
            } catch (const ::apache::thrift::transport::TTransportException& ttx) {
                flag = 0;
                log_e((std::string("thrift error: ") + ttx.what() + " from " + Ip + ":" + std::to_string(Port)).c_str());
            }
            if (flag == 0) {
                continue;
            }
            clients.push_back(client);
        }
    
        if (!flag) {
            _return.message = "fail";
            return;
        }
        
        std::string exist = MTry(delRequest.key, ref(clients));
//        log_i(("exist: " + exist).c_str());
        std::vector<std::shared_future<DelResponse>> delObjs;
        for (size_t i = 0; i < ::rpc::conf::SlaveConf::slave_address.size(); i++) {
            if (exist[i] == '0') {
                log_i(("key =" + delRequest.key + " not exist from " + clients[i]->getIp() + ":" + std::to_string(clients[i]->getPort())).c_str());
                continue;
            }
            std::promise<DelResponse> promiseObj;
            delObjs.push_back({promiseObj.get_future()});
            
            rpc::thread::ThreadPool::ThreadTask::ptr task(new rpc::thread::ThreadPool::ThreadTask("del", delRequest, clients[i], std::move(promiseObj)));
            task->del_cb = std::bind(&MDelFromSlave, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
            pool->addTask(task);
        }
        
        int success = 1;
        std::vector<FinishRequest> finishRequests;
        std::vector<slaveClient::ptr> _clients;
        for (size_t i = 0, j = 0; i < exist.size(); i++) {
            if (exist[i] == '0')
                continue;
            DelResponse delResponse;
            try {
                delResponse = delObjs[j].get();
            } catch (const std::future_error& err) {
                log_e(err.what());
            }
        
            success &= (delResponse.message == "success");
            log_i(("Del " + delResponse.message + " from " + clients[i]->getIp() + ":" + std::to_string(clients[i]->getPort())).c_str());
        
            FinishRequest finishRequest;
            finishRequest.connection_id = delResponse.connection_id;
            finishRequests.push_back(finishRequest);
            _clients.push_back(clients[i]);
            j++;
        }
        
        std::string call_func;
        if (!success) {
            _return.message = "fail";
            call_func = "rollback";
        } else {
            _return.message = "success";
            call_func = "commit";
        }
        rpc::thread::ThreadPool::ThreadTask::ptr task(new rpc::thread::ThreadPool::ThreadTask("_finish", finishRequests, call_func, _clients));
        task->_finish_cb = std::bind(&MMFinish, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
        pool->addTask(task);
    }
    
    static void MDelFromSlave(const DelRequest& delRequest, slaveClient::ptr& client, std::promise<DelResponse>& obj) {
        DelResponse delResponse;
        try {
            client->Del(delResponse, delRequest);
        }catch (const ::apache::thrift::transport::TTransportException& ttx) {
            log_e((std::string("Del ") + ttx.what()).c_str());
            delResponse.message = "fail";
        }
        try {
            obj.set_value(delResponse);
        }catch (const std::future_error& err) {
            log_e(err.what());
        }
    }
    
    static void MMFinish(std::vector<FinishRequest> fv, std::string call_func, std::vector<slaveClient::ptr> clients) {
        log_i("MMFinish begin");
        for (int i = 0; i < 3; i++) {
            if (MFinish(fv, call_func, clients)) {
                break;
            }
        }
    }
    
    static int MFinish(std::vector<FinishRequest> fv, std::string call_func, std::vector<slaveClient::ptr> clients) {
        log_i("MFinish begin");
        auto pool = rpc::thread::getThreadPool::get()->getPool(1);
        std::vector<std::shared_future<FinishResponse>> condObjs;
        for (size_t i = 0; i < fv.size(); i++) {
            std::promise<FinishResponse> promiseObj;
            condObjs.push_back({promiseObj.get_future()});
            fv[i].call_func = call_func;
            
            rpc::thread::ThreadPool::ThreadTask::ptr task(new rpc::thread::ThreadPool::ThreadTask("finish", fv[i], clients[i], std::move(promiseObj)));
            task->finish_cb = std::bind(&MFinishFromSlave, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
            pool->addTask(task);
        }
    
        int ret = 1;
        for (size_t i = 0; i < condObjs.size(); i++)  {
            FinishResponse finishResponse;
        
            try {
                finishResponse = condObjs[i].get();
            }catch (const std::future_error& err) {
                log_e(err.what());
            }
            ret &= finishResponse.message == "success";
            log_i(("finish " + finishResponse.message + " from " + clients[i]->getIp() + ":" + std::to_string(clients[i]->getPort())).c_str());
        }
        return ret;
    }
    
    static void MFinishFromSlave(const FinishRequest& finishRe