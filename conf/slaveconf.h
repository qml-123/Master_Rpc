//
// Created by ByteDance on 2022/6/14.
//

#ifndef MASTER_RPC_SLAVECONF_H
#define MASTER_RPC_SLAVECONF_H

#include <string>
#include <vector>

namespace rpc{namespace conf{
        class SlaveConf {
        public:
            static const std::vector<std::pair<std::string, int>> slave_address;
        };
        const std::vector<std::pair<std::string, int>> SlaveConf::slave_address{
                {"82.156.171.212", 9091},
                {"182.92.68.121", 9091}
            };
        
        //, {"182.92.68.121", 9091}
    }}


#endif //MASTER_RPC_SLAVECONF_H
