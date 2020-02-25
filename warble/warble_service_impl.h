#ifndef WARBLE_SERVICE_IMPL_H
#define WARBLE_SERVICE_IMPL_H

#include <grpcpp/grpcpp.h>
#include "warble_service.pb.h"

#include "../key_value_store/key_value_data_structure.h"
#include "../key_value_store/key_value_client.h"

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
/*
  A bunch of functions executed by func upon appropriate events 
  arriving from command-line client.
*/
class WarbleService {
public:
  // Register users to backend store given non-blank username
  // If username already exists, return StatusCode::ALREADY_EXISTS
  Status RegisterUser(const RegisteruserRequest* request,
                            RegisteruserReply* reply,
                      const KeyValueClient& kvclient);
  // Posts a new warble (optionally as a reply), returns the id of the new warble
  Status Warble(const WarbleRequest* request,
                      WarbleReply* reply,
                const KeyValueClient& kvclient);
  // Starts following a given user;
  Status Follow(const FollowRequest* request,
                      FollowReply* reply,
                const KeyValueClient& kvclient);
  // Reads a warble thread from the given id
  Status Read(const ReadRequest* request, 
                    ReadReply* reply,
              const KeyValueClient& kvclient);
  // Returns this user’s following and followers
  Status Profile(const ProfileRequest* request,
                       ProfileReply* reply,
                 const KeyValueClient& kvclient);
};
}// End of warble namespace.

#endif