#include "command_line_helper.h"

namespace commandhelper {
// This method is used to print a warble
void PrintWarble(const Warble& warble) {
  LOG(INFO) << "=====================warble=====================" << std::endl;
  LOG(INFO) << "Username : " << warble.username() << std::endl;
  LOG(INFO) << "Text : " << warble.text() << std::endl;
  LOG(INFO) << "Warble id : " << warble.id() << std::endl;
  if (warble.parent_id().length() > 0) {
    LOG(INFO) << "Parent of this warble has id : " << warble.parent_id() << std::endl;
  }
  LOG(INFO) << "=====================warble=====================" << std::endl;
  return;
}

// This method prints username, and corresponding followings
// and followers of the username.
void PrintUser(const std::string& username,
	              const std::vector<std::string>& following,
	              const std::vector<std::string>& followers) {
	LOG(INFO) << "=====================user=====================" << std::endl;
  LOG(INFO) << "User : " << username << " is following : ";
  for (std::string following_user : following) {
    LOG(INFO) << following_user << " ";
  }
  
  LOG(INFO) << "User : " << username << " is followed by :";
  for (std::string follower : followers) {
    LOG(INFO) << follower << " ";
  }
  LOG(INFO) << "=====================user=====================" << std::endl;
}

// ParseFlag method is used to parse the command line options
// given by user. It returns the type of handler method for
// dealing with the flag. 
int ParseFlag(FlagOption flag_option) {
  bool registeruser = flag_option.registeruser.length() != 0;
  bool user = flag_option.user.length() != 0;
  bool warble = flag_option.warble.length() != 0;
  bool reply = flag_option.reply.length() != 0;
  bool follow = flag_option.follow.length() != 0;
  bool read = flag_option.read.length() != 0;

  if (registeruser && !user && !warble && !reply && !follow && !read && !flag_option.profile) {
    return REGISTER_FLAG;
  }
  if (!registeruser && user && warble && !reply && !follow && !read && !flag_option.profile) {
    return WARBLE_FLAG;
  }
  if (!registeruser && user && warble && reply && !follow && !read && !flag_option.profile) {
    return REPLY_FLAG;
  }
  if (!registeruser && user && !warble && !reply && follow && !read && !flag_option.profile) {
    return FOLLOW_FLAG;
  }
  if (!registeruser && user && !warble && !reply && !follow && read && !flag_option.profile) {
    return READ_FLAG;
  }
  if (!registeruser && user && !warble && !reply && !follow && !read && flag_option.profile) {
    return PROFILE_FLAG;
  }
  
  return OTHER_FLAG;
}

// PrintOptions method is used to print all possible
// flag options to users.
void PrintOptions() {
  LOG(INFO) << "Commands are invalid. ";
  LOG(INFO) << "Valid flag options are as following : " << std::endl;
  LOG(INFO) << "Register a user : ./warble --registeruser <USER_NAME>" << std::endl;
  LOG(INFO) << "Post a warble : ./warble --user <USER_NAME> --warble <WARBLE_TEXT>" << std::endl;
  LOG(INFO) << "Reply a warble : ./warble --user <USER_NAME> --warble <WARBLE_TEXT> --reply <WARBLE_ID>" << std::endl;
  LOG(INFO) << "Follow a user : ./warble --user <USER_NAME> --follow <USER_NAME_TO_FOLLOW>" << std::endl;
  LOG(INFO) << "Read a warble thread : ./warble --user <USER_NAME> --read <WARBLE_ID>" << std::endl;
  LOG(INFO) << "Show profile of a user : ./warble --user <USER_NAME> --profile" << std::endl;
  return;
}

}