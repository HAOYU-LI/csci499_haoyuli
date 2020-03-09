#include "command_line.h"

namespace commandline {
  CommandLineClient::CommandLineClient() {
    func_client = new func::FuncClientImpl(grpc::CreateChannel("0.0.0.0:50000",
                            grpc::InsecureChannelCredentials()));
    // hook functions with given event type
    func_client.Hook(REGISTER_TYPE, "registeruser");
    func_client.Hook(WARBLE_TYPE, "warble");
    func_client.Hook(FOLLOW_TYPE, "follow");
    func_client.Hook(READ_TYPE, "read");
    func_client.Hook(PROFILE_TYPE, "profile");
  }
  
  // Handle user registration call
  bool CommandLineClient::RegisterHandler(std::string username) {
    ClientEventParams event_params;
    ClientEventReply  event_reply;
    event_params.username = username;
    bool register_result = func_client->event(REGISTER_TYPE, event_params, event_reply);
    
    if (register_result) {
      LOG(INFO) << "User " << username << " has been registered successfully."
                << std::endl;
    } else {
      LOG(INFO) << "User " << username << " already exists." << std::endl;
    }
    return register_result;
  }

  // Handle warble call from command line. If warble_id is not empty, warble is a reply warble. 
  bool CommandLineClient::WarbleHandler(std::string username, 
                                        std::string warble_text, 
                                        std::warble_id = "") {
    ClientEventParams event_params;
    ClientEventReply  event_reply;
    event_params.username = username;
    event_params.warble_text = warble_text;
    event_params.warble_id = warble_id;
    bool warble_result = func_client->event(WARBLE_TYPE, event_params, event_reply);
    
    if (warble_result) {
      PrintWarble(event_reply.warble);
    } else {
      LOG(INFO) << "This warble cannot be posted" << std::endl;
    }
    return warble_result;
  }

   // Handle follow request from command line.
  bool CommandLineClient::FollowHandler(std::string username, std::string username_to_follow) {
    ClientEventParams event_params;
    ClientEventReply  event_reply;
    event_params.username = username;
    event_params.to_follow = username_to_follow;
    bool follow_result = func_client->event(FOLLOW_TYPE, event_params, event_reply);

    if (follow_result) {
      PrintUser(username, event_reply.following, event_reply.followers);
    } else {
      LOG(INFO) << "Follow request failed." << std::endl;
    }
    return follow_result;
  }

  // Handle read request from command line.
  bool CommandLineClient::ReadHandler(std::string username, std::string warble_id) {
    ClientEventParams event_params;
    ClientEventReply  event_reply;
    event_params.username = username;
    event_params.warble_id = warble_id;
    bool read_result = func_client->event(READ_TYPE, event_params, event_reply);
    bool no_warble_threads = event_reply.warble_threads.size() == 0;
    bool not_posted_by_user = username.compare(event_reply.warble_threads[0].username()) != 0;
    
    if (no_warble_threads) {
      LOG(INFO) << "The warble with warble id : [" << warble_id << "] does not exist." << std::endl;
      return false;
    }
    if (not_posted_by_user) {
      LOG(INFO) << "The warble is not posted by user : " << username << std::endl;
      return false;
    }

    if (read_result) {
    	 LOG(INFO) << "Current warble thread with warble id : [" << warlbe_id << "] has following warbles." << std::endl;
      for (Warble warble : event_reply.warble_threads) {
        PrintWarble(warble);
      }
      LOG(INFO) << "End of warble thread" << std::endl;
    }

    return read_result;
  }

  // Handle profile request from command line.
  bool CommandLineClient::ProfileHandler(std::string username) {
    ClientEventParams event_params;
    ClientEventReply  event_reply;
    event_params.username = username;
    bool profile_result = func_client->event(PROFILE_TYPE, event_params, event_reply);

    if (profile_result) {
      printUser(username, event_reply.following, event_reply.followers);
    } else {
      LOG(INFO) << "Current username does not exist in system" << std::endl;
    }

    return profile_result;
  }

}// namespace commandline



