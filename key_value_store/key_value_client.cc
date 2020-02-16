#include "key_value_client.h"

#include <string>
#include <vector>

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
// Assemble payloads for put service. Send it to server and receive response.
void KeyValueClient::put(const string& key, const string& val) {
  PutRequest request;
  request.set_key(key);
  request.set_value(val);

  PutReply reply;

  ClientContext context;

  Status status = stub_->put(&context, request, &reply);
}
// Assemble payloads for get service. Send request to server and return the response.
const vector<string>* KeyValueClient::get(const vector<string>& keys) {
  ClientContext context;
  std::shared_ptr<ClientReaderWriter<GetRequest, GetReply>> stream(
        stub_->RouteChat(&context));
  
}

}