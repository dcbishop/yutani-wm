#include "Globals.hpp"

// Fallback defines
#ifndef BUILD_DATE_
#define BUILD_DATE_ "UNKNOWN"
#endif /* _BUILD_DATE_ */
#ifndef GIT_VERSION_
#define GIT_VERSION_ "UNKNOWN"
#endif /* _GIT_VERSION_ */
#ifndef GIT_SHA1_
#define GIT_SHA1_ "UNKNOWN"
#endif /* _GIT_SHA1_ */
#ifndef BUILD_TYPE_
#define BUILD_TYPE_ "UNKNOWN"
#endif /* BUILD_TYPE_ */

const std::string Globals::build_date_ = BUILD_DATE_;
const std::string Globals::git_version_ = GIT_VERSION_;
const std::string Globals::git_sha1_ = GIT_SHA1_;
const std::string Globals::build_type_ = BUILD_TYPE_;

Globals globals;

const std::string& Globals::getBuildDate() const {
   return build_date_;
}

const std::string& Globals::getGitVersion() const {
   return git_version_;
}

const std::string& Globals::getGitSHA1() const {
   return git_sha1_;
}

const std::string& Globals::getBuildType() const {
   return build_type_;
}
