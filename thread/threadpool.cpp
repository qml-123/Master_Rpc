//
// Created by ByteDance on 2022/5/24.
//

#include "threadpool.h"
#include "conf/conf.h"
#include "thread.h"
#include "log/elog.h"
#include <utility>

namespace rpc {namespace thread{
        
        static thread_local ThreadPool* t_threadPool = nullptr;
//    static thread_local Thread* t_thread = nullptr;
        
        ThreadPool::ThreadPool(size_t threads, const std::string& name)
                :m_name(name) {
            QML_ASSERT(threads > 0);
            
            QML_ASSERT(GetThis() == nullptr);
            t_threadPool = this;
            
            m_threadCount = threads;
            m_idleThreadCount = threads;
            
            start();
        }
        
        ThreadPool::~ThreadPool() {
            QML_ASSERT(m_stopping);
            if (GetThis() == this) {
                t_threadPool = nullptr;
            }
        }
        
        ThreadPool* ThreadPool::GetThis() {
            return t_threadPool;
        }
        
        void ThreadPool::start() {
            Condition::Cond cond(m_cond);
            if (!m_stopping) {
                return;
            }
            m_stopping = false;
            QML_ASSERT(m_threads.empty());
            
            m_threads.resize(m_threadCount);
            for (size_t i = 0; i < m_threadCount; i++) {
                m_threads[i].reset(new Thread(std::bind(&ThreadPool::run, this), m_name + "_" + std::to_string(i + 1)));
                m_threadIds.push_back(m_threads[i]->getId());
            }
        }
        
        void ThreadPool::stop() {
            if (stopping()) {
                return;
            }
            m_stopping = true;
            m_cond.notify_all();
            sleep(5);
            for (size_t i = 0; i < m_threadCount; ++i) {
                m_threads[i]->join();
            }
        }
        
        void ThreadPool::setThis() {
            t_threadPool = this;
        }
        
        void ThreadPool::run() {
            while (true) {
                Condition::Cond cond(m_cond);
                while (!stopping() && !hasTaskCount()) {
//                    print("waiting");
                    cond.wait();
                }

//                print("work");
                if (stopping()) {
//                    print("exit");
                    break;
                }
    
                cond.lock();
                ThreadTask::ptr task = m_taskQueue.front();
                m_taskQueue.pop();
                cond.unlock();
                --m_taskCount;
                --m_idleThreadCount;
                if (task->call_name == "get") {
                    task->get_cb(task->getRequest, std::move(task->client), ref(task->getObj));
                } else if (task->call_name == "set") {
                    task->set_cb(task->setRequest, ref(task->client), ref(task->setObj));
                } else if (task->call_name == "del") {
                    task->del_cb(task->delRequest, ref(task->client), ref(task->delObj));
                } else if (task->call_name == "try") {
                    task->try_cb(task->tryRequest, ref(task->client), ref(task->tryObj));
                } else if (task->call_name == "finish") {
                    task->finish_cb(task->finishRequest, ref(task->client), ref(task->finishObj));
                } else if (task->call_name == "_finish") {
                    task->_finish_cb(task->fv, task->call_func, task->clients);
                } else if (task->call_name == "sendlog") {
                    task->senlog_cb(task->binLogRequest, ref(task->m_client));
                } else {
                    log_e("thread call err");
                }
                ++m_idleThreadCount;
            }
        }
        
        void ThreadPool::addTask(ThreadTask::ptr task) {
            Condition::Cond cond(m_cond);
            m_taskQueue.push(task);
            ++m_taskCount;
            cond.notify();
        }
    }}