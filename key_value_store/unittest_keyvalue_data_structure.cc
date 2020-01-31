#include "keyvalue_data_structure.h"

using kvstorage::Storage;
using std::string;


TEST(Storage, Put) {
	const Storage storage_;
	ASSERT_EQ(true, storage_.put("test", "test"));
}

TEST(Storage, Get) {
	const Storage storage_;
	storage_.put("test", "test");
	ASSERT_STREQ("test", storage_.get("test")); 
}

TEST(Storage, DeleteKey) {
	const Storage storage_;
	storage_.put("test", "test");
	ASSERT_EQ(true, storage_.DeleteKey("test"));
}

int main(int argc, char* argv[]) {	
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

