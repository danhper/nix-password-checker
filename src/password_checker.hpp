#include <libconfig.h++>
#include <set>

#include <pwd.h>

class password_checker
{
public:
  password_checker(const char* path);
  ~password_checker();
  bool is_group_authorized(const char* group_name);
  bool is_password_valid(const char* user_name, const char* password);
  bool user_exists(const char* user_name);
  bool is_user_authorized(const char* user_name);

private:
  std::set<const char*> authorized_groups;
  std::set<const char*> authorized_users;
  passwd* get_user(const char* user_name);
  passwd* user;
  bool group_needs_authorization;
  bool user_needs_authorization;

  bool generate_authorizations(const libconfig::Config& config,
    const char* key, std::set<const char*>& authorizations);
};
