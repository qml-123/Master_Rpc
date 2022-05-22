//
// Created by ByteDance on 2022/5/22.
//

#include "mysql_client.h"
#include "../conf/conf.h"
#include <mysql/mysql.h>
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
            return !mysql_real_connect(&con, MysqlConf::host.c_str(), MysqlConf::user_name.c_str(), MysqlConf::password.c_str(), MysqlConf::db_name.c_str(), 0, NULL, 0);
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
            std::cout << "put key=" << key << " value=" << value << std::endl;
            std::string sql = "SELECT * FROM infoTable WHERE k='" + key + "'";
            if (query(sql)) {
                std::cout << "exec sql=" + sql + " err" << std::endl;
                return 1;
            }
            mysql_res res(get_con());
            if (!res.get_next_row()) {
                std::cout << "key=" + key + " exists" << std::endl;
                return 1;
            }
            res.free();
            sql = "INSERT INTO infoTable(k, v) VALUES('" + key + "','" + value + "')";
            
            if (query(sql)) {
                std::cout << "exec sql=" + sql + " err" << std::endl;
                rollback();
                return 1;
            }
            
            return 0;
        }
        
        std::string MysqlClient::get(std::string key) {
            std::cout << "get key=" << key << std::endl;
            std::string sql = "SELECT v FROM infoTable WHERE k='" + key + "'";
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
            std::cout << "del key=" << key << std::endl;
            std::string sql = "SELECT * FROM infoTable WHERE k='" + key + "'";
            if (query(sql)) {
                std::cout << "exec sql=" + sql + " err" << std::endl;
                return 1;
            }
            mysql_res res(get_con());
            if (res.get_next_row()) {
                std::cout << "key=" + key + " is not exists" << std::endl;
                return 1;
            }
            sql = "DELETE FROM infoTable WHERE k='" + key + "'";
            if (query(sql)) {
                std::cout << "exec sql=" + sql + " err" << std::endl;
                rollback();
                return 1;
            }
            return 0;
        }
    }}