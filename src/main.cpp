#include <stdio.h>
#include "password_checker.hpp"

int main(int argc, char* argv[])
{
  if(argc <= 2) {
    printf("usage: ./nix-password-checker username password\n");
    return 9;
  }

  const char* username = argv[1];
  const char* password = argv[2];

  try {
    password_checker checker("../conf/application.cfg");
    if(!checker.user_exists(username)) {
      return 3;
    } else if(!checker.is_user_authorized(username)) {
      return 2;
    }
  } catch(libconfig::ParseException) {
    return 8;
  } catch(libconfig::ConfigException& e) {
    return 7;
  }

  return 0;
}
