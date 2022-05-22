//
// Created by ByteDance on 2022/5/22.
//

#include "Mysql.h"
#include "../conf/conf.h"
using namespace ::rpc::conf::db;

namespace rpc {namespace db {
    Mysql_Base::Mysql_Base() {
        mysql_init(&con);
        if (!connect()) {
            std::cout << "connection success" << std::endl;
        } else {
            QML_ASSERT2(false, "mysql_real_connect error");
        }
    }
    
    int Mysql_Base::connect() {
        return !mysql_real_query(&con, MysqlConf::host, MysqlConf::user_name, MysqlConf::password, 0, nullptr, 0);
    }
    
    int Mysql_Base::query(std::string sql) {
        return mysql_query(&con, sql.c_str());
    }
    
    int Mysql_Base::rollback() {
        mysql_rollback(&con);
        return 0;
    }
    
    Mysql_Base::~Mysql_Base() {
        mysql_close(&con);
    }
    
    int MysqlClient::put(std::string key, std::string value) {
        std::string sql = "SELECT * FROM infoTable WHERE key=" + key;
        if (query(sql)) {
            std::cout << "exec sql=" + sql + " err" << std::endl;
            return 1;
        }
        mysql_res res(get_con());
        if (!res.get_next_row()) {
            std::cout << "key=" + key + " exists" << std::endl;
            return 1;
        }
        
        sql = "INSERT INFO infoTable(k, v) VALUES(" + key + "," + value + ")";
        
        if (query(sql)) {
            std::cout << "exec sql=" + sql + " err" << std::endl;
            return 1;
        }
        
        return 0;
    }
    
    std::string MysqlClient::get(std::string key) {
        std::string sql = "SELECT value FROM infoTable WHERE k =" + key;
        if (query(sql)) {
            std::cout << "exec sql=" + sql + " err" << std::endl;
            return "";
        }
        mysql_res res(get_con());
        if (res.get_next_row()) {
            std::cout << "key=" + key + " is not exists" << std::endl;
            return "";
        }
        
        return res.get_row()[0];
    }
    
    int MysqlClient::del(std::string key) {
        std::stirng sql = "SELECT * FROM user WHERE key=" + key;
        if (query(sql)) {
            std::cout << "exec sql=" + sql + " err" << std::endl;
            return 1;
        }
        mysql_res res(get_con());
        if (res.get_next_row()) {
            std::cout << "key=" + key + " is not exists" << std::endl;
            return 1;
        }
        sql = "DELETE FROM user WHERE key=" + key;
        if (query(sql)) {
            std::cout << "exec sql=" + sql + " err" << std::endl;
            return 1;
        }
        return 0;
    }
}}