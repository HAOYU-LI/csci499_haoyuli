#ifndef COMMAND_LINE_H
#define COMMAND_LINE_H
#include <string>
#include <iostream>
#include <chrono>
#include <thread>

#include <gflags/gflags.h>
#include <glog/logging.h>
#include <grpcpp/grpcpp.h>

#include "../func/func_client_impl.h"
#include "command_line_helper.h"

using func::ClientEventParams;
using func::ClientEventReply;
using func::FuncClientImpl;
using grpc::Server;
using grpc::ServerContext;
using grpc::ServerReaderWriter;
using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using grpc::StatusCode;
using commandhelper::PrintWarble;
using commandhelper::PrintUser;
using commandhelper::FlagOption;
using commandhelper::ParseFlag;
using commandhelper::PrintOptions;
using commandhelper::REGISTER_FLAG;
using commandhelper::USER_FLAG;
using commandhelper::WARBLE_FLAG;
using commandhelper::REPLY_FLAG;
using commandhelper::FOLLOW_FLAG;
using commandhelper::READ_FLAG;
using commandhelper::PROFILE_FLAG;
using commandhelper::OTHER_FLAG;
using commandhelper::STREAM_FLAG;


namespace commandline {
/*
  This class is used to parse
*/
class CommandLineClient {
public:
	 // Constructor of CommandLineClient
	 CommandLineClient();

  // Deconstructor of CommandLineClient
  ~CommandLineClient() { delete func_client; }
  
  // Handle user registration call
  bool RegisterHandler(std::string username);

  // Handle warble call from command line. If warble_id is not empty, warble is a reply warble. 
  bool WarbleHandler(std::string username, std::string warble_text, std::string parent_warble_id = "");

  // Handle follow request from command line.
  bool FollowHandler(std::string username, std::string username_to_follow);

  // Handle read request from command line.
  bool ReadHandler(std::string username, std::string warble_id);

  // Handle profile request from command line.
  bool ProfileHandler(std::string username);

  // Handles parsing raw hashtag string
  std::vector<std::string> ParseHashtag(std::string raw_hashtag);

  // Handle warble streaming
  bool StreamHandler(std::string username, std::string hashtag);

  // Handles updating stream with new warbles
  void CheckStream(std::string hashtag);

private:
  FuncClientImpl* func_client;

  const static int REGISTER_TYPE { 0 };
  const static int WARBLE_TYPE { 1 };
  const static int FOLLOW_TYPE { 2 };
  const static int READ_TYPE { 3 };
  const static int PROFILE_TYPE { 4 };
  const static int STREAM_TYPE { 5 };
};

} // namespace commandline

#endif