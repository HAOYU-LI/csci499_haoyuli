#ifndef KEY_VALUE_SERVICE_H
#define KEY_VALUE_SERVICE_H
#include <grpcpp/grpcpp.h>

#include "key_value_store.pb.h"
#include "key_value_store.grpc.pb.h"

#include "key_value_data_structure.h"

using kvstore::PutRequest;
using kvstore::PutReply;
using kvstore::GetRequest;
using kvstore::GetReply;
using kvstore::RemoveRequest;
using kvstore::RemoveReply;
using grpc::Server;
using grpc::ServerContext;
using grpc::ServerReaderWriter;
using grpc::Status;
using grpc::StatusCode;
using kvstorage::Storage;

namespace kvstore {
class KeyValueStoreImpl final : public KeyValueStore::Service {
  // Put element into key value backend storage. If put failed, return StatusCode::UNKNOWN
  Status put(ServerContext* context, 
  	          const PutRequest* request, PutReply* reply) override;
  // Get value from key value backend storage. If key doesn't exist, return StatusCode::NOT_FOUND
  Status get(ServerContext* context, 
  	          ServerReaderWriter<GetReply, GetRequest>* stream) override;
  // Delete a given key from backend storage. If key doesn't exist, return StatuCode::NOT_FOUND.
  Status remove(ServerContext* context, 
  	                const RemoveRequest* request, RemoveReply* reply) override;
 private:
  Storage storage_;
};
}// End of namespace kvstore

#endif