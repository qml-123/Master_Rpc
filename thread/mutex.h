//
// Created by ByteDance on 2022/5/24.
//

#ifndef MASTER_RPC_MUTEX_H
#define MASTER_RPC_MUTEX_H

#include <thread>
#include <functional>
#include <memory>
#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include <atomic>
#include <list>
#include <stdint-gcc.h>

#include "noncopyable.h"

namespace rpc{namespace thread{
        /**
     * @brief 信号量
     */
        class Semaphore : Noncopyable {
        public:
            /**
             * @brief 构造函数
             * @param[in] count 信号量值的大小
             */
            Semaphore(uint32_t count = 0);
            
            /**
             * @brief 析构函数
             */
            ~Semaphore();
            
            /**
             * @brief 获取信号量
             */
            void wait();
            
            /**
             * @brief 释放信号量
             */
            void notify();
        private:
            sem_t m_semaphore;
        };
        
/**
 * @brief 局部锁的模板实现
 */
        template<class T>
        struct ScopedLockImpl {
        public:
            /**
             * @brief 构造函数
             * @param[in] mutex Mutex
             */
            ScopedLockImpl(T& mutex)
                    :m_mutex(mutex) {
                m_mutex.lock();
                m_locked = true;
            }
            
            /**
             * @brief 析构函数,自动释放锁
             */
            ~ScopedLockImpl() {
                unlock();
            }
            
            /**
             * @brief 加锁
             */
            void lock() {
                if(!m_locked) {
                    m_mutex.lock();
                    m_locked = true;
                }
            }
            
            /**
             * @brief 解锁
             */
            void unlock() {
                if(m_locked) {
                    m_mutex.unlock();
                    m_locked = false;
                }
            }
        private:
            /// mutex
            T& m_mutex;
            /// 是否已上锁
            bool m_locked;
        };

/**
 * @brief 局部读锁模板实现
 */
        template<class T>
        struct ReadScopedLockImpl {
        public:
            /**
             * @brief 构造函数
             * @param[in] mutex 读写锁
             */
            ReadScopedLockImpl(T& mutex)
                    :m_mutex(mutex) {
                m_mutex.rdlock();
                m_locked = true;
            }
            
            /**
             * @brief 析构函数,自动释放锁
             */
            ~ReadScopedLockImpl() {
                unlock();
            }
            
            /**
             * @brief 上读锁
             */
            void lock() {
                if(!m_locked) {
                    m_mutex.rdlock();
                    m_locked = true;
                }
            }
            
            /**
             * @brief 释放锁
             */
            void unlock() {
                if(m_locked) {
                    m_mutex.unlock();
                    m_locked = false;
                }
            }
        private:
            /// mutex
            T& m_mutex;
            /// 是否已上锁
            bool m_locked;
        };

/**
 * @brief 局部写锁模板实现
 */
        template<class T>
        struct WriteScopedLockImpl {
        public:
            /**
             * @brief 构造函数
             * @param[in] mutex 读写锁
             */
            WriteScopedLockImpl(T& mutex)
                    :m_mutex(mutex) {
                m_mutex.wrlock();
                m_locked = true;
            }
            
            /**
             * @brief 析构函数
             */
            ~WriteScopedLockImpl() {
                unlock();
            }
            
            /**
             * @brief 上写锁
             */
            void lock() {
                if(!m_locked) {
                    m_mutex.wrlock();
                    m_locked = true;
                }
            }
            
            /**
             * @brief 解锁
             */
            void unlock() {
                if(m_locked) {
                    m_mutex.unlock();
                    m_locked = false;
                }
            }
        private:
            /// Mutex
            T& m_mutex;
            /// 是否已上锁
            bool m_locked;
        };

/**
 * @brief 互斥量
 */
        class Mutex : Noncopyable {
        public:
            /// 局部锁
            typedef ScopedLockImpl<Mutex> Lock;
            
            /**
             * @brief 构造函数
             */
            Mutex() {
                pthread_mutex_init(&m_mutex, nullptr);
            }
            
            /**
             * @brief 析构函数
             */
            ~Mutex() {
                pthread_mutex_destroy(&m_mutex);
            }
            
            /**
             * @brief 加锁
             */
            void lock() {
                pthread_mutex_lock(&m_mutex);
            }
            
            /**
             * @brief 解锁
             */
            void unlock() {
                pthread_mutex_unlock(&m_mutex);
            }
        private:
            /// mutex
            pthread_mutex_t m_mutex;
        };
        
        template<class T>
        struct ScopedCondImpl {
        public:
            /**
             * @brief 构造函数
             * @param[in] mutex Mutex
             */
            ScopedCondImpl(T& cond)
                    :m_cond(cond) {
                m_cond.lock();
                m_locked = true;
            }
            
            /**
             * @brief 析构函数,自动释放锁
             */
            ~ScopedCondImpl() {
                unlock();
            }
            
            /**
             * @brief 加锁
             */
            void lock() {
                if(!m_locked) {
                    m_cond.lock();
                    m_locked = true;
                }
            }
            
            /**
             * @brief 解锁
             */
            void unlock() {
                if(m_locked) {
                    m_cond.unlock();
                    m_locked = false;
                }
            }
            
            void wait() {
                m_locked = false;
                m_cond.wait();
            }
            
            void notify() {
                m_cond.notify();
            }
            
            void notify_all() {
                m_cond.notify_all();
            }
            
        private:
            ///
            T& m_cond;
            /// 是否已上锁
            bool m_locked;
        };
        
        
        /**
 * @brief 条件变量
 */
        class Condition: Noncopyable {
        public:
            typedef ScopedCondImpl<Condition> Cond;
            
            Condition() {
                pthread_cond_init(&(m_cond), nullptr);
                pthread_mutex_init(&m_mutex, nullptr);
            }
            
            ~Condition() {
                pthread_cond_destroy(&m_cond);
                pthread_mutex_destroy(&m_mutex);
            }
            void notify() {
                pthread_cond_signal(&m_cond);
            }
            
            void wait() {
                pthread_cond_wait(&m_cond, &m_mutex);
            }
            
            void notify_all() {
                pthread_cond_broadcast(&m_cond);
            }
            
            void lock() {
                pthread_mutex_lock(&m_mutex);
            }
            
            void unlock() {
                pthread_mutex_unlock(&m_mutex);
            }
            
        private:
            pthread_cond_t m_cond;
            pthread_mutex_t m_mutex;
        };

/**
 * @brief 读写互斥量
 */
        class RWMutex : Noncopyable{
        public:
            
            /// 局部读锁
            typedef ReadScopedLockImpl<RWMutex> ReadLock;
            
            /// 局部写锁
            typedef WriteScopedLockImpl<RWMutex> WriteLock;
            
            /**
             * @brief 构造函数
             */
            RWMutex() {
                pthread_rwlock_init(&m_lock, nullptr);
            }
            
            /**
             * @brief 析构函数
             */
            ~RWMutex() {
                pthread_rwlock_destroy(&m_lock);
            }
            
            /**
             * @brief 上读锁
             */
            void rdlock() {
                pthread_rwlock_rdlock(&m_lock);
            }
            
            /**
             * @brief 上写锁
             */
            void wrlock() {
                pthread_rwlock_wrlock(&m_lock);
            }
            
            /**
             * @brief 解锁
             */
            void unlock() {
                pthread_rwlock_unlock(&m_lock);
            }
        private:
            /// 读写锁
            pthread_rwlock_t m_lock;
        };
        
    }}


#endif //MASTER_RPC_MUTEX_H
