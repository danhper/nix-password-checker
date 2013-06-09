#include "password_checker.hpp"

#include <algorithm>
#include <iterator>
#include <cstring>
#include <cstdlib>
#include <grp.h>

using namespace libconfig;

password_checker::password_checker(const char* path)
{
  Config config;
  try {
    config.readFile(path);
    group_needs_authorization = generate_authorizations(config,
      "application.authorized-groups", authorized_groups);
    user_needs_authorization = generate_authorizations(config, "application.authorized-users", authorized_users);
  } catch(FileIOException& e){
    group_needs_authorization = false;
    user_needs_authorization = false;
  }
}

password_checker::~password_checker()
{
  authorized_groups.clear();
}

bool password_checker::generate_authorizations(const Config& config,
  const char* key, std::set<const char*>& authorizations)
{
  if(config.exists(key)) {
    Setting& auths = config.lookup(key);
    if(!auths.isArray()) {
      throw new ConfigException();
    } else {
      for(int i = 0; i < auths.getLength(); i++) {
        char* s = new char[strlen(auths[i].c_str()) + 1];
        strcpy(s, auths[i].c_str());
        authorizations.insert(s);
      }
    }
    return true;
  } else {
    return false;
  }

}

passwd* password_checker::get_user(const char* user_name)
{
  if(user == NULL || strcmp(user_name, user->pw_name) != 0)
  {
    user = getpwnam(user_name);
  }
  return user;
}

bool password_checker::user_exists(const char* user_name)
{
  return get_user(user_name) != NULL;
}

bool password_checker::is_password_valid(const char* user_name, const char* password)
{
  passwd* u = get_user(user_name);

}

bool password_checker::is_user_authorized(const char* user_name)
{
  passwd* u = get_user(user_name);
  group* g = getgrgid(u->pw_gid);
  if(!is_group_authorized(g->gr_name)) {
    return false;
  }
  if(!user_needs_authorization) {
    return true;
  }

  std::set<const char*>::iterator it;
  for(it = authorized_users.begin(); it != authorized_users.end(); ++it) {
    if(strcmp(user_name, *it) == 0) {
      return true;
    }
  }

  return false;
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
