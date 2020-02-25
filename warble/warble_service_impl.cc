#include "warble_service_impl.h"

#include <string>
#include <vector>

#include "warble_service.pb.h"

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
                                   const KeyValueClient& kvclient) {
  std::string username = request->username();
  std::string username_key = "user_" + username;
  std::vector<std::string> kvclient_key{username_key};
  const std::vector<std::string>* kvclient_get_result = kvclient.Get(kvclient_key);
  if (kvclient_get_result != nullptr) {
    return Status(StatusCode::ALREADY_EXISTS, "Username already exists.");
  }
  kvclient.Put(username_key, username);
  return Status::OK;
}

// Posts a new warble (optionally as a reply), returns the id of the new warble
Status WarbleService::Warble(const WarbleRequest* request,
                             WarbleReply* reply,
                             const KeyValueClient& kvclient) {
  //to be done
}

// Starts following a given user;
Status WarbleService::Follow(const FollowRequest* request,
                             FollowReply* reply,
                             const KeyValueClient& kvclient) {
  std::string username = request->username();
  std::string username_key = "following_" + username;
  std::string username_to_follow = request->to_follow();
  std::string username_to_follow_key = "followed_by_" + username_to_follow;

  kvclient.Put(username_key, username_to_follow);
  kvclient.Put(username_to_follow_key, username);
  
  return Status::OK;
}

// Reads a warble thread from the given id
Status WarbleService::Read(const ReadRequest* request, 
                           ReadReply* reply,
                           const KeyValueClient& kvclient) {
  // to be done
  // ReadReply* reply needs to return Warbles to users,
  // which means Warble should be stored as string
  // in backend storage. Point needs to be solved.
}

// Returns this user’s following and followers
Status WarbleService::Profile(const ProfileRequest* request,
                              ProfileReply* reply,
                              const KeyValueClient& kvclient) {
  std::string username = request->username();
  std::string username_following = "following_" + username;
  std::string username_to_be_followed = "followed_by_" + username;

  const std::vector<std::string>* following_usernames = kvclient.Get(username_following);
  const std::vector<std::string>* followed_by_usernames = kvclient.Get(username_to_be_followed);

  if (following_usernames != nullptr) {
    for (auto username : &following_usernames) {
      reply->following().add_following(username);
    }
  }

  if (followed_by_usernames != nullptr) {
    for (auto username : &followed_by_usernames) {
      reply->followers().add_followers(username);
    }
  }

  return Status::OK;
}
}// namespace warble