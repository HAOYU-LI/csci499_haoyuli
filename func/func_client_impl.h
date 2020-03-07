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
  bool hook(int event_type, std::string function_name);
  
  // unhook method intends to remove the relationship between
  // event type and function name.
  bool unhook(int event_type);

  // event method is responsible for calling function with given event type 
  bool event(int event_type, ClientEventParams& event_params, 
  	          ClientEventReply& client_event_reply);

private:
  std::unique_ptr<FuncService::Stub> stub_;
  const static std::string REGISTER;
  const static std::string WARBLE;
  const static std::string FOLLOW;
  const static std::string READ;
  const static std::string PROFILE;

  bool doRegister(int event_type,
  	               ClientEventParams& client_event_params, ClientEventReply& client_event_reply);

  bool doWarble(int event_type,
  	             ClientEventParams& client_event_params, ClientEventReply& client_event_reply);

  bool doFollow(int event_type,
  	             ClientEventParams& client_event_params, ClientEventReply& client_event_reply);

  bool doRead(int event_type,
  	           ClientEventParams& client_event_params, ClientEventReply& client_event_reply);

  bool doProfile(int event_type,
  	              ClientEventParams& client_event_params, ClientEventReply& client_event_reply);
};

}// namespace func


#endif