//
// Created by ByteDance on 2022/6/5.
//

#ifndef MASTER_RPC_THREADGROUP_H
#define MASTER_RPC_THREADGROUP_H

#include "threadpool.h"
#include <iostream>
#include <memory>
#include <stddef.h>

namespace rpc {namespace thread {
        class ThreadGroup : public ThreadPool{
        public:
            typedef std::shared_ptr<ThreadGroup> ptr;
            
            ThreadGroup(size_t threads = 1, const std::string& name = "");
            
            ~ThreadGroup();
        
        protected:
            void tickle() override;
            void run() override;
        
        private:
            int m_epfd = 0;
            int m_tickleFds[2];
            
            MutexType m_mutex;
        };
        
    }
}

#endif //MASTER_RPC_THREADGROUP_H
