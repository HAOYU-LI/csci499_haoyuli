#include "key_value_data_structure.h"
#include "gtest/gtest.h"

using kvstorage::Storage;
using std::string;
using std::vector;

class StorageTest : public ::testing::Test {
  protected:
  void SetUp() override {
    storage = new Storage();
  }

  void TearDown() override {
    delete storage;
  }

  Storage* storage;
};

//test Put member function of Storage class.
TEST_F(StorageTest, Put) {
  ASSERT_EQ(true, storage->Put("test", "test"));
}
//test Get member fucntion of Storage class.
TEST_F(StorageTest, GetWithoutInternalError) {
  storage->Put("test", "test");
  const vector<string>* vals = storage->Get("test");
  ASSERT_EQ(vals->size(), 1);
  ASSERT_STREQ("test", vals->at(0).c_str()); 
}
//test Get member function when key does not exist.
TEST_F(StorageTest, GetKeyNotExist) {
  const vector<string>* vals = storage->Get("NOT_EXIST_KEY");
  ASSERT_EQ(vals, nullptr);
}
//test DeleteKey member function of Storage class.
TEST_F(StorageTest, DeleteKeyWithoutInternalError) {
  storage->Put("test", "test");
  ASSERT_EQ(true, storage->DeleteKey("test"));
}
//test DeleteKey member function when key doesn't exist.
TEST_F(StorageTest, DeleteKeyNotExist) {
  ASSERT_EQ(true, storage->DeleteKey("test"));
}

int main(int argc, char* argv[]) {	
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

