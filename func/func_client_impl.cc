#include "func_client_impl.h"

#include <iostream>

namespace func {
	 const std::string FuncClientImpl::REGISTER = "registeruser";
  const std::string FuncClientImpl::WARBLE = "warble";
  const std::string FuncClientImpl::FOLLOW = "follow";
  const std::string FuncClientImpl::READ = "read";
  const std::string FuncClientImpl::PROFILE = "profile";
	 // hook method is used to combine event type with specific handler function name.
  bool FuncClientImpl::hook(int event_type, std::string function_name) {
    HookRequest request;
    HookReply reply;
    ClientContext context;
    request.set_event_type(event_type);
    request.set_event_function(function_name);

    Status status = stub_->hook(&context, request, &reply);
    return status.ok();
  }

  // unhook method intends to remove the relationship between
  // event type and function name.
  bool FuncClientImpl::unhook(int event_type) {
    UnhookRequest request;
    UnhookReply reply;
    ClientContext context;
    request.set_event_type(event_type);
    
    Status status = stub_->unhook(&context, request, &reply);
    return status.ok();
  }

  // event method is responsible for calling function with given event type 
  bool FuncClientImpl::event(int event_type, ClientEventParams& client_event_params,
  	                          ClientEventReply& client_event_reply) {
    QueryRequest query_request;
    QueryReply query_reply;
    ClientContext context;
    query_request.set_event_type(event_type);
    Status query_status = stub_->query(&context, query_request, &query_reply);
    if (!query_status.ok()) {
      LOG(INFO) << "[func_client] event_type doesn't match any existing function. " << std::endl;
      return false;
    }
    
    std::string function_name = query_reply.event_function();

    if (function_name.compare(FuncClientImpl::REGISTER) == 0) {
      return doRegister(event_type, client_event_params, client_event_reply);
    }
    if (function_name.compare(FuncClientImpl::WARBLE) == 0) {
      return doWarble(event_type, client_event_params, client_event_reply);
    }
    if (function_name.compare(FuncClientImpl::FOLLOW) == 0) {
      return doFollow(event_type, client_event_params, client_event_reply);
    }
    if (function_name.compare(FuncClientImpl::READ) == 0) {
      return doRead(event_type, client_event_params, client_event_reply);
    }
    if (function_name.compare(FuncClientImpl::PROFILE) == 0) {
      return doProfile(event_type, client_event_params, client_event_reply);
    }

    return false;
  }

  bool FuncClientImpl::doRegister(int event_type,
  	                 ClientEventParams& client_event_params, ClientEventReply& client_event_reply) {
    EventRequest event_request;
    EventReply event_reply;
    ClientContext event_context;
    google::protobuf::Any any_request;
    google::protobuf::Any any_reply;
    event_request.set_event_type(event_type);

    RegisteruserRequest register_request;
    RegisteruserReply register_reply;
    register_request.set_username(client_event_params.username);
    any_request.PackFrom(register_request);
    event_request.mutable_payload()->google::protobuf::Any::MergeFrom(any_request);
    event_reply.mutable_payload()->google::protobuf::Any::MergeFrom(any_reply);
    Status status = stub_->event(&event_context, event_request, &event_reply);
    event_reply.payload().UnpackTo(&register_reply);

    return status.ok();
  }

  bool FuncClientImpl::doWarble(int event_type,
  	               ClientEventParams& client_event_params, ClientEventReply& client_event_reply) {
    EventRequest event_request;
    EventReply event_reply;
    ClientContext event_context;
    google::protobuf::Any any_request;
    google::protobuf::Any any_reply;
    event_request.set_event_type(event_type);
    // Set warble request information
    WarbleRequest warble_request;
    WarbleReply warble_reply;
    warble_request.set_username(client_event_params.username);
    warble_request.set_text(client_event_params.warble_text);
    warble_request.set_parent_id(client_event_params.parent_warble_id);
    // Construct event request and reply message and make function call.
    any_request.PackFrom(warble_request);      
    event_request.mutable_payload()->google::protobuf::Any::MergeFrom(any_request);
    event_reply.mutable_payload()->google::protobuf::Any::MergeFrom(any_reply);
    Status status = stub_->event(&event_context, event_request, &event_reply);
    event_reply.payload().UnpackTo(&warble_reply);

    // Write warble information to client_event_reply
    Warble warble = warble_reply.warble();
    client_event_reply.warble = warble;

    return status.ok();
  }

  bool FuncClientImpl::doFollow(int event_type,
  	               ClientEventParams& client_event_params, ClientEventReply& client_event_reply) {
    EventRequest event_request;
    EventReply event_reply;
    ClientContext event_context;
    google::protobuf::Any any_request;
    google::protobuf::Any any_reply;
    event_request.set_event_type(event_type);
    // Construct follow request and reply. Make function call to func server.
    FollowRequest follow_request;
    FollowReply follow_reply;
    follow_request.set_username(client_event_params.username);
    follow_request.set_to_follow(client_event_params.to_follow);
    // Construct event request and reply message and make function call.
    any_request.PackFrom(follow_request);      
    event_request.mutable_payload()->google::protobuf::Any::MergeFrom(any_request);
    event_reply.mutable_payload()->google::protobuf::Any::MergeFrom(any_reply);
    Status status = stub_->event(&event_context, event_request, &event_reply);
    event_reply.payload().UnpackTo(&follow_reply);

    return status.ok();
  }

  bool FuncClientImpl::doRead(int event_type,
  	             ClientEventParams& client_event_params, ClientEventReply& client_event_reply) {
    EventRequest event_request;
    EventReply event_reply;
    ClientContext event_context;
    google::protobuf::Any any_request;
    google::protobuf::Any any_reply;
    event_request.set_event_type(event_type);
    // Construct follow request and reply. Make function call to func server.
    ReadRequest read_request;
    ReadReply read_reply;
    read_request.set_warble_id(client_event_params.warble_id);

    any_request.PackFrom(read_request);
    event_request.mutable_payload()->google::protobuf::Any::MergeFrom(any_request);
    event_reply.mutable_payload()->google::protobuf::Any::MergeFrom(any_reply);
    Status status = stub_->event(&event_context, event_request, &event_reply);
    event_reply.payload().UnpackTo(&read_reply);

    for (Warble warble : read_reply.warbles()) {
      client_event_reply.warble_threads.push_back(warble);
    }

    return status.ok();
  }

  bool FuncClientImpl::doProfile(int event_type,
  	                ClientEventParams& client_event_params, ClientEventReply& client_event_reply) {
    EventRequest event_request;
    EventReply event_reply;
    ClientContext event_context;
    google::protobuf::Any any_request;
    google::protobuf::Any any_reply;
    event_request.set_event_type(event_type);
    // Construct profile request and reply. Make function call to func server.
    ProfileRequest profile_request;
    ProfileReply profile_reply;
    profile_request.set_username(client_event_params.username);

    any_request.PackFrom(profile_request);
    event_request.mutable_payload()->google::protobuf::Any::MergeFrom(any_request);
    event_reply.mutable_payload()->google::protobuf::Any::MergeFrom(any_reply);
    Status status = stub_->event(&event_context, event_request, &event_reply);
    event_reply.payload().UnpackTo(&profile_reply);
    
    for (std::string follower : profile_reply.followers()) {
      client_event_reply.followers.push_back(follower);
    }

    for (std::string following : profile_reply.following()) {
      client_event_reply.following.push_back(following);
    }
    
    return status.ok();
  }

}// namespace func

// int main() {

//   func::FuncClientImpl* func_client = new func::FuncClientImpl(grpc::CreateChannel("0.0.0.0:50000",
//                            grpc::InsecureChannelCredentials()));

//   // hook type with function;
//   func_client->hook(0, "registeruser");
//   func_client->hook(1, "warble");
//   func_client->hook(2, "follow");
//   func_client->hook(3, "read");
//   func_client->hook(4, "profile");

//   // post warble:
//   func::ClientEventParams params;
//   func::ClientEventReply reply;
//   params.username = "hyli";
//   params.to_follow = "LBJ";
//   params.warble_text = "play basketball";
//   func_client->event(1, params, reply);
//   std::cout << "username is : " << reply.warble.username() << std::endl;
//   std::cout << "test is : " << reply.warble.text() << std::endl;
//   std::cout << "warble id is : " << reply.warble.id() << std::endl;
  
//   //Read warble:
//   params.warble_id = reply.warble.id();
//   func_client->event(3, params, reply);
//   std::cout << "warble threads length = " << reply.warble_threads.size() << std::endl;
//   std::cout<< "warble threads contains : " << std::endl;
//   std::cout << "username is : " << reply.warble_threads[0].username() << std::endl;
//   std::cout << "text is : " << reply.warble_threads[0].text() << std::endl;
//   std::cout << "warble id is : " << reply.warble_threads[0].id() << std::endl;
//   std::cout << "Timestamp is : " << reply.warble_threads[0].timestamp().seconds() << std::endl;

// }



