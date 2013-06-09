#include <stdio.h>
#include <unistd.h>
#include "password_checker.hpp"

int main(int argc, char* argv[])
{
  if(argc <= 2) {
    printf("usage: ./nix-password-checker username password\n");
    return 9;
  }

  if(getuid() != 0) {
    printf("this program must be ran as root\n");
    return 8;
  }

  const char* username = argv[1];
  const char* password = argv[2];

  try {
    password_checker checker("../conf/application.cfg");
    if(!checker.user_exists(username)) {
      return 3;
    } else if(!checker.is_user_authorized(username)) {
      return 2;
    } else if(!checker.is_password_valid(username, password)) {
      return 1;
    }
  } catch(libconfig::ParseException) {
    return 7;
  } catch(libconfig::ConfigException& e) {
    return 6;
  }

  return 0;
}
