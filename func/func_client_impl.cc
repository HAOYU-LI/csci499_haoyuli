#include "func_client_impl.h"

#include <iostream>

namespace func {
  // hook method is used to combine event type with specific handler function name.
  bool FuncClientImpl::Hook(int event_type, std::string function_name) {
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
  bool FuncClientImpl::Unhook(int event_type) {
    UnhookRequest request;
    UnhookReply reply;
    ClientContext context;
    request.set_event_type(event_type);
    
    Status status = stub_->unhook(&context, request, &reply);
    return status.ok();
  }

  // event method is responsible for calling function with given event type 
  bool FuncClientImpl::Event(int event_type, ClientEventParams& client_event_params,
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
      return RegisterHelper(event_type, client_event_params, client_event_reply);
    }
    if (function_name.compare(FuncClientImpl::WARBLE) == 0) {
      return WarbleHelper(event_type, client_event_params, client_event_reply);
    }
    if (function_name.compare(FuncClientImpl::FOLLOW) == 0) {
      return FollowHelper(event_type, client_event_params, client_event_reply);
    }
    if (function_name.compare(FuncClientImpl::READ) == 0) {
      return ReadHelper(event_type, client_event_params, client_event_reply);
    }
    if (function_name.compare(FuncClientImpl::PROFILE) == 0) {
      return ProfileHelper(event_type, client_event_params, client_event_reply);
    }
    if (function_name.compare(FuncClientImpl::STREAM) == 0) {
      return StreamHelper(event_type, client_event_params, client_event_reply);
    }

    return false;
  }

  // Helper method used by Event to handling registeruser request.
  bool FuncClientImpl::RegisterHelper(int event_type,
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

  // Helper method used by Event to handling warble request.
  bool FuncClientImpl::WarbleHelper(int event_type,
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

  // Helper method used by Event to handling follow request.
  bool FuncClientImpl::FollowHelper(int event_type,
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

  // Helper method used by Event to handling read request.
  bool FuncClientImpl::ReadHelper(int event_type,
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
    read_request.set_username(client_event_params.username);

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
  
  // Helper method used by Event to handling profile request.
  bool FuncClientImpl::ProfileHelper(int event_type,
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

  // Helper method used by Event to handling streaming warbles.
  bool FuncClientImpl::StreamHelper(int event_type,
                    ClientEventParams& client_event_params, ClientEventReply& client_event_reply) {
    EventRequest event_request;
    EventReply event_reply;
    ClientContext event_context;
    google::protobuf::Any any_request;
    google::protobuf::Any any_reply;
    event_request.set_event_type(event_type);

    // Construct stream request and reply. Make function call to func server.
    StreamRequest stream_request;
    StreamReply stream_reply;
    stream_request.set_hashtag(client_event_params.hashtag);

    any_request.PackFrom(stream_request);
    event_request.mutable_payload()->google::protobuf::Any::MergeFrom(any_request);
    event_reply.mutable_payload()->google::protobuf::Any::MergeFrom(any_reply);
    Status status = stub_->event(&event_context, event_request, &event_reply);
    event_reply.payload().UnpackTo(&stream_reply);

    for (Warble warble : stream_reply.warbles()) {
      client_event_reply.stream.push_back(warble);
    }

    return status.ok();

  }

}// namespace func