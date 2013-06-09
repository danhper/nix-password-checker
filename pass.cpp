#include "password_checker.hpp"

int main(void)
{
    try {
        password_checker checker("foo.conf");
        printf("%d\n", checker.is_group_authorized("users"));
    } catch(libconfig::FileIOException) {
        printf("file not found\n");
    } catch(libconfig::ParseException) {
        printf("invalid format\n");
    } catch(libconfig::ConfigException& e) {
        printf("config error\n");
    }

    return 0;
}
