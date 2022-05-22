//
// Created by ByteDance on 2022/5/23.
//

#ifndef SLAVE_RPC_MYSQL_RSYNC_H
#define SLAVE_RPC_MYSQL_RSYNC_H
#include <iostream>
#include <fstream>
namespace rpc {namespace db {
    class mysql_rsync {
    public:
        static std::string get_sql();
        static void set_sql(std::string sql_command, std::string file_name);
    private:
    
    };
}}


#endif //SLAVE_RPC_MYSQL_RSYNC_H
