#include "key_value_data_structure.h"

namespace kvstorage{

bool Storage::Put(const string& key, const string& val) {
  //we don't allow put empty message into database.
  if (val.length() == 0) {
    return false; 
  }
  std::lock_guard<std::mutex> lock(storage_mutex_);
  storage_map_.insert({key, val});
  unordered_map<string, string>::const_iterator got = storage_map_.find(key);
  bool put_succeed = got != storage_map_.end() && val.compare(got->second) == 0;
  return put_succeed;
}

const string& Storage::Get(const string& key) {
  std::lock_guard<std::mutex> lock(storage_mutex_);
  unordered_map<string, string>::const_iterator got = storage_map_.find(key);
  bool get_succeed = got != storage_map_.end();
  return get_succeed ? got->second : "";
}

bool Storage::DeleteKey(const string& key) {
  std::lock_guard<std::mutex> lock(storage_mutex_);
  boolean delete_succeed = storage_map_.find(key) == storage_map_.end() || 
                           storage_map_.erase(key) == 1;
  return delete_succeed;
}

}// end of kvstorage namespace