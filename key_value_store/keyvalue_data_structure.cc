#include "keyvalue_data_structure.h"

namespace kvstorage{

bool Storage::Put(const string& key, const string& val) {
    std::lock_guard<std::mutex> lock(storage_mutex);
    storage_map.insert({key, val});
    unordered_map<string, string>::const_iterator got = storage_map.find(key);
    return got == storage_map.end() || val.compare(got->second) != 0 ? false : true;
}

const string& Storage::Get(const string& key) {
	std::lock_guard<std::mutex> lock(storage_mutex);
	unordered_map<string, string>::const_iterator got = storage_map.find(key);
	return got == storage_map.end() ? "" : got->second;
}

bool Storage::Deletekey(const string& key) {
	std::lock_guard<std::mutex> lock(storage_mutex);
	if (storage_map.find(key) == storage_map.end() || 
		storage_map.erase(key) == 1) {
		return true;
	} else {
		return false;
	}
}

}