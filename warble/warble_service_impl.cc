#include "warble_service_impl.h"

#include <string>
#include <vector>
#include <sys/time.h>

#include <glog/logging.h>

using warble::Timestamp;
using warble::Warble;
using warble::RegisteruserRequest;
using warble::RegisteruserReply;
using warble::WarbleRequest;
using warble::WarbleReply;
using warble::FollowRequest;
using warble::FollowReply;
using warble::ReadRequest;
using warble::ReadReply;
using warble::ProfileRequest;
using warble::ProfileReply;
using grpc::Status;
using grpc::StatusCode;
using kvstore::KeyValueClient;

namespace warble {

// Register users to backend store given non-blank username
// If username already exists, return StatusCode::ALREADY_EXISTS
Status WarbleService::RegisterUser(const RegisteruserRequest* request,
                                   RegisteruserReply* reply,
                                   KeyValueClient* kvclient) {
  const std::string username = request->username();
  const std::string username_key = "user_" + username;
  std::vector<std::string> kvclient_key{username_key};
  std::vector<std::string>* kvclient_get_result = kvclient->Get(kvclient_key);
  if (kvclient_get_result != nullptr) {
    LOG(INFO) << "[warble_service] Username already exists" << std::endl;
    delete kvclient_get_result;
    return Status(StatusCode::ALREADY_EXISTS, "Username already exists.");
  }
  kvclient->Put(username_key, username);
  return Status::OK;
}

void WarbleService::WarbleStreaming(std::string serialized_warble, 
                    std::string text,
                    KeyValueClient* kvclient){
  std::vector<std::string> streaming_key{"stream_"};
  std::vector<std::string>* hashtags = kvclient->Get(streaming_key);
  std::unordered_map<std::string, std::string> hashtag_map;

  for (int i=0; i<hashtags->size(); i++){
    hashtag_map.insert({(*hashtags)[i],(*hashtags)[i]});
  }

  std::stringstream ss(text); 
  std::string word; 
      
  while (std::getline(ss, word, ' ')) { 
    auto it = hashtag_map.find(word);
    if (it != hashtag_map.end()){
      std::string stream_key = word + "_";
      kvclient->Put(stream_key, serialized_warble);
    }
  }
}

// Posts a new warble (optionally as a reply), returns the id of the new warble
Status WarbleService::WarbleMethod(const WarbleRequest* request,
                             WarbleReply* reply,
                             KeyValueClient* kvclient) {
  // Check whether user exists in database:
  std::vector<std::string> user_existence_key{"user_" + request->username()};
  std::vector<std::string>* user_existence_value = kvclient->Get(user_existence_key);
  if (user_existence_value == nullptr) {
    return Status(StatusCode::NOT_FOUND, "User does not exist in database");
  }
  // Reply Warble id should be valid
  std::string parent_id = request->parent_id();
  if (parent_id.length() != 0) {
    std::vector<std::string> id_valid_key{parent_id};
    std::vector<std::string>* id_valid_value = kvclient->Get(id_valid_key);
    if (id_valid_value == nullptr) {
      return Status(StatusCode::NOT_FOUND, "Parent id does not exist in database.");
    }
  }
  // Store WarbleRequest into a warble.
  Warble* warble_to_reply = reply->mutable_warble();
  warble_to_reply->set_username(request->username());
  warble_to_reply->set_text(request->text());
  warble_to_reply->set_parent_id(request->parent_id());
  // Set timestamp of warble.
  Timestamp* timestamp = warble_to_reply->mutable_timestamp();
  struct timeval tv;
  gettimeofday(&tv, NULL);
  timestamp->set_seconds((int64_t) tv.tv_sec);
  timestamp->set_useconds((int64_t) tv.tv_usec);
  // Warble id will be serialized warble message
  // (username, text, parent_id, timestamp) without id.
  std::string serialized_warble_id;
  warble_to_reply->SerializeToString(&serialized_warble_id);
  std::size_t hashed_warble_id = std::hash<std::string>{}(serialized_warble_id);
  std::string warble_id = std::to_string(hashed_warble_id);
  warble_to_reply->set_id(warble_id);
  // Save warble into kvstore.
  std::string serialized_warble;
  warble_to_reply->SerializeToString(&serialized_warble);
  kvclient->Put(warble_id, serialized_warble);

  // Parse warble for streaming
  WarbleStreaming(serialized_warble, request->text(), kvclient);
  return Status::OK;
}

// Starts following a given user;
Status WarbleService::Follow(const FollowRequest* request,
                             FollowReply* reply,
                             KeyValueClient* kvclient) {
  std::string username = request->username();
  std::string username_key = "following_" + username;
  std::string username_to_follow = request->to_follow();
  std::string username_to_follow_key = "followed_by_" + username_to_follow;

  // Check whether user and user_to_follow exist in current database:
  std::vector<std::string> user_existence_key{"user_" + username};
  std::vector<std::string> username_to_follow_existence_key{"user_" + username_to_follow};
  std::vector<std::string>* user_existence_value = kvclient->Get(user_existence_key);
  std::vector<std::string>* username_to_follow_existence_value = kvclient->Get(username_to_follow_existence_key);
  if (user_existence_value == nullptr || username_to_follow_existence_value == nullptr) {
    return Status(StatusCode::NOT_FOUND, "User or User_to_follow does not exist in database");
  }

  // Users cannot follow themselves.
  if (username.compare(username_to_follow) == 0) {
    return Status(StatusCode::NOT_FOUND, "Users cannot follow themselves.");
  }

  kvclient->Put(username_key, username_to_follow);
  kvclient->Put(username_to_follow_key, username);
  
  return Status::OK;
}

// Reads a warble thread from the given id
Status WarbleService::Read(const ReadRequest* request, 
                           ReadReply* reply,
                           KeyValueClient* kvclient) {
  std::string warble_id = request->warble_id();
  std::string username = request->username();
  // Check whether user exists in database
  std::vector<std::string> user_existence_key{"user_" + username};
  std::vector<std::string>* user_existence_value = kvclient->Get(user_existence_key);
  if (user_existence_value == nullptr) {
    return Status(StatusCode::NOT_FOUND, "User does not exist in database");
  }
  // Fetch warbles from backend store.
  std::vector<std::string> kvclient_key{warble_id};
  std::vector<std::string>* serialized_warbles = kvclient->Get(kvclient_key);
  if (serialized_warbles == nullptr) {
    LOG(INFO) << "[warble_service] Warble_id is not found." << std::endl;
    return Status(StatusCode::NOT_FOUND, "Warble_id is not found.");
  }
  // Iterate serialized warble strings and parse them back to warble.
  while (serialized_warbles != nullptr && serialized_warbles->size() == 1) {
    Warble* new_warble = reply->add_warbles();
    new_warble->ParseFromString((*serialized_warbles)[0]);
    warble_id = new_warble->parent_id();
    std::vector<std::string> key{warble_id};
    delete serialized_warbles;
    serialized_warbles = kvclient->Get(key);
  }
  
  return Status::OK;
}

// Returns this user’s following and followers
Status WarbleService::Profile(const ProfileRequest* request,
                              ProfileReply* reply,
                              KeyValueClient* kvclient) {
  std::string username = request->username();
  // Check whether user exists in database
  std::vector<std::string> user_existence_key{"user_" + username};
  std::vector<std::string>* user_existence_value = kvclient->Get(user_existence_key);
  if (user_existence_value == nullptr) {
    return Status(StatusCode::NOT_FOUND, "User does not exist in database");
  }
  // Fetch following and followed user information
  const std::string username_following = "following_" + username;
  const std::string username_to_be_followed = "followed_by_" + username;
  std::vector<std::string> following_key{username_following};
  std::vector<std::string> followed_key{username_to_be_followed};

  std::vector<std::string>* following_usernames = kvclient->Get(following_key);
  std::vector<std::string>* followed_by_usernames = kvclient->Get(followed_key);

  if (following_usernames != nullptr) {
    for (const std::string username : (*following_usernames)) {
      reply->add_following(username);
    }
    delete following_usernames;
  }
  
  if (followed_by_usernames != nullptr) {
    for (const std::string username : (*followed_by_usernames)) {
      reply->add_followers(username);
    }
    delete followed_by_usernames;
  }
  
  return Status::OK;
}

// Returns warble that matches streaming
Status WarbleService::Stream(const StreamRequest* request,
                       StreamReply* reply,
                       KeyValueClient* kvclient) {
  std::string hashtag = request->hashtag();

  const std::string hashtag_key = hashtag + "_";
  std::vector<std::string> streaming_key{hashtag_key};
  std::vector<std::string>* streaming_warbles = kvclient->Get(streaming_key);

  if (streaming_warbles != nullptr) {
    // Iterate through serialized warbles containing hashtag and parse to warble
    for(int i=0; i<streaming_warbles->size(); i++){
      Warble* new_warble = reply->add_warbles();
      new_warble->ParseFromString((*streaming_warbles)[i]);
    }
    delete streaming_warbles;
  }
  else{ // hashtag_ entry not yet in db 
    kvclient->Put(hashtag_key, "");
    std::string stream_key = "stream_";
    kvclient->Put(stream_key, hashtag);
  }
  return Status::OK;
}

}// namespace warble
