#ifndef COMMAND_LINE_HELPER_H
#define COMMAND_LINE_HELPER_H

#include <iostream>
#include <string>
#include <vector>
// #include <chrono>
// #include <sstream>
#include <time.h>

#include <glog/logging.h>
#include <grpcpp/grpcpp.h>

#include "../func/func_client_impl.h"
#include "../warble/warble_service_impl.h"

using warble::Timestamp;
using warble::Warble;
using warble::RegisteruserRequest;
using warble::RegisteruserReply;
using warble::WarbleRequest;
using warble::WarbleReply;
using warble::FollowRequest;
using warble::FollowReply;
using warble::ReadRequest;
using warble::ReadReply;
using warble::ProfileRequest;
using warble::ProfileReply;

namespace commandhelper {
const int REGISTER_FLAG = 0;
const int USER_FLAG = 1;
const int WARBLE_FLAG = 2;
const int REPLY_FLAG = 3;
const int FOLLOW_FLAG = 4;
const int READ_FLAG = 5;
const int PROFILE_FLAG = 6;
const int OTHER_FLAG = 7;

// FlagOption is used to store flags information from command line.
// Once we support more funtionalities, it's easy to add
// the flag option into this structure.
struct FlagOption {
  std::string registeruser;
  std::string user;
  std::string warble;
  std::string reply;
  std::string follow;
  std::string read;
  bool profile;
};

// This method is used to print a warble
void PrintWarble(const Warble& warble);

// This method prints username, and corresponding followings
// and followers of the username.
void PrintUser(const std::string& username,
	              const std::vector<std::string>& following,
	              const std::vector<std::string>& followers);

// ParseFlag method is used to parse the command line options
// given by user. It returns the type of handler method for
// dealing with the flag. 
int ParseFlag(FlagOption flag_option);

// PrintOptions method is used to print all possible
// flag options to users.
void PrintOptions();

}

#endif