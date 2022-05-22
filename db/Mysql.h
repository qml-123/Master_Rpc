//
// Created by ByteDance on 2022/5/22.
//

#ifndef MASTER_RPC_MYSQL_H
#define MASTER_RPC_MYSQL_H
#include "../conf/dbconf.h"
#include <mysql/mysql.h>
namespace rpc {namespace db{
    class Mysql_Base {
    public:
        Mysql_Base();
        ~Mysql_Base();
        int connect();  //1 fail  0 success
        int query(std::string sql);    //1 fail  0 success
        int rollback();
        Mysql get_con() {return con;}
        
        class mysql_res {
        public:
            mysql_res(Mysql &con) {res = mysql_store_result(&con); }
            ~mysql_res() {mysql_free_result(res);}
            
            bool isNull() {return res == nullptr; }
            
            int get_next_row() {
                if (isNull() || res->row_count == 0)
                    return 1;
                row = mysql_fetch_row(res);
                if (row == nullptr)
                    return 1;
                return 0;
            }
    
            MYSQL_ROW get_row() {return row;}
            
        private:
            MYSQL_RES* res;
            MYSQL_ROW row;
        };
        
    private:
        Mysql con;
    };
    
    class MysqlClient : public Mysql_Base {
    public:
        int put(std::string key, std::string value);
        std::string get(std::string key);
        int del(std::string key);

    };
}
}

#endif //MASTER_RPC_MYSQL_H
