#include "password_checker.hpp"
#include <algorithm>
#include <iterator>
#include <cstring>

using namespace libconfig;

password_checker::password_checker(const char* path)
{
  Config config;
  config.readFile(path);
  generate_authorized_groups(config);
}

password_checker::~password_checker()
{
  authorized_groups.clear();
}

void password_checker::generate_authorized_groups(const Config& config)
{
  if(config.exists("application.authorized-groups")) {
    Setting& auth_groups = config.lookup("application.authorized-groups");
    if(!auth_groups.isArray()) {
      throw new ConfigException();
    } else {
      for(int i = 0; i < auth_groups.getLength(); i++) {
        char* s = new char[strlen(auth_groups[i].c_str()) + 1];
        strcpy(s, auth_groups[i].c_str());
        authorized_groups.insert(s);
      }
    }
    group_needs_authorization = true;
  } else {
    group_needs_authorization = false;
  }
}

bool password_checker::is_group_authorized(const char* group_name)
{
  if(!group_needs_authorization) {
    return true;
  }
  std::set<const char*>::iterator it;
  for(it = authorized_groups.begin(); it != authorized_groups.end(); ++it) {
    if(strcmp(group_name, *it) == 0) {
      return true;
    }
  }

  return false;
}
