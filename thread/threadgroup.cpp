//
// Created by ByteDance on 2022/5/25.
//

#include "threadgroup.h"
#include "../conf/conf.h"
#include <sys/epoll.h>
#include <fcntl.h>
#include <unistd.h>
#include <stddef.h>
#include <string>

namespace rpc {namespace thread{
        ThreadGroup::ThreadGroup(size_t threads, const std::string& name)
                : ThreadPool(threads, name){
            m_epfd = epoll_create(5000);
            QML_ASSERT(m_epfd > 0);
            
            int rt = pipe(m_tickleFds);
            QML_ASSERT(!rt);
            
            epoll_event event;
            memset(&event, 0, sizeof(epoll_event));
            event.events = EPOLLIN | EPOLLET;
            event.data.fd = m_tickleFds[0];
            
            rt = fcntl(m_tickleFds[0], F_SETFL, O_NONBLOCK);
            QML_ASSERT(!rt);
            
            rt = epoll_ctl(m_epfd, EPOLL_CTL_ADD, m_tickleFds[0], &event);
            QML_ASSERT(!rt);
            
            start();
        }
        
        ThreadGroup::~ThreadGroup() {
            stop();
            close(m_epfd);
            close(m_tickleFds[0]);
            close(m_tickleFds[1]);
        }
        
        void ThreadGroup::tickle() {
            if (!hasIdleThreads()) {
                return;
            }
            print("tickle");
            int rt = write(m_tickleFds[1], "T", 1);
            QML_ASSERT(rt == 1);
        }
        
        void ThreadGroup::run() {
            epoll_event* events = new epoll_event[64]();
            std::shared_ptr<epoll_event> shared_events(events, [&](epoll_event* ptr){
                delete[] ptr;
            });
            
            while (true) {
                if (stopping()) {
//                    print("stopping exit");
                    break;
                }
                
                int rt = 0;
                do {
//                    print("exec epoll_wait");
                    rt = epoll_wait(m_epfd, events, 64, -1);
                    if (rt < 0 && errno == EINTR) {
                        continue;
                    } else {
                        break;
                    }
                }while (true);
                
                if (!hasTaskCount())
                    continue;
                
                MutexType::Lock lock(m_mutex);
                for (int i = 0; i < rt; i++) {
                    if (events[i].data.fd == m_tickleFds[0]) {
                        char c;
                        read(m_tickleFds[0], &c,1);
                    }
                }
                if (!hasTaskCount())
                    continue;
                ThreadTask threadTask = getTask();
                lock.unlock();
                --m_idleThreadCount;
                threadTask.cb(threadTask.response, threadTask.request);
                ++m_idleThreadCount;
            }
        }
        
    }}