/**
 * Autogenerated by Thrift Compiler (0.11.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef Master_H
#define Master_H

#include <thrift/TDispatchProcessor.h>
#include <thrift/async/TConcurrentClientSyncInfo.h>
#include "master_types.h"

namespace rpc { namespace master {

#ifdef _MSC_VER
  #pragma warning( push )
  #pragma warning (disable : 4250 ) //inheriting methods via dominance 
#endif

class MasterIf {
 public:
  virtual ~MasterIf() {}
  virtual void Get(GetResponse& _return, const GetRequest& getRequest) = 0;
  virtual void Set(SetResponse& _return, const SetRequest& setRequest) = 0;
  virtual void Del(DelResponse& _return, const DelRequest& delRequest) = 0;
};

class MasterIfFactory {
 public:
  typedef MasterIf Handler;

  virtual ~MasterIfFactory() {}

  virtual MasterIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler(MasterIf* /* handler */) = 0;
};

class MasterIfSingletonFactory : virtual public MasterIfFactory {
 public:
  MasterIfSingletonFactory(const ::apache::thrift::stdcxx::shared_ptr<MasterIf>& iface) : iface_(iface) {}
  virtual ~MasterIfSingletonFactory() {}

  virtual MasterIf* getHandler(const ::apache::thrift::TConnectionInfo&) {
    return iface_.get();
  }
  virtual void releaseHandler(MasterIf* /* handler */) {}

 protected:
  ::apache::thrift::stdcxx::shared_ptr<MasterIf> iface_;
};

class MasterNull : virtual public MasterIf {
 public:
  virtual ~MasterNull() {}
  void Get(GetResponse& /* _return */, const GetRequest& /* getRequest */) {
    return;
  }
  void Set(SetResponse& /* _return */, const SetRequest& /* setRequest */) {
    return;
  }
  void Del(DelResponse& /* _return */, const DelRequest& /* delRequest */) {
    return;
  }
};

typedef struct _Master_Get_args__isset {
  _Master_Get_args__isset() : getRequest(false) {}
  bool getRequest :1;
} _Master_Get_args__isset;

class Master_Get_args {
 public:

  Master_Get_args(const Master_Get_args&);
  Master_Get_args& operator=(const Master_Get_args&);
  Master_Get_args() {
  }

  virtual ~Master_Get_args() throw();
  GetRequest getRequest;

  _Master_Get_args__isset __isset;

  void __set_getRequest(const GetRequest& val);

  bool operator == (const Master_Get_args & rhs) const
  {
    if (!(getRequest == rhs.getRequest))
      return false;
    return true;
  }
  bool operator != (const Master_Get_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Master_Get_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class Master_Get_pargs {
 public:


  virtual ~Master_Get_pargs() throw();
  const GetRequest* getRequest;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _Master_Get_result__isset {
  _Master_Get_result__isset() : success(false) {}
  bool success :1;
} _Master_Get_result__isset;

class Master_Get_result {
 public:

  Master_Get_result(const Master_Get_result&);
  Master_Get_result& operator=(const Master_Get_result&);
  Master_Get_result() {
  }

  virtual ~Master_Get_result() throw();
  GetResponse success;

  _Master_Get_result__isset __isset;

  void __set_success(const GetResponse& val);

  bool operator == (const Master_Get_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const Master_Get_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Master_Get_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _Master_Get_presult__isset {
  _Master_Get_presult__isset() : success(false) {}
  bool success :1;
} _Master_Get_presult__isset;

class Master_Get_presult {
 public:


  virtual ~Master_Get_presult() throw();
  GetResponse* success;

  _Master_Get_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _Master_Set_args__isset {
  _Master_Set_args__isset() : setRequest(false) {}
  bool setRequest :1;
} _Master_Set_args__isset;

class Master_Set_args {
 public:

  Master_Set_args(const Master_Set_args&);
  Master_Set_args& operator=(const Master_Set_args&);
  Master_Set_args() {
  }

  virtual ~Master_Set_args() throw();
  SetRequest setRequest;

  _Master_Set_args__isset __isset;

  void __set_setRequest(const SetRequest& val);

  bool operator == (const Master_Set_args & rhs) const
  {
    if (!(setRequest == rhs.setRequest))
      return false;
    return true;
  }
  bool operator != (const Master_Set_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Master_Set_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class Master_Set_pargs {
 public:


  virtual ~Master_Set_pargs() throw();
  const SetRequest* setRequest;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _Master_Set_result__isset {
  _Master_Set_result__isset() : success(false) {}
  bool success :1;
} _Master_Set_result__isset;

class Master_Set_result {
 public:

  Master_Set_result(const Master_Set_result&);
  Master_Set_result& operator=(const Master_Set_result&);
  Master_Set_result() {
  }

  virtual ~Master_Set_result() throw();
  SetResponse success;

  _Master_Set_result__isset __isset;

  void __set_success(const SetResponse& val);

  bool operator == (const Master_Set_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const Master_Set_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Master_Set_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _Master_Set_presult__isset {
  _Master_Set_presult__isset() : success(false) {}
  bool success :1;
} _Master_Set_presult__isset;

class Master_Set_presult {
 public:


  virtual ~Master_Set_presult() throw();
  SetResponse* success;

  _Master_Set_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _Master_Del_args__isset {
  _Master_Del_args__isset() : delRequest(false) {}
  bool delRequest :1;
} _Master_Del_args__isset;

class Master_Del_args {
 public:

  Master_Del_args(const Master_Del_args&);
  Master_Del_args& operator=(const Master_Del_args&);
  Master_Del_args() {
  }

  virtual ~Master_Del_args() throw();
  DelRequest delRequest;

  _Master_Del_args__isset __isset;

  void __set_delRequest(const DelRequest& val);

  bool operator == (const Master_Del_args & rhs) const
  {
    if (!(delRequest == rhs.delRequest))
      return false;
    return true;
  }
  bool operator != (const Master_Del_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Master_Del_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class Master_Del_pargs {
 public:


  virtual ~Master_Del_pargs() throw();
  const DelRequest* delRequest;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _Master_Del_result__isset {
  _Master_Del_result__isset() : success(false) {}
  bool success :1;
} _Master_Del_result__isset;

class Master_Del_result {
 public:

  Master_Del_result(const Master_Del_result&);
  Master_Del_result& operator=(const Master_Del_result&);
  Master_Del_result() {
  }

  virtual ~Master_Del_result() throw();
  DelResponse success;

  _Master_Del_result__isset __isset;

  void __set_success(const DelResponse& val);

  bool operator == (const Master_Del_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const Master_Del_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Master_Del_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _Master_Del_presult__isset {
  _Master_Del_presult__isset() : success(false) {}
  bool success :1;
} _Master_Del_presult__isset;

class Master_Del_presult {
 public:


  virtual ~Master_Del_presult() throw();
  DelResponse* success;

  _Master_Del_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

class MasterClient : virtual public MasterIf {
 public:
  MasterClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  MasterClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    setProtocol(iprot,oprot);
  }
 private:
  void setProtocol(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
  setProtocol(prot,prot);
  }
  void setProtocol(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    piprot_=iprot;
    poprot_=oprot;
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
 public:
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  void Get(GetResponse& _return, const GetRequest& getRequest);
  void send_Get(const GetRequest& getRequest);
  void recv_Get(GetResponse& _return);
  void Set(SetResponse& _return, const SetRequest& setRequest);
  void send_Set(const SetRequest& setRequest);
  void recv_Set(SetResponse& _return);
  void Del(DelResponse& _return, const DelRequest& delRequest);
  void send_Del(const DelRequest& delRequest);
  void recv_Del(DelResponse& _return);
 protected:
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
};

class MasterProcessor : public ::apache::thrift::TDispatchProcessor {
 protected:
  ::apache::thrift::stdcxx::shared_ptr<MasterIf> iface_;
  virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext);
 private:
  typedef  void (MasterProcessor::*ProcessFunction)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*, void*);
  typedef std::map<std::string, ProcessFunction> ProcessMap;
  ProcessMap processMap_;
  void process_Get(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_Set(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_Del(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
 public:
  MasterProcessor(::apache::thrift::stdcxx::shared_ptr<MasterIf> iface) :
    iface_(iface) {
    processMap_["Get"] = &MasterProcessor::process_Get;
    processMap_["Set"] = &MasterProcessor::process_Set;
    processMap_["Del"] = &MasterProcessor::process_Del;
  }

  virtual ~MasterProcessor() {}
};

class MasterProcessorFactory : public ::apache::thrift::TProcessorFactory {
 public:
  MasterProcessorFactory(const ::apache::thrift::stdcxx::shared_ptr< MasterIfFactory >& handlerFactory) :
      handlerFactory_(handlerFactory) {}

  ::apache::thrift::stdcxx::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo);

 protected:
  ::apache::thrift::stdcxx::shared_ptr< MasterIfFactory > handlerFactory_;
};

class MasterMultiface : virtual public MasterIf {
 public:
  MasterMultiface(std::vector<apache::thrift::stdcxx::shared_ptr<MasterIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~MasterMultiface() {}
 protected:
  std::vector<apache::thrift::stdcxx::shared_ptr<MasterIf> > ifaces_;
  MasterMultiface() {}
  void add(::apache::thrift::stdcxx::shared_ptr<MasterIf> iface) {
    ifaces_.push_back(iface);
  }
 public:
  void Get(GetResponse& _return, const GetRequest& getRequest) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->Get(_return, getRequest);
    }
    ifaces_[i]->Get(_return, getRequest);
    return;
  }

  void Set(SetResponse& _return, const SetRequest& setRequest) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->Set(_return, setRequest);
    }
    ifaces_[i]->Set(_return, setRequest);
    return;
  }

  void Del(DelResponse& _return, const DelRequest& delRequest) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->Del(_return, delRequest);
    }
    ifaces_[i]->Del(_return, delRequest);
    return;
  }

};

// The 'concurrent' client is a thread safe client that correctly handles
// out of order responses.  It is slower than the regular client, so should
// only be used when you need to share a connection among multiple threads
class MasterConcurrentClient : virtual public MasterIf {
 public:
  MasterConcurrentClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  MasterConcurrentClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    setProtocol(iprot,oprot);
  }
 private:
  void setProtocol(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
  setProtocol(prot,prot);
  }
  void setProtocol(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    piprot_=iprot;
    poprot_=oprot;
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
 public:
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  void Get(GetResponse& _return, const GetRequest& getRequest);
  int32_t send_Get(const GetRequest& getRequest);
  void recv_Get(GetResponse& _return, const int32_t seqid);
  void Set(SetResponse& _return, const SetRequest& setRequest);
  int32_t send_Set(const SetRequest& setRequest);
  void recv_Set(SetResponse& _return, const int32_t seqid);
  void Del(DelResponse& _return, const DelRequest& delRequest);
  int32_t send_Del(const DelRequest& delRequest);
  void recv_Del(DelResponse& _return, const int32_t seqid);
 protected:
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
  ::apache::thrift::async::TConcurrentClientSyncInfo sync_;
};

#ifdef _MSC_VER
  #pragma warning( pop )
#endif

}} // namespace

#endif
