#include "command_line.h"

using commandline::CommandLineClient;

DEFINE_string(registeruser, "", "Registers the given username");
DEFINE_string(user, "", "Logs in as the given username");
DEFINE_string(warble, "", "Creates a new warble with the given text");
DEFINE_string(reply, "", "Indicates that the new warble is a reply to the given id");
DEFINE_string(follow, "", "Starts following the given username");
DEFINE_string(read, "", "Reads the warble thread starting at the given id");
DEFINE_bool(profile, false, "Gets the user’s profile of following and followers");
DEFINE_string(stream, "", "Streams all new warbles containing ‘hashtag’");

namespace commandline {
  CommandLineClient::CommandLineClient() {
    func_client = new func::FuncClientImpl(grpc::CreateChannel("0.0.0.0:50000",
                            grpc::InsecureChannelCredentials()));
    // hook functions with given event type
    func_client->Hook(REGISTER_TYPE, "registeruser");
    func_client->Hook(WARBLE_TYPE, "warble");
    func_client->Hook(FOLLOW_TYPE, "follow");
    func_client->Hook(READ_TYPE, "read");
    func_client->Hook(PROFILE_TYPE, "profile");
    func_client->Hook(STREAM_TYPE, "stream");
  }
  
  // Handle user registration call
  bool CommandLineClient::RegisterHandler(std::string username) {
    ClientEventParams event_params;
    ClientEventReply  event_reply;
    event_params.username = username;
    bool register_result = func_client->Event(REGISTER_TYPE, event_params, event_reply);
    
    if (register_result) {
      std::cout << "User " << username << " has been registered successfully."
                << std::endl;
    } else {
      std::cout << "User " << username << " already exists." << std::endl;
    }
    return register_result;
  }

  // Handle warble call from command line. If warble_id is not empty, warble is a reply warble. 
  bool CommandLineClient::WarbleHandler(std::string username, 
                                        std::string warble_text, 
                                        std::string parent_warble_id) {
    ClientEventParams event_params;
    ClientEventReply  event_reply;
    event_params.username = username;
    event_params.warble_text = warble_text;
    event_params.parent_warble_id = parent_warble_id;
    bool warble_result = func_client->Event(WARBLE_TYPE, event_params, event_reply);
    
    if (warble_result) {
      PrintWarble(event_reply.warble);
    } else {
      std::cout << "This warble cannot be posted" << std::endl;
    }
    return warble_result;
  }

   // Handle follow request from command line.
  bool CommandLineClient::FollowHandler(std::string username, std::string username_to_follow) {
    ClientEventParams event_params;
    ClientEventReply  event_reply;
    event_params.username = username;
    event_params.to_follow = username_to_follow;
    bool follow_result = func_client->Event(FOLLOW_TYPE, event_params, event_reply);

    if (!follow_result) {
      std::cout << "Follow request failed." << std::endl;
    }
    return follow_result;
  }

  // Handle read request from command line.
  bool CommandLineClient::ReadHandler(std::string username, std::string warble_id) {
    ClientEventParams event_params;
    ClientEventReply  event_reply;
    event_params.username = username;
    event_params.warble_id = warble_id;
    bool read_result = func_client->Event(READ_TYPE, event_params, event_reply);
    LOG(INFO) << "In command_line_client, return from Read call";
    bool no_warble_threads = event_reply.warble_threads.size() == 0;
    if (no_warble_threads) {
      std::cout << "The warble with warble id : [" << warble_id << "] does not exist." << std::endl;
      return false;
    }

    if (read_result) {
    	 std::cout << "Current warble thread with warble id : [" << warble_id << "] has following warbles." << std::endl;
      for (int i = event_reply.warble_threads.size() - 1; i >= 0; i --) {
        Warble warble = event_reply.warble_threads[i];
        PrintWarble(warble);
      }
      std::cout << "End of warble thread" << std::endl;
    }

    return read_result;
  }

  // Handle profile request from command line.
  bool CommandLineClient::ProfileHandler(std::string username) {
    ClientEventParams event_params;
    ClientEventReply  event_reply;
    event_params.username = username;
    bool profile_result = func_client->Event(PROFILE_TYPE, event_params, event_reply);

    if (profile_result) {
      PrintUser(username, event_reply.following, event_reply.followers);
    } else {
      std::cout << "Current username does not exist in system" << std::endl;
    }

    return profile_result;
  }

// Parse hashtag string 
  std::vector<std::string> CommandLineClient::ParseHashtag(std::string raw_hashtag) {
    LOG(INFO) << "In command_line_client, parsing hashtag";
    std::vector<std::string> words; 

    std::stringstream ss(raw_hashtag); 
    std::string word; 
      
    while(getline(ss, word, '#')) { 
        words.push_back(word); 
    }

    return words; 
}

// Handle warble streaming
bool CommandLineClient::StreamHandler(std::string username, std::string hashtag) {
    std::vector<std::string> words = ParseHashtag(hashtag);
    if(words.size() == 0){ // hashtag formatted incorrectly
      std::cout << "Streaming #" << hashtag << " failed." << std::endl;
      return false;
    }

    // TODO - clarify if there's #Hello#World, client should stream both hello and world
    // and if it should be case sensitive
    ClientEventParams event_params;
    ClientEventReply  event_reply;
    for(std::string word : words){
      event_params.hashtag = word;
      bool stream_result = func_client->Event(STREAM_TYPE, event_params, event_reply);

      if (!stream_result) {
          std::cout << "Streaming #" << hashtag << " failed." << std::endl;
          return false;
      }
    }

    // Start streaming
    while(true){
      std::chrono::seconds duration(5);
      std::this_thread::sleep_for(duration);

      for(std::string word : words) {
        CheckStream(word);
      }
    }

    return true;
}

// Handles updating stream with new warbles
void CommandLineClient::CheckStream(std::string hashtag){
    ClientEventParams event_params;
    ClientEventReply  event_reply;
    event_params.hashtag = hashtag;
    bool stream_result = func_client->Event(STREAM_TYPE, event_params, event_reply);

    LOG(INFO) << "In command_line_client, checking for new warbles with hashtag";

    if (stream_result) {
      for (int i = event_reply.stream.size() - 1; i >= 0; i --) {
        Warble warble = event_reply.stream[i];
        PrintWarble(warble);
      }
    }
}

}// namespace commandline

int main(int argc, char** argv) {
  google::ParseCommandLineFlags(&argc, &argv, true);
  CommandLineClient client;

  FlagOption flag_option;
  flag_option.registeruser = FLAGS_registeruser;
  flag_option.user = FLAGS_user;
  flag_option.warble = FLAGS_warble;
  flag_option.reply = FLAGS_reply;
  flag_option.follow = FLAGS_follow;
  flag_option.read = FLAGS_read;
  flag_option.profile = FLAGS_profile;
  flag_option.hashtag = FLAGS_stream;

  int request_type = ParseFlag(flag_option);
  switch (request_type) {
    case REGISTER_FLAG:
      client.RegisterHandler(flag_option.registeruser);
      break;
    case WARBLE_FLAG:
      client.WarbleHandler(flag_option.user, flag_option.warble);
      break;
    case REPLY_FLAG:
      client.WarbleHandler(flag_option.user, flag_option.warble, flag_option.reply);
      break;
    case FOLLOW_FLAG:
      client.FollowHandler(flag_option.user, flag_option.follow);
      break;
    case READ_FLAG:
      client.ReadHandler(flag_option.user, flag_option.read);
      break;
    case PROFILE_FLAG:
      client.ProfileHandler(flag_option.user);
      break;
    case STREAM_FLAG:
      client.StreamHandler(flag_option.user, flag_option.hashtag);
      break;
    case OTHER_FLAG:
      PrintOptions();
  }

  return 0;
}

