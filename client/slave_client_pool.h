//
// Created by ByteDance on 2022/6/4.
//

#ifndef MASTER_RPC_SLAVE_CLIENT_POOL_H
#define MASTER_RPC_SLAVE_CLIENT_POOL_H

#include "gen-cpp/Slave.h"
#include "conf/client_pool.h"
#include "slave_client.h"
#include "conf/conf.h"

namespace rpc{namespace client{
    class SlaveClientPool : public conf::ClientPool<SlaveClient> {
    public:
    
        static std::shared_ptr<SlaveClientPool> get() {
            return instance;
        }
        
        void addClient() {
            idle.push_front(Slave_Conf::getInstance()->GetSlaveClient());
        }
    
        static std::shared_ptr<SlaveClientPool> instance;

    private:
        SlaveClientPool() {
            //10
            for (int i = 0; i < 10; i++) {
                idle.push_back(Slave_Conf::getInstance()->GetSlaveClient());
            }
        }
    };
    
    std::shared_ptr<SlaveClientPool> SlaveClientPool::instance(new SlaveClientPool());
    
}}

#endif //MASTER_RPC_SLAVE_CLIENT_POOL_H
