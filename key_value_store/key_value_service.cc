#include "key_value_service.h"

#include <string>
#include <vector>

#include "key_value_store.grpc.pb.h"
#include "key_value_store.pb.h"

using kvstore::PutRequest;
using kvstore::PutReply;
using kvstore::GetRequest;
using kvstore::GetReply;
using kvstore::RemoveRequest;
using kvstore::RemoveReply;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReaderWriter;
using grpc::Status;
using grpc::StatusCode;
using std::string;
using std::vector;
using kvstorage::Storage;

namespace kvstore {
// Put element into key value backend storage. If put failed, return StatusCode::UNKNOWN
Status KeyValueStoreImpl::put(ServerContext* context, 
	                             const PutRequest* request, PutReply* reply) {
  bool put_result = storage_.Put(request->key(), request->value());
  return Status::OK;
}

// Get value from key value backend storage. If key doesn't exist, return StatusCode::NOT_FOUND
Status KeyValueStoreImpl::get(ServerContext* context, 
	                            ServerReaderWriter<GetReply, GetRequest>* stream) {
  GetRequest request;
  while (stream->Read(&request)) {
    string request_key = request.key();
    std::cout << "received key : " << request_key << std::endl;
    const vector<string>* values = storage_.Get(request_key);
    if (values == nullptr) {
      std::cout << "Key is not found." << std::endl;
      return Status(StatusCode::NOT_FOUND, "Key is not found.");
    }
    for (auto value : (*values)) {
      GetReply reply;
      reply.set_value(value);
      stream->Write(reply);
    }
    std::cout << "end of writing reply." << std::endl;
  }
  return Status::OK;
}

// Delete a given key from backend storage. If key doesn't exist, return StatuCode::NOT_FOUND.
Status KeyValueStoreImpl::remove(ServerContext* context, 
	                                   const RemoveRequest* request, RemoveReply* reply) {
  if (storage_.DeleteKey(request->key())) {
    return Status::OK;
  }
  return Status(StatusCode::NOT_FOUND, "Failed to delete the key.");
}

} // End of namespace kvstore.

int main(int argc, char** argv) {
  string key_value_server_address("0.0.0.0:50001");
  ServerBuilder serverbuilder;
  kvstore::KeyValueStoreImpl service_impl;
  serverbuilder.AddListeningPort(key_value_server_address, 
  	                        grpc::InsecureServerCredentials());
  serverbuilder.RegisterService(&service_impl);
  std::unique_ptr<Server> server(serverbuilder.BuildAndStart());
  std::cout<<"key value server is listening on port : "<<key_value_server_address<<std::endl;
  server->Wait();
}


