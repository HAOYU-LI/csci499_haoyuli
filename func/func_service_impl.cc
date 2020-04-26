#include "func_service_impl.h"

namespace func {
// Wrapper method for registering user. Parse general Wrapper parameters
// to specific request and reply.
Status RegisterUser(RequestReplyWrapper& wrapper, KeyValueClient* kvclient) {
  RegisteruserRequest request;
  RegisteruserReply reply;
  wrapper.request.UnpackTo(&request);
  Status status = WarbleService::RegisterUser(&request, &reply, kvclient);
  wrapper.reply.PackFrom(reply);
  return status;
}

// Wrapper method for posting warble. Parse general Wrapper parameters
// to specific request and reply.
Status WarbleMethod(RequestReplyWrapper& wrapper, KeyValueClient* kvclient) {
  WarbleRequest request;
  WarbleReply reply;
  wrapper.request.UnpackTo(&request);
  Status status = WarbleService::WarbleMethod(&request, &reply, kvclient);
  wrapper.reply.PackFrom(reply);
  return status;
}

// Wrapper method for follow request. Parse general Wrapper parameters
// to specific request and reply.
Status Follow(RequestReplyWrapper& wrapper, KeyValueClient* kvclient) {
  FollowRequest request;
  FollowReply reply;
  wrapper.request.UnpackTo(&request);
  Status status = WarbleService::Follow(&request, &reply, kvclient);
  wrapper.reply.PackFrom(reply);
  return status;
}

// Wrapper method for read request. Parse general Wrapper parameters
// to specific request and reply.
Status Read(RequestReplyWrapper& wrapper, KeyValueClient* kvclient) {
  ReadRequest request;
  ReadReply reply;
  wrapper.request.UnpackTo(&request);
  Status status = WarbleService::Read(&request, &reply, kvclient);
  wrapper.reply.PackFrom(reply);
  return status;
}

// Wrapper method for querying user's profile. Parse general Wrapper parameters
// to specific request and reply.
Status Profile(RequestReplyWrapper& wrapper, KeyValueClient* kvclient) {
  ProfileRequest request;
  ProfileReply reply;
  wrapper.request.UnpackTo(&request);
  Status status = WarbleService::Profile(&request, &reply, kvclient);
  wrapper.reply.PackFrom(reply);
  return status;
}

// Wrapper method for beginning warble stream. Parse general Wrapper parameters
// to specific request and reply.
Status Stream(RequestReplyWrapper& wrapper, KeyValueClient* kvclient) {
  //TODO
  return status;
}

FuncServiceImpl::FuncServiceImpl() {

  kvclient = new KeyValueClient(grpc::CreateChannel("0.0.0.0:50001",
                           grpc::InsecureChannelCredentials()));
  name_method_map_["registeruser"] = std::function<
                                       Status(RequestReplyWrapper&,
                                              kvstore::KeyValueClient*)>(RegisterUser);
  name_method_map_["warble"] = std::function<
                                       Status(RequestReplyWrapper&,
                                              kvstore::KeyValueClient*)>(WarbleMethod);  
  name_method_map_["follow"] = std::function<
                                       Status(RequestReplyWrapper&,
                                              kvstore::KeyValueClient*)>(Follow);                                                                   
  name_method_map_["read"] = std::function<
                                       Status(RequestReplyWrapper&,
                                              kvstore::KeyValueClient*)>(Read);
  name_method_map_["profile"] = std::function<
                                       Status(RequestReplyWrapper&,
                                              kvstore::KeyValueClient*)>(Profile);
  name_method_map_["stream"] = std::function<
                                       Status(RequestReplyWrapper&,
                                              kvstore::KeyValueClient*)>(Stream);
}

// The hook method Intended to allow a service (not a user) to specify that 
// a certain piece of code (a specific function) should be used 
// to process messages of a specific type.  
Status FuncServiceImpl::hook(ServerContext* context, 
             const HookRequest* request, HookReply* response) {
  std::lock_guard<std::mutex> lock(func_mutex_);
  int event_type = request->event_type();
  std:string event_function = request->event_function();
  if (name_method_map_.find(event_function) == name_method_map_.end()) {
    LOG(INFO) << "[func_service] event_function doesn't match any existing function. " << 
                 "Possible choices are registeruser; warble; follow; read; profile"    << std::endl;
    return Status(StatusCode::NOT_FOUND, "event_function doesn't match any existing function.");
  }
  hook_map_[event_type] = event_function;
  return Status::OK;
}

// The unhook method Intended to remove relationship between
// a specific type and a certain piece of code.
Status FuncServiceImpl::unhook(ServerContext* context, 
             const UnhookRequest* request, UnhookReply* response) {
  std::lock_guard<std::mutex> lock(func_mutex_);
  int event_type = request->event_type();
  hook_map_.erase(event_type);
  return Status::OK;
}

// The event method is responsible for executing the given function 
// that was hooked previously for that type of event and responding 
// with the appropriate reply message  (if there exists a hooked function 
// to process that type of event -- if not, throw an error).
Status FuncServiceImpl::event(ServerContext* context, 
             const EventRequest* request, EventReply* response) {
  // To be done
  int event_type = request->event_type();
  if (hook_map_.find(event_type) == hook_map_.end()) {
    LOG(INFO) << "[func_service] event_type is not hooked with any method." << std::endl;
    return Status(StatusCode::NOT_FOUND, "event_type is not hooked with any method.");
  }
  std::string event_function = hook_map_[event_type];
  google::protobuf::Any function_request = request->payload();
  google::protobuf::Any function_reply;
  RequestReplyWrapper function_param_wrapper = {function_request, function_reply};
  Status status = name_method_map_[event_function](function_param_wrapper, kvclient);
  response->mutable_payload()->google::protobuf::Any::MergeFrom(function_reply);

  return status;
}

// Query corresponding event_function from func server for given event_type.
Status FuncServiceImpl::query(ServerContext* context,
               const QueryRequest* request, QueryReply* response) {
  int event_type = request->event_type();
  if (hook_map_.find(event_type) == hook_map_.end()) {
    LOG(INFO) << "[func_service] event_type is not hooked with any method." << std::endl;
    return Status(StatusCode::NOT_FOUND, "event_type is not hooked with any method.");
  }
  
  response->set_event_function(hook_map_[event_type]);
  return Status::OK;
}

}// End of func namespace

int main(int argc, char** argv) {
  google::InitGoogleLogging(argv[0]);
  string func_server_address("0.0.0.0:50000");
  ServerBuilder serverbuilder;
  func::FuncServiceImpl Func_server;
  serverbuilder.AddListeningPort(func_server_address, 
                            grpc::InsecureServerCredentials());
  serverbuilder.RegisterService(&Func_server);
  std::unique_ptr<Server> server(serverbuilder.BuildAndStart());
  std::cout<<"func server is listening on port : "<<func_server_address<<std::endl;
  server->Wait();

  return 0;
}