#ifndef COMMAND_LINE_H
#define COMMAND_LINE_H
#include <string>

#include <gflags/gflags.h>
#include <glog/logging.h>
#include <grpcpp/grpcpp.h>

#include "../func/func_client_impl.h"

using func::ClientEventParams;
using func::ClientEventReply;
using func::FuncClientImpl
using grpc::Server;
using grpc::ServerContext;
using grpc::ServerReaderWriter;
using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using grpc::StatusCode;

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
  Status RegisterHandler(std::string username);

  // Handle warble call from command line. If warble_id is not empty, warble is a reply warble. 
  Status WarbleHandler(std::string username, std::string warble_text, std::warble_id = "");

  // Handle follow request from command line.
  Status FollowHandler(std::string username, std::string username_to_follow);

  // Handle read request from command line.
  Status ReadHandler(std::string username, std::string warble_id);

  // Handle profile request from command line.
  Status ProfileHandler(std::string username);

private:
  FuncClientImpl* func_client;
};

}

#endif