#include "warble_service_impl.h"

#include <grpcpp/grpcpp.h>
#include <glog/logging.h>

#include "gtest/gtest.h"

using warble::WarbleService;
using kvstore::KeyValueClient;
using grpc::Status;

class WarbleServiceTest : public ::testing::Test {
protected:
  void SetUp() override {
    kvclient = new KeyValueClient(grpc::CreateChannel("0.0.0.0:50001",
                           grpc::InsecureChannelCredentials()));
  }

  void TearDown() override {
    delete kvclient;
  }

  KeyValueClient* kvclient;
};

// Test RegisterUser member function of WarbleService class.
TEST_F(WarbleServiceTest, RegisterUserStatusCheck) {
  RegisteruserRequest register_user_request;
  register_user_request.set_username("test_username");
  RegisteruserReply register_user_reply;
  Status status = WarbleService::RegisterUser(&register_user_request, 
                                              &register_user_reply, kvclient);
  ASSERT_EQ(true, status.ok());
}

// Test WarbleMethod member function of WarbleService class.
TEST_F(WarbleServiceTest, WarbleMethodStatusCheck) {
  WarbleRequest warble_request;
  warble_request.set_username("test_username");
  warble_request.set_text("test_text");
  WarbleReply warble_reply;
  Status status = WarbleService::WarbleMethod(&warble_request, 
                                              &warble_reply, kvclient);
  ASSERT_EQ(true, status.ok());
}

// Test Follow member function of WarbleService class.
TEST_F(WarbleServiceTest, FollowStatusCheck) {
  FollowRequest follow_request;
  follow_request.set_username("test_username");
  follow_request.set_to_follow("test_username1");
  FollowReply follow_reply;
  Status status = WarbleService::Follow(&follow_request,
                                        &follow_reply, kvclient);
  ASSERT_EQ(true, status.ok());
}

// Test Profile member function of WarbleService class.
TEST_F(WarbleServiceTest, ProfileStatusCheck) {
  ProfileRequest profile_request;
  profile_request.set_username("test_username");
  ProfileReply profile_reply;
  Status status = WarbleService::Profile(&profile_request, &profile_reply, kvclient);

  ASSERT_EQ(true, status.ok());
}

int main(int argc, char* argv[]) {
  google::InitGoogleLogging(argv[0]); 	
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}