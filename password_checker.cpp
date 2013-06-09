#include "password_checker.hpp"
#include <algorithm>

using namespace libconfig;

password_checker::password_checker(const char* path)
{
  Config config;
  config.readFile(path);
  generate_authorized_groups(config);
}

void password_checker::generate_authorized_groups(const Config& config)
{
  if(config.exists("app.authorized_groups")) {
    Setting& auth_groups = config.lookup("app.authorized_groups");
    if(!auth_groups.isArray()) {
      throw new ConfigException();
    } else {
      for(int i = 0; i < auth_groups.getLength(); i++) {
        authorized_groups.insert(auth_groups[i].c_str());
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
  std::set<const char*>& v = authorized_groups;

  return std::find(v.begin(), v.end(), group_name) != v.end();
}

