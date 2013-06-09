#include "password_checker.hpp"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <cstring>
#include <cstdlib>
#include <grp.h>
#include <crypt.h>

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
  user = NULL;
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

passwd* password_checker::get_user(const char* username)
{
  if(user == NULL || strcmp(username, user->pw_name) != 0)
  {
    user = getpwnam(username);
  }
  return user;
}

bool password_checker::user_exists(const char* username)
{
  return get_user(username) != NULL;
}

std::string password_checker::get_shadow_line(const char* username)
{
  std::string line;
  std::ifstream shadow_file("/etc/shadow");
  if(!shadow_file.is_open()) {
    return "";
  }
  while(shadow_file.good()) {
    getline(shadow_file, line);
    if(strncmp(line.c_str(), username, strlen(username)) == 0) {
      return line;
    }
  }
  return "";
}

std::string password_checker::extract_hash(const char* shadow_line)
{
  std::string hash;
  const char* p = shadow_line;
  while(*p++ != ':');
  while(*p != ':') {
    hash += *p++;
  }
  return hash;
}

std::string password_checker::extract_salt(const char* hash)
{
  std::string salt;
  const char* p = hash;
  // when no crypt algorithm is set
  if(*p != '$') {
    while(*p != '$') {
      salt += *p++;
    }
  } else {
    int c = 0;
    while(c < 3) {
      if(*p == '$') {
        c++;
      }
      salt += *p++;
    }
  }
  return salt;
}

bool password_checker::is_password_valid(const char* username, const char* password)
{
  std::string shadow_line = get_shadow_line(username);
  if(shadow_line.empty()) {
    return false;
  }
  std::string hash = extract_hash(shadow_line.c_str());
  std::string salt = extract_salt(hash.c_str());
  const char* crypted_password = crypt(password, salt.c_str());
  return strncmp(hash.c_str(), crypted_password, shadow_line.length()) == 0;
}

bool password_checker::is_user_authorized(const char* username)
{
  passwd* u = get_user(username);
  group* g = getgrgid(u->pw_gid);
  if(!is_group_authorized(g->gr_name)) {
    return false;
  }
  if(!user_needs_authorization) {
    return true;
  }

  std::set<const char*>::iterator it;
  for(it = authorized_users.begin(); it != authorized_users.end(); ++it) {
    if(strcmp(username, *it) == 0) {
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
