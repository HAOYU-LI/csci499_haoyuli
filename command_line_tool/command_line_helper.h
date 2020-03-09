#ifndef COMMAND_LINE_HELPER_H
#define COMMAND_LINE_HELPER_H

#include <iostream>
#include <string>
#include <vector>

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
// This method is used to print a warble
void PrintWarble(const Warble& warble);

// This method prints username, and corresponding followings
// and followers of the username.
void PrintUser(const std::string& username,
	              const std::vector<std::string>& following,
	              const std::vector<std::string>& followers);

}

#endif