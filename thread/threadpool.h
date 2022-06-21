//
// Created by ByteDance on 2022/5/24.
//

#ifndef MASTER_RPC_THREADPOOL_H
#define MASTER_RPC_THREADPOOL_H

#include "noncopyable.h"
#include "mutex.h"
#include "thread.h"
#include "conf/conf.h"
#include "client/slave_client.h"
#include "client/master_client_base.h"
#include <functional>
#include <vector>
#include <iostream>
#include <atomic>
#include <queue>
#include <condition_variable>
#include <map>
#include <future>

namespace rpc{namespace thread{
        class ThreadPool : Noncopyable{
        public:
            struct ThreadTask {
                typedef std::shared_ptr<ThreadTask> ptr;
                typedef std::function<void(const master::GetRequest&, rpc::client::slaveClient::ptr, std::promise<master::GetResponse>&) >                     getfunc;
                typedef std::function<void(const master::SetRequest&, rpc::client::slaveClient::ptr&, std::promise<master::SetResponse>&) >                    setfunc;
                typedef std::function<void(const master::DelRequest&, rpc::client::slaveClient::ptr&, std::promise<master::DelResponse>&) >                    delfunc;
                typedef std::function<void(const TryRequest&,         rpc::client::slaveClient::ptr&, std::promise<TryResponse>&) >                            tryfunc;
                typedef std::function<void(const FinishRequest&,      rpc::client::slaveClient::ptr&, std::promise<FinishResponse>&) >                         finishfunc;
                typedef std::function<void(std::vector<FinishRequest>,std::string,                    std::vector<rpc::client::slaveClient::ptr>&) >           _finishfunc;
                typedef std::function<void(const master::BinLogRequest&, rpc::client::masterClient::ptr&)>                                                     senlogfunc;
    
                std::string call_name;
    
                const master::GetRequest    getRequest;
                const master::SetRequest    setRequest;
                const master::DelRequest    delRequest;
                const TryRequest            tryRequest;
                const FinishRequest         finishRequest;
                const master::BinLogRequest binLogRequest;
                
                //执行函数
                getfunc     get_cb;
                setfunc     set_cb;
                delfunc     del_cb;
                tryfunc     try_cb;
                finishfunc  finish_cb;
                _finishfunc _finish_cb;
                senlogfunc  senlog_cb;
    
                rpc::client::slaveClient::ptr client;
                rpc::client::masterClient::ptr m_client;
                
                std::promise<master::GetResponse>   getObj;
                std::promise<master::SetResponse>   setObj;
                std::promise<master::DelResponse>   delObj;
                std::promise<TryResponse>           tryObj;
                std::promise<FinishResponse>        finishObj;
    
                std::vector<FinishRequest> fv;
                std::string call_func;
                std::vector<rpc::client::slaveClient::ptr> clients;
                
                ThreadTask(std::string name, const master::GetRequest& request, rpc::client::slaveClient::ptr _client, std::promise<master::GetResponse> promiseObj)
                    : call_name(name), getRequest(request), client(_client){
                    getObj = std::move(promiseObj);
                }
                ThreadTask(std::string name, const master::SetRequest& request, rpc::client::slaveClient::ptr _client, std::promise<master::SetResponse> promiseObj)
                        : call_name(name), setRequest(request), client(_client){
                    setObj = std::move(promiseObj);
                }
                ThreadTask(std::string name, const master::DelRequest& request, rpc::client::slaveClient::ptr _client, std::promise<master::DelResponse> promiseObj)
                        : call_name(name), delRequest(request), client(_client){
                    delObj = std::move(promiseObj);
                }
                ThreadTask(std::string name, const TryRequest& request, rpc::client::slaveClient::ptr _client, std::promise<TryResponse> promiseObj)
                        : call_name(name), tryRequest(request), client(_client){
                    tryObj = std::move(promiseObj);
                }
                ThreadTask(std::string name, const FinishRequest& request, rpc::client::slaveClient::ptr _client, std::promise<FinishResponse> promiseObj)
                        : call_name(name), finishRequest(request), client(_client){
                    finishObj = std::move(promiseObj);
                }
                ThreadTask(std::string name, std::vector<FinishRequest> _fv, std::string _call_func, std::vector<rpc::client::slaveClient::ptr> _clients)
                        : call_name(name), fv(_fv), call_func(_call_func),clients(_clients){
                }
                ThreadTask(std::string name, const BinLogRequest& request, rpc::client::masterClient::ptr _client)
                        : call_name(name), binLogRequest(request), m_client(_client){
                }
            };
        
        public:
            typedef std::shared_ptr<ThreadPool> ptr;
            typedef Mutex MutexType;
            
            ThreadPool(size_t threads = 1, const std::string& name = "");
            
            virtual ~ThreadPool();
            
            const std::string& getName()const {return m_name;}
            
            static ThreadPool* GetThis();
            
            void start();
            
            void stop();
            
            void addTask(ThreadTask::ptr task);
            
            bool hasIdleThreads() {return m_idleThreadCount > 0;}
            
            bool hasTaskCount() {return m_taskCount > 0; }
            
        protected:
            
            void run();
            
            //是否可以停止
            bool stopping() {return m_stopping; }
            
            
            void setThis();
            
            //idle Thread count 空闲
            std::atomic<size_t> m_idleThreadCount = {0};
        
        private:
            //线程池
            std::vector<Thread::ptr> m_threads;
            //task queue
            std::queue<ThreadTask::ptr> m_taskQueue;
            //cond vector
            std::map<std::shared_ptr<Condition>, bool> m_conds;
            
            std::string m_name;
            
            //Thread id
            std::vector<int> m_threadIds;
            
            Thread::ptr m_rootThread;
            size_t m_threadCount = 0;
            
            //task count
            std::atomic<size_t> m_taskCount = {0};
            //是否停止
            bool m_stopping = true;
            //主线程id
            int m_rootThreadId = 0;
            
            Condition m_cond;
        };
        
        
        class getThreadPool {
        public:
            typedef std::shared_ptr<getThreadPool> ptr;
            static ptr get() {
                return instance;
            }
    
            ThreadPool::ptr getPool(long long Id) {
                if (m_map.count(Id)) {
                    return m_map[Id];
                }
                return m_map[Id] = ThreadPool::ptr(new ThreadPool(10));
            }
        private:
            getThreadPool() {};
            getThreadPool(const getThreadPool&);
            getThreadPool& operator=(const getThreadPool&);
            
            
            static ptr instance;
            std::map<long long, ThreadPool::ptr> m_map;
        };
        getThreadPool::ptr getThreadPool::instance(new getThreadPool);
    }
}



#endif //MASTER_RPC_THREADPOOL_H
