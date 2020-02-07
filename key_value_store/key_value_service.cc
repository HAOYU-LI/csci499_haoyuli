#include "key_value_service.h"

#include <string>

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
using kvstorage::Storage;

namespace kvstore {
//put element into key value backend storage. If put failed, return StatusCode::UNKNOWN
Status KeyValueStoreImpl::put(ServerContext* context, 
	                             const PutRequest* request, PutReply* reply) {
  bool put_result = storage_.Put(request->key(), request->value());
 	if (put_result) {
 	  return Status::OK;
 	} else {
 	  return Status(StatusCode::UNKNOWN, "put <key, value> failed.");
 	}
}

//get value from key value backend storage. If key doesn't exist, return StatusCode::NOT_FOUND
Status KeyValueStoreImpl::get(ServerContext* context, 
	                             ServerReaderWriter<GetReply, GetRequest>* stream) {
  GetRequest request;
  while (stream->Read(&request)) {
    string request_key = request.key();
    string reply_value = storage_.Get(request_key);
    if (reply_value.length() > 0) {
      GetReply reply;
      reply.set_value(reply_value);
      stream->Write(reply);
      return Status::OK;
    } else {
      return Status(StatusCode::NOT_FOUND, "The key does not exist in database.");
    }
  }
}

//delete a given key from backend storage. If key doesn't exist, return StatuCode::NOT_FOUND.
Status KeyValueStoreImpl::remove(ServerContext* context, 
	                                   const RemoveRequest* request, RemoveReply* reply) {
  if (storage_.DeleteKey(request->key())) {
    return Status::OK;
  } else {
    return Status(StatusCode::NOT_FOUND, "Failed to delete the key.");
  }
}
} // end of namespace kvstore.

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


