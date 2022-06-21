namespace cpp rpc.master

struct BinLogRequest {
    1: string command,
    2: string key,
    3: string value,
}

struct BinLogResponse {
    1: string message,
}

struct GetRequest{
    1: string key,
}

struct GetResponse{
    1: string value,
    2: string message,
    3: i64 connection_id,
}

struct SetRequest{
    1: string key,
    2: string value,
    3: string func_call,
}

struct SetResponse{
    1: i64 connection_id,
    2: string message,
}

struct DelRequest{
    1: string key,
}

struct DelResponse{
    1: i64 connection_id,
    2: string message,
}

service Master{
    BinLogResponse  SendBinLog(1: BinLogRequest rsyncRequest),
    GetResponse     Get(1: GetRequest getRequest),
    SetResponse     Set(1: SetRequest setRequest),
    DelResponse     Del(1: DelRequest delRequest),
}