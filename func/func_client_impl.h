#ifndef FUNC_CLIENT_IMPL_H
#define FUNC_CLIENT_IMPL_H
#include <string>
#include <vector>

#include <grpcpp/grpcpp.h>
#include <glog/logging.h>
#include <google/protobuf/any.pb.h>

#include "func_service.pb.h"
#include "func_service.grpc.pb.h"
#include "../warble/warble_service.pb.h"

#include "../warble/warble_service_impl.h"

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
using grpc::Channel;
using grpc::ClientContext;

namespace func {
struct ClientEventParams {
  std::string username;
  std::string to_follow;
  std::string warble_text;
  std::string warble_id;
  std::string parent_warble_id;
};

struct ClientEventReply {
  Warble warble;
  std::vector<Warble> warble_threads;
  std::vector<std::string> following;
  std::vector<std::string> followers;
};

/*
  Func service client implementation. An instance will be created by command-line tool
*/
class FuncClientImpl {
public:
  FuncClientImpl(std::shared_ptr<Channel> channel) 
    : stub_(FuncService::NewStub(channel)) {}
  
  // hook method is used to combine event type with specific handler function name.
  bool Hook(int event_type, std::string function_name);
  
  // unhook method intends to remove the relationship between
  // event type and function name.
  bool Unhook(int event_type);

  // event method is responsible for calling function with given event type 
  bool Event(int event_type, ClientEventParams& event_params, 
              ClientEventReply& client_event_reply);

private:
  std::unique_ptr<FuncService::Stub> stub_;
  const static std::string REGISTER { "registeruser" };
  const static std::string WARBLE { "warble" };
  const static std::string FOLLOW { "follow" };
  const static std::string READ { "read" };
  const static std::string PROFILE { "profile" };
  
  // Wrapper function for Register call in warble service.
  bool RegisterHelper(int event_type,
                   ClientEventParams& client_event_params, ClientEventReply& client_event_reply);
  // Wrapper function for Warble call in warble service.
  bool WarbleHelper(int event_type,
                 ClientEventParams& client_event_params, ClientEventReply& client_event_reply);
  // Wrapper function for Follow call in warble service.
  bool FollowHelper(int event_type,
                 ClientEventParams& client_event_params, ClientEventReply& client_event_reply);
  // Wrapper function for Read call in warble service.
  bool ReadHelper(int event_type,
               ClientEventParams& client_event_params, ClientEventReply& client_event_reply);
  // Wrapper function for Profile call in warble service.
  bool ProfileHelper(int event_type,
                  ClientEventParams& client_event_params, ClientEventReply& client_event_reply);
};
}// namespace func


#endif