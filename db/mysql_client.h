//
// Created by ByteDance on 2022/5/22.
//

#ifndef MASTER_RPC_MYSQL_H
#define MASTER_RPC_MYSQL_H
#include "../conf/dbconf.h"
#include <mysql/mysql.h>

namespace rpc {namespace db{
        
        class mysql_res {
        public:
            mysql_res(MYSQL* con) {_res = mysql_store_result(con); }
            ~mysql_res() {}
            void free() {mysql_free_result(_res);}
            bool isNull() {return _res == nullptr; }
            
            int get_next_row() {
                if (isNull() || _res->row_count == 0)
                    return 1;
                row = mysql_fetch_row(_res);
                if (row == nullptr)
                    return 1;
                return 0;
            }
            
            MYSQL_ROW get_row() {return row;}
        
        private:
            MYSQL_RES* _res;
            MYSQL_ROW row;
        };
        
        class Mysql_Base {
        public:
            Mysql_Base();
            ~Mysql_Base();
            int begin();
            int rollback();
            int commit();
            int save_point(std::string savepoint);
            int delete_savepoint(std::string savepoint);
            int exec(std::string func);
        
        protected:
            int connect();  //1 fail  0 success
            int query(std::string sql);    //1 fail  0 success
            
            MYSQL* get_con() {return &con;}
        
        private:
            MYSQL con;
        };
        
        class MysqlClient : public Mysql_Base {
        public:
            int check_key(std::string key);           //1 就绪  0不就绪，无法操作
            int put(std::string key, std::string value);
            std::string get(std::string key);
            int del(std::string key);
            
        };
    }
}

#endif //MASTER_RPC_MYSQL_H
