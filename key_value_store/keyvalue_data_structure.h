#ifndef KEYVALUE_DATA_STRUCTURE_H
#define KEYVALUE_DATA_STRUCTURE_H

#include <unordered_map>
#include <string>
#include <mutex>

using std::string;
using std::unordered_map;
using std::mutex;

namespace kvstorage {	

class Storage final{
 unordered_map<string, string> storage_map;
 mutex storage_mutex;
 public:
  //put given (key, val) pair to Storage. If put successful, return true else return false;
  bool Put(const string& key, const string& val);
  //get val for given key from storage. If key doesn't exist, return empty string;
  const string& Get(const string& key);
  // delete value of a given key from storage. If key doesn't exist, return false, else true;
  bool Deletekey(const string& key);
};

}

#endif