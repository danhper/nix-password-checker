#include <libconfig.h++>
#include <set>

class password_checker
{
public:
  password_checker(const char* path);
  ~password_checker();
  bool is_group_authorized(const char* group_name);

private:
  std::set<const char*> authorized_groups;
  bool group_needs_authorization;
  void generate_authorized_groups(const libconfig::Config& config);
};
