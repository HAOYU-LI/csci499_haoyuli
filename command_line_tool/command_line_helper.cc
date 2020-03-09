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
  LOG(INFO) << "User : " << username << "is following : [";
  for (std::string following_user : following) {
    LOG(INFO) << following_user << " ";
  }
  LOG(INFO) << "]" << std::endl;
  
  LOG(INFO) << "User : " << username << "is followed by : [";
  for (std::string follower : followers) {
    LOG(INFO) << follower << " ";
  }
  LOG(INFO) << "]" << std::endl;
  LOG(INFO) << "=====================user=====================" << std::endl;
}

}