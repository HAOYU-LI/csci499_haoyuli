#ifndef FUNC_SERVICE_IMPL_H
#define FUNC_SERVICE_IMPL_H
#include <any>
#include <functional>
#include <string>
#include <unordered_map>
#include <mutex>

#include <grpcpp/grpcpp.h>
#include <glog/logging.h>
#include <google/protobuf/any.pb.h>

#include "func_service.pb.h"
#include "func_service.grpc.pb.h"

#include "../warble/warble_service_impl.h"
#include "../key_value_store/key_value_client.h"

using func::HookRequest;
using func::HookReply;
using func::UnhookRequest;
using func::UnhookReply;
using func::EventRequest;
using func::EventReply;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReaderWriter;
using grpc::Status;
using grpc::StatusCode;
using warble::WarbleService;

namespace func {

struct RequestReplyWrapper {
  google::protobuf::Any request;
  google::protobuf::Any reply;
};

class FuncServiceImpl final : public FuncService::Service {
public:
  FuncServiceImpl();
  ~FuncServiceImpl() {
    delete kvclient;
  }
  // The hook method Intended to allow a service (not a user) to specify that 
  // a certain piece of code (a specific function) should be used 
  // to process messages of a specific type.  
  Status hook(ServerContext* context, 
               const HookRequest* request, HookReply* response) override;
  // The unhook method Intended to remove relationship between
  // a specific type and a certain piece of code.
  Status unhook(ServerContext* context, 
               const UnhookRequest* request, UnhookReply* response) override;
  // The event method is responsible for executing the given function 
  // that was hooked previously for that type of event and responding 
  // with the appropriate reply message  (if there exists a hooked function 
  // to process that type of event -- if not, throw an error).
  Status event(ServerContext* context, 
               const EventRequest* request, EventReply* response) override;
private:
  std::mutex func_mutex_;
  kvstore::KeyValueClient* kvclient;
  std::unordered_map<int, std::string> hook_map_;
  std::unordered_map<std::string, std::function<
                                       Status(RequestReplyWrapper,kvstore::KeyValueClient*)>> 
                                       name_method_map_;
};
}// End of func namespace

#endif