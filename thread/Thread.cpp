//
// Created by ByteDance on 2022/5/24.
//

#include "thread.h"
#include "../conf/conf.h"

namespace rpc{namespace thread {
        static thread_local thread* t_thread = nullptr;
        static thread_local std::string t_thread_name = "UNKNOW";
        
        
        thread* thread::GetThis() {
            return t_thread;
        }
        
        const std::string& thread::GetName() {
            return t_thread_name;
        }
        
        void thread::SetName(const std::string& name) {
            if(name.empty()) {
                return;
            }
            if(t_thread) {
                t_thread->m_name = name;
            }
            t_thread_name = name;
        }
        
        thread::thread(std::function<void()> cb, const std::string& name)
                :m_cb(cb)
                ,m_name(name) {
            if(name.empty()) {
                m_name = "UNKNOW";
            }
            int rt = pthread_create(&m_thread, nullptr, &thread::run, this);
            if(rt) {
                throw std::logic_error("pthread_create error");
            }
            m_semaphore.wait();
        }
        
        thread::~thread() {
            if(m_thread) {
                pthread_detach(m_thread);
            }
        }
        
        void thread::join() {
            if(m_thread) {
                int rt = pthread_join(m_thread, nullptr);
                if(rt) {
                    throw std::logic_error("pthread_join error");
                }
                m_thread = 0;
            }
        }
        
        void* thread::run(void* arg) {
            thread* thread = (thread*)arg;
            t_thread = thread;
            t_thread_name = thread->m_name;
            thread->m_id = GetThreadId();
            pthread_setname_np(pthread_self(), thread->m_name.substr(0, 15).c_str());
            
            std::function<void()> cb;
            cb.swap(thread->m_cb);
            
            thread->m_semaphore.notify();
            
            cb();
            return 0;
        }
}}