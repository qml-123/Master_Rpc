//
// Created by ByteDance on 2022/6/17.
//

#ifndef MASTER_RPC_BIN_LOG_H
#define MASTER_RPC_BIN_LOG_H
#include <iostream>
#include <memory>
#include <list>
#include <boost/archive/text_oarchive.hpp>
#include "gen-cpp/slave_types.h"

namespace rpc{namespace db {
        class List {
        public:
            void push_back(slave::Sql sql) {
                m_sql.push_back(sql);
            }
            
        private:
            friend class boost::serialization::access;
    
            template<class Archive>
            void serialize(Archive& ar, const unsigned int version)
            {
                // serialize base class information
                ar & m_sql;
            }
            
        private:
            std::list<slave::Sql> m_sql;
        };
        class BinLog{
        public:
            typedef std::shared_ptr<BinLog> ptr;
            static ptr get() {
                return instance;
            }
            static void write(std::string command, std::string key, std::string value = "") {
                slave::Sql sql;
                sql.command = command;
                sql.key = key;
                if (sql.command == "set") {
                    sql.value = value;
                }
                m_sql.push_back(sql);
                
                //保存
//                std::ostringstream os;
//                boost::archive::binary_oarchive oa(os);
//                oa << d1
                
                // 向其他master 同步操作，
            }
            static void read() {
            
            }
            
        private:
            BinLog();
            BinLog(const BinLog&);
            BinLog(const BinLog&&);
            BinLog& operator=(const BinLog&);
            
            class CG {
            public:
                ~CG() {
                    BinLog::instance = nullptr;
                }
            };
            
            static ptr instance;
            static List m_sql;
        };
        BinLog::ptr BinLog::instance(new BinLog);
    }}
#endif //MASTER_RPC_BIN_LOG_H
