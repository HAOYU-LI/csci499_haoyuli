#include "key_value_client.h"

#include <string>
#include <vector>
#include <iostream>
#include <thread>

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
using grpc::ClientReaderWriter;
using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using grpc::StatusCode;
using std::string;
using std::vector;
using std::cout;
using std::endl;

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
void KeyValueClient::get(const vector<string>& keys) {
  ClientContext context;
  std::shared_ptr<ClientReaderWriter<GetRequest, GetReply> > stream(
        stub_->get(&context));
  // Transport keys to server and get response asynchronously.
  // Create thread for writing request to server:
  std::thread writer([stream, keys]() {
  for (const string& key : keys) {
    cout << "Sending key : " << key << " to key value store server" << endl;
    GetRequest request;
    request.set_key(key); 
    stream->Write(request);
  }
  stream->WritesDone();
  });
  // Main thread that is used to read response from server.
  GetReply reply;
  while (stream->Read(&reply)) {
    cout << "Get reply " << reply.value() << " from server" << endl;
  }
  writer.join();
  Status status = stream->Finish();
  if (!status.ok()) {
    cout << "get rpc failed." << endl;
    return;
  }
  cout << "get rpc succeed." << endl;
}

// Obtain key from user and send remove request to server.
void KeyValueClient::remove(const string& key) {
  RemoveRequest request;
  request.set_key(key);
  RemoveReply reply;
  ClientContext context;

  Status status = stub_->remove(&context, request, &reply);
} 

}// End of kvstore namespace