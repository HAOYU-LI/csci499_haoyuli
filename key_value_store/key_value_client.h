#ifndef KEY_VALUE_CLIENT_H
#define KEY_VALUE_CLIENT_H
#include <string>
#include <vector>
#include <iostream>

#include <grpcpp/grpcpp.h>

#include "key_value_store.pb.h"
#include "key_value_store.grpc.pb.h"

using kvstore::PutRequest;
using kvstore::PutReply;
using kvstore::GetRequest;
using kvstore::GetReply;
using kvstore::RemoveRequest;
using kvstore::RemoveReply;
using kvstore::KeyValueStore;
using grpc::Server;
using grpc::ServerContext;
using grpc::ServerReaderWriter;
using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using grpc::Status;
using grpc::StatusCode;
using std::string;
using std::vector;

namespace kvstore {
/*
  Key value store client class that will be used to connect to key value store server.
*/
class KeyValueClient {
public:
  KeyValueClient(std::shared_ptr<Channel> channel)
    : stub_(KeyValueStore::NewStub(channel)) {}

  // Assemble payloads for put service. Send it to server and receive response.
  void put(const string& key, const string& val);
  // Assemble payloads for get service. Send request to server and print the response.
  void get(const vector<string>& keys);
  // Obtain key from user and send remove request to server.
  void remove(const string& key);

private:
	 std::unique_ptr<KeyValueStore::Stub> stub_;
};
}// End of kvstore namespace

#endif