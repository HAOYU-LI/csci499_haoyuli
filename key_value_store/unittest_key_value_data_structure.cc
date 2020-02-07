#include "key_value_data_structure.h"
#include "gtest/gtest.h"

using kvstorage::Storage;
using std::string;

//test Put member function of Storage class.
TEST(Storage, Put) {
  Storage storage_;
  ASSERT_EQ(true, storage_.Put("test", "test"));
}
//test Get member fucntion of Storage class.
TEST(Storage, Get) {
  Storage storage_;
  storage_.Put("test", "test");
  ASSERT_STREQ("test", storage_.Get("test").c_str()); 
}
//test DeleteKey member function of Storage class.
TEST(Storage, DeleteKey) {
  Storage storage_;
  storage_.Put("test", "test");
  ASSERT_EQ(true, storage_.DeleteKey("test"));
}

int main(int argc, char* argv[]) {	
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

