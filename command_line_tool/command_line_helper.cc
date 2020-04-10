#include "command_line_helper.h"

namespace commandhelper {
// This method is used to print a warble
void PrintWarble(const Warble& warble) {
  std::cout << "=====================warble=====================" << std::endl;
  std::cout << "Username : " << warble.username() << std::endl;
  std::cout << "Text : " << warble.text() << std::endl;
  std::cout << "Warble id : " << warble.id() << std::endl;
  if (warble.parent_id().length() > 0) {
    std::cout << "This warble replies to Parent id : " << warble.parent_id() << std::endl;
  }
  std::string seconds = std::to_string(warble.timestamp().seconds());
  std::string useconds = std::to_string(warble.timestamp().useconds());
  time_t time = atoi((seconds + ":" + useconds).c_str());
  std::cout << "This warble is posted on : " << asctime(localtime(&time));
  std::cout << "=====================warble=====================" << std::endl;
  return;
}

// This method prints username, and corresponding followings
// and followers of the username.
void PrintUser(const std::string& username,
	              const std::vector<std::string>& following,
	              const std::vector<std::string>& followers) {
	std::cout << "=====================user=====================" << std::endl;
  std::cout << "User : " << username << " is following : ";
  for (std::string following_user : following) {
    std::cout << following_user << " ";
  }
  std::cout << std::endl;
  std::cout << "User : " << username << " is followed by :";
  for (std::string follower : followers) {
    std::cout << follower << " ";
  }
  std::cout << std::endl;
  std::cout << "=====================user=====================" << std::endl;
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
  std::cout << "Commands are invalid. ";
  std::cout << "Valid flag options are as following : " << std::endl;
  std::cout << "Register a user : ./warble --registeruser <USER_NAME>" << std::endl;
  std::cout << "Post a warble : ./warble --user <USER_NAME> --warble <WARBLE_TEXT>" << std::endl;
  std::cout << "Reply a warble : ./warble --user <USER_NAME> --warble <WARBLE_TEXT> --reply <WARBLE_ID>" << std::endl;
  std::cout << "Follow a user : ./warble --user <USER_NAME> --follow <USER_NAME_TO_FOLLOW>" << std::endl;
  std::cout << "Read a warble thread : ./warble --user <USER_NAME> --read <WARBLE_ID>" << std::endl;
  std::cout << "Show profile of a user : ./warble --user <USER_NAME> --profile" << std::endl;
  return;
}

}