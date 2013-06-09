#include "password_checker.hpp"

int main(void)
{
  try {
    password_checker checker("../conf/application.cfg");
    printf("%d\n", checker.is_group_authorized("users"));
  } catch(libconfig::FileIOException) {
    printf("configuration file not found\n");
  } catch(libconfig::ParseException) {
    printf("invalid configuration file format\n");
  } catch(libconfig::ConfigException& e) {
    printf("config error\n");
  }

  return 0;
}
