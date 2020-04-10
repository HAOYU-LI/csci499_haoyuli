#include "key_value_service.h"

#include <string>
#include <vector>

#include <glog/logging.h>

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

// When store flag is defined, class will load information from file.
// Put and Remove request will be saved into file. The operation is 
// similar to AOF in redis.
DEFINE_string(store, "", "Use persistent store mode.");
namespace kvstore {

// Method used to read number character from in_file.
std::string ReadNCharacter(std::ifstream& in_file, int num_character) {
  std::string result = "";
  char ch;
  while (num_character > 0) {
    in_file.get(ch);
    result += ch;
    num_character --;
  }
  in_file.get(ch);
  return result;
}
// Put element into key value backend storage. If put failed, return StatusCode::UNKNOWN
Status KeyValueStoreImpl::put(ServerContext* context, 
                               const PutRequest* request, PutReply* reply) {
  bool put_result = storage_.Put(request->key(), request->value());
  // If persistent mode is activated, save key and value into file.
  if (persistent_mode_) {
    PersistentStore("put", request->key(), request->value());
  }
  return Status::OK;
}

// Get value from key value backend storage. If key doesn't exist, return StatusCode::NOT_FOUND
Status KeyValueStoreImpl::get(ServerContext* context, 
                              ServerReaderWriter<GetReply, GetRequest>* stream) {
  GetRequest request;
  while (stream->Read(&request)) {
    string request_key = request.key();
    LOG(INFO) << "[key_value_server] Received key : " << request_key << std::endl;
    const vector<string>& values = storage_.Get(request_key);
    if (values.size() == 0) {
      LOG(INFO) << "[key_value_server] Key is not found." << std::endl;
      return Status(StatusCode::NOT_FOUND, "Key is not found.");
    }
    for (std::string value : values) {
      GetReply reply;
      reply.set_value(value);
      stream->Write(reply);
    }
    LOG(INFO) << "[key_value_server] End of writing reply." << std::endl;
  }
  return Status::OK;
}

// Delete a given key from backend storage. If key doesn't exist, return StatuCode::NOT_FOUND.
Status KeyValueStoreImpl::remove(ServerContext* context, 
                                     const RemoveRequest* request, RemoveReply* reply) {
  if (storage_.DeleteKey(request->key())) {
    // If persistent mode is activated, append remove request into file.
    if (persistent_mode_) {
      PersistentStore("remove", request->key(), "");
    }
    return Status::OK;
  }
  return Status(StatusCode::NOT_FOUND, "Failed to delete the key.");
}

// When instantiating a KeyValueStore class, load data from persistent_file_
// if the file exists.
void KeyValueStoreImpl::PersistentLoad() {
  if (persistent_file_.length() == 0) {
    persistent_mode_ = false;
    return;
  }
  
  persistent_mode_ = true;
  std::ifstream in_file;
  in_file.open(persistent_file_);
  // If file doesn't exist before, create it.
  if (!in_file.is_open()) {
    std::cout << "Create new file " << persistent_file_ << " for persistent store..." << std::endl;
    std::ofstream out_file;
    out_file.open(persistent_file_);
    out_file.close();
    std::cout << "Successfully created." << std::endl;
    return;
  }
   
  //  Read data from persistent_file_. Key-Value pairs in `put` request are separated into four
  //  parts. 1. # of chars for key; 2. Key; 3. # of chars for value; 4. Value;
  //  For `remove` request, Only key is stored. Only first two parts are stored.
  std::string request;
  while (getline(in_file, request)) {
    std::string key_len, value_len, key, value;
    if (request.compare("put") == 0) {
      getline(in_file, key_len);
      key = ReadNCharacter(in_file, std::stoi(key_len));
      getline(in_file, value_len);
      value = ReadNCharacter(in_file, std::stoi(value_len));
      storage_.Put(key, value);
    } else if (request.compare("remove") == 0) {
      getline(in_file, key_len);
      key = ReadNCharacter(in_file, std::stoi(key_len));
      storage_.DeleteKey(key);
    } else {
      // meet eof character. just break
      break;
    }
  }

  in_file.close();
  return;
}

// If persistent store mode is set, Put and Remove request will be saved into
// persistent_file_
void KeyValueStoreImpl::PersistentStore(std::string request, std::string key, std::string value) {
  std::ofstream out_file(persistent_file_, std::ios_base::app | std::ios_base::out);
  out_file << request << "\n";
  out_file << key.length() << "\n";
  out_file << key << "\n";
  if (request.compare("put") == 0) { 
    out_file << value.length() << "\n";
    out_file << value << "\n";
  }
  out_file.close();
}

} // End of namespace kvstore.

int main(int argc, char** argv) {
  google::InitGoogleLogging(argv[0]);
  google::ParseCommandLineFlags(&argc, &argv, true);
  string key_value_server_address("0.0.0.0:50001");
  ServerBuilder serverbuilder;
  kvstore::KeyValueStoreImpl service_impl(FLAGS_store);
  serverbuilder.AddListeningPort(key_value_server_address, 
                            grpc::InsecureServerCredentials());
  serverbuilder.RegisterService(&service_impl);
  std::unique_ptr<Server> server(serverbuilder.BuildAndStart());
  std::cout<<"key value server is listening on port : "<<key_value_server_address<<std::endl;
  server->Wait();
}


