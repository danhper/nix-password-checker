#include <libconfig.h++>
#include <set>

#include <pwd.h>

class password_checker
{
public:
  password_checker(const char* path);
  ~password_checker();
  bool is_group_authorized(const char* group_name);
  bool is_password_valid(const char* username, const char* password);
  bool user_exists(const char* username);
  bool is_user_authorized(const char* username);

private:
  bool group_needs_authorization;
  bool user_needs_authorization;
  std::set<const char*> authorized_groups;
  std::set<const char*> authorized_users;
  passwd* user;


  passwd* get_user(const char* username);
  std::string get_shadow_line(const char* username);
  std::string extract_hash(const char* shadow_line);
  std::string extract_salt(const char* hash);

  bool generate_authorizations(const libconfig::Config& config,
    const char* key, std::set<const char*>& authorizations);
};
