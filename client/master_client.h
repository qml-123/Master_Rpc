//
// Created by ByteDance on 2022/6/19.
//

#ifndef MASTER_RPC_MASTER_CLIENT_H
#define MASTER_RPC_MASTER_CLIENT_H

#include "master_client_base.h"
#include "thread/threadpool.h"
#include "log/elog.h"
#include <memory>

namespace rpc{namespace client{
        
        class master_client {
        public:
            typedef std::shared_ptr<master_client> ptr;
            static masterClient::ptr GetMasterClient(std::string Ip, int Port) {
                return masterClient::ptr (new masterClient(Ip, Port));
            }
            
            static ptr getPtr() {
                return instance;
            }
            
            static void reset(std::pair<std::string, int> m) {
                if (client_map.count(m)) {
                    client_map[m].reset();
                }
                client_map[m] = GetMasterClient(m.first, m.second);
            }
            
            void SendLog(std::string command, std::string key, std::string value) {
                const BinLogRequest binLogRequest(command, key, value);
                auto pool = rpc::thread::getThreadPool::get()->getPool(1);
                for (auto& [address, client] : client_map) {
                    rpc::thread::ThreadPool::ThreadTask::ptr task(new rpc::thread::ThreadPool::ThreadTask("sendlog", binLogRequest, client));
                    task->senlog_cb = std::bind(&master_client::MSendLog, std::placeholders::_1, std::placeholders::_2);
                    pool->addTask(task);
                }
            }
            
            static void MSendLog(const BinLogRequest& binLogRequest, masterClient::ptr& client) {
                for (int i = 0; i < 3; i++) {
                    BinLogResponse binLogResponse;
                    try {
                        client->SendLog(binLogResponse, binLogRequest);
                    } catch (const ::apache::thrift::transport::TTransportException& ttx) {
                        log_e((std::string("SendLog ") + ttx.what()).c_str());
                        reset(std::make_pair(client->getIp(), client->getPort()));
                    }
                    if (binLogResponse.message == "success") {
                        break;
                    }
                }
                log_w(("Send Log fail from " + client->getIp() + ":" + std::to_string(client->getPort())).c_str());
            }
        
        private:
            master_client() {};
            master_client(const master_client&);
            master_client& operator=(const master_client&);
            static ptr instance;
            static std::map<std::pair<std::string, int>, masterClient::ptr> client_map;
        };
        master_client::ptr master_client::instance(new master_client());
        std::map<std::pair<std::string, int>, masterClient::ptr> master_client::client_map;
}}

#endif //MASTER_RPC_MASTER_CLIENT_H
