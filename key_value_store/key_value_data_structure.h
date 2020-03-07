#ifndef KEY_VALUE_DATA_STRUCTURE_H
#define KEY_VALUE_DATA_STRUCTURE_H

#include <unordered_map>
#include <vector>
#include <string>
#include <mutex>

using std::string;
using std::unordered_map;
using std::vector;
using std::mutex;

namespace kvstorage {	
/* 
    Underlying data structure for key value store. Storage class support Put, 
    Get and DeleteKey methods under multithread environment.
*/  
class Storage final {
public:
  // Empty vector<string> used to be returned when key is not found.
  Storage() {}
  // Put given (key, val) pair to Storage. If put successful, return true;
  bool Put(const string& key, const string& val);
  // Get val for given key from storage. If key doesn't exist, return null pointer;
  const vector<string>& Get(const string& key);
  // Delete value of a given key from storage. If key is deleted, return true;
  bool DeleteKey(const string& key);

private:
  static const vector<string> empty_;
  unordered_map<string, vector<string>> storage_map_;
  mutex storage_mutex_;
};
}// End of kvstorage namespace

#endif