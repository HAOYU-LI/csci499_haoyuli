#ifndef KEY_VALUE_SERVICE_H
#define KEY_VALUE_SERVICE_H
#include <string>
#include <iostream>
#include <fstream>

#include <grpcpp/grpcpp.h>
#include <gflags/gflags.h>

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
/*
  KeyValueStoreImpl is a child class of KeyValueStore::Service which
  implements put, get and remove rpc service defined in the proto file.
*/
class KeyValueStoreImpl final : public KeyValueStore::Service {
public:
  // Put element into key value backend storage. If put failed, return StatusCode::UNKNOWN
  Status put(ServerContext* context, 
  	          const PutRequest* request, PutReply* reply) override;
  // Get value from key value backend storage. If key doesn't exist, return StatusCode::NOT_FOUND
  Status get(ServerContext* context, 
  	          ServerReaderWriter<GetReply, GetRequest>* stream) override;
  // Delete a given key from backend storage. If key doesn't exist, return StatuCode::NOT_FOUND.
  Status remove(ServerContext* context, 
  	                const RemoveRequest* request, RemoveReply* reply) override;
  // Constructor for KeyValueStoreImpl class.
  KeyValueStoreImpl(std::string file) : KeyValueStore::Service(), persistent_db_(file){
    PersistentLoad();
  }

 private:
  Storage storage_;
  std::string persistent_db_;
  bool persistent_mode_;

  // When instantiating a KeyValueStore class, load data from persistent_db_
  // if the file exists.
  void PersistentLoad();

  // If persistent store mode is set, Put and Remove request will be saved into
  // persistent_db_ file
  void PersistentStore(std::string request, std::string key, std::string value);
};
}// End of namespace kvstore

#endif