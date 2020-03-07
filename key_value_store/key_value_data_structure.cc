#include "key_value_data_structure.h"

namespace kvstorage{
const std::vector<std::string> Storage::empty_;

bool Storage::Put(const string& key, const string& val) {
  std::lock_guard<std::mutex> lock(storage_mutex_);
  bool notFindKey = storage_map_.find(key) == storage_map_.end();
  if (notFindKey) {
    storage_map_[key] = vector<string>();
  }
  storage_map_[key].push_back(val);
  return true;
}

const vector<string>& Storage::Get(const string& key) {
  std::lock_guard<std::mutex> lock(storage_mutex_);
  unordered_map<string, vector<string>>::const_iterator got = storage_map_.find(key);
  bool get_succeed = got != storage_map_.end();
  return get_succeed ? got->second : empty_;
}

bool Storage::DeleteKey(const string& key) {
  std::lock_guard<std::mutex> lock(storage_mutex_);
  bool delete_succeed = storage_map_.find(key) == storage_map_.end() || 
                           storage_map_.erase(key) == 1;
  return delete_succeed;
}
}// End of kvstorage namespace