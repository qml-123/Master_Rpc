//
// Created by ByteDance on 2022/5/24.
//

#ifndef MASTER_RPC_THREADPOOL_H
#define MASTER_RPC_THREADPOOL_H

#include "noncopyable.h"
#include "mutex.h"
#include "thread.h"
#include "../conf/conf.h"
#include <functional>
#include <vector>
#include <iostream>
#include <atomic>
#include <queue>
#include <condition_variable>

namespace rpc{namespace thread{
        class ThreadPool : Noncopyable{
        public:
            struct ThreadTask {
                //执行函数
                std::function<void(void*, const void*)> cb;
                //Thread id
                int thread;
                const void* request;
                void* response;
    
                ThreadTask(std::function<void(void*, const void*)> f, int thr)
                        : cb(f), thread(thr), request(nullptr), response(nullptr) {
        
                }
                
                ThreadTask(std::function<void(void*, const void*)> f, int thr, const void* _request, void* _response)
                        : cb(f), thread(thr), request(_request), response(_response) {
                    
                }
                
                ThreadTask()
                        : thread(-1), request(nullptr), response(nullptr) {
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
            
            void addTask(ThreadTask threadTask);
            
            bool hasIdleThreads() {return m_idleThreadCount > 0;}
            
            bool hasTaskCount() {return m_taskCount > 0; }
            
        protected:
            
            void run();
            
            //是否可以停止
            bool stopping() {return m_stopping; }
            
            
            void setThis();
            
            ThreadTask getTask() {
                ThreadTask ret = m_taskQueue.front();
                m_taskQueue.pop();
                m_taskCount--;
                return ret;
            }
            //idle Thread count 空闲
            std::atomic<size_t> m_idleThreadCount = {0};
        
        private:
            //线程池
            std::vector<Thread::ptr> m_threads;
            //task queue
            std::queue<ThreadTask> m_taskQueue;
            
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
    }
}



#endif //MASTER_RPC_THREADPOOL_H
