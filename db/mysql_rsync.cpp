//
// Created by ByteDance on 2022/5/23.
//

#include "mysql_rsync.h"
#include "conf/dbconf.h"
using namespace ::rpc::conf::db;
namespace rpc {namespace db {
    std::string mysql_rsync::get_sql() {
        std::string file_name = MysqlConf::db_name + "-" + MysqlConf::user_name + ".sql";
        std::string command = "mysqldump -u" + MysqlConf::user_name + " -p" + MysqlConf::password + " " + MysqlConf::db_name + " > temp/" + file_name;
        system(command.c_str());
        
        std::ifstream infile;
        infile.open("temp/" + file_name);
        
        char c;
        std::string sql_command;
        while ((c = infile.get()) != EOF)
        {
            sql_command.push_back(c);
        }
        infile.close();
        command = "rm temp/" + file_name;
//        system(command.c_str());
        return sql_command;
    }
    
    void mysql_rsync::set_sql(std::string sql_command, std::string file_name) {
        file_name = "temp/" + file_name;
        std::ofstream outfile;
        outfile.open(file_name);
        outfile << sql_command;
        outfile.close();
        std::string command = "mysql -u" + MysqlConf::user_name + " -p" + MysqlConf::password + " " + MysqlConf::db_name + " < " + file_name;
        system(command.c_str());
    }
}}