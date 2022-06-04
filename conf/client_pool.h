//
// Created by ByteDance on 2022/6/4.
//

#ifndef MASTER_RPC_CLIENT_POOL_H
#define MASTER_RPC_CLIENT_POOL_H

#include "thread/mutex.h"
#include "thread/noncopyable.h"
#include "dbconf.h"
#include <list>
#include <memory>
#include <algorithm>

namespace rpc { namespace conf {
        template<class T>
        class ClientPool : public thread::Noncopyable {
        public:
            typedef std::shared_ptr<T> ptr;
        public:
            ClientPool() {
            }
            
            virtual ~ClientPool() {
                this->idle.clear();
                this->busy.clear();
            }
            
            virtual void addClient() = 0;
            
            typename std::list<ptr>::iterator getClient() {
                rpc::thread::Mutex::Lock lock(mutex);
                std::shared_ptr<T> client = this->idle.back();
                this->idle.pop_back();
                this->busy.push_front(client);
                return this->busy.begin();
            }
            
            void delClient(typename std::list<ptr>::iterator iterator) {
                rpc::thread::Mutex::Lock lock(mutex);
                this->idle.push_back(*iterator);
                this->busy.erase(iterator);
            }
            
            size_t total_count() {
                return this->idle.size() + this->busy.size();
            }
            
            size_t free_count() {
                return this->idle.size();
            }
        
        protected:
            std::list<ptr> idle, busy; //空闲， 被使用
            rpc::thread::Mutex mutex;
        };
}}

#endif //MASTER_RPC_CLIENT_POOL_H
