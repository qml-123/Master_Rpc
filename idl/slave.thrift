include "master.thrift"
namespace cpp rpc.slave

struct RsyncRequest {
    1: string database,
    2: string sql_file,
    3: string message,
}

struct RsyncResponse {
    1: string message,
}

struct TryRequest {
    1: string call_func,
    2: string key,
}

struct TryResponse {
    1: string message
}


service Slave{
    RsyncResponse Rsync(1: RsyncRequest rsyncRequest),
    TryResponse   Try(1: TryRequest tryRequest),
    master.GetResponse   Get(1: master.GetRequest getRequest),
    master.SetResponse   Set(1: master.SetRequest setRequest),
    master.DelResponse   Del(1: master.DelRequest delRequest),
}
