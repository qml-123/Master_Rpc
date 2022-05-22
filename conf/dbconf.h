//
// Created by ByteDance on 2022/5/22.
//

#ifndef MASTER_RPC_DBCONF_H
#define MASTER_RPC_DBCONF_H
#include <string>
namespace rpc{ namespace conf { namespace db {
    class MysqlConf {
    public:
        static std::string host;
        static std::string db_name;
        static std::string table_name;
        static std::string user_name;
        static std::string password;
    private:
        MysqlConf();
        MysqlConf(const MysqlConf&);
        MysqlConf& operator=(const MysqlConf&);
    };
    
    
}
}
}

#endif //MASTER_RPC_DBCONF_H
