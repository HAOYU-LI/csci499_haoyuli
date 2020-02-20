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
  Status registerUser(const RegisteruserRequest* request,
  	                     RegisteruserReply* reply,
  	                     KeyValueClient* kvclient);
  // Posts a new warble (optionally as a reply), returns the id of the new warble
  Status warble(const WarbleRequest* request,
  	                     WarbleReply* reply,
  	                     KeyValueClient* kvclient);
  // Starts following a given user;
  Status follow(const FollowRequest* request,
  	                     FollowReply* reply,
  	                     KeyValueClient* kvclient);
  // Reads a warble thread from the given id
  Status read(const ReadRequest* request, 
  	                     ReadReply* reply,
  	                     KeyValueClient* kvclient);
  // Returns this user’s following and followers
  Status profile(const ProfileRequest* request,
  	                     ProfileReply* reply,
  	                     KeyValueClient* kvclient);
};
}// End of warble namespace.

#endif
