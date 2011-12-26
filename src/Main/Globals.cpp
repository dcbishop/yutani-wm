#include "Globals.hpp"

Globals globals;

#ifndef BUILD_DATE
#define BUILD_DATE "UNKNOWN"
#endif /* _BUILD_DATE */
#ifndef GIT_VERSION
#define GIT_VERSION "UNKNOWN"
#endif /* _GIT_VERSION */
#ifndef GIT_SHA1
#define GIT_SHA1 "UNKNOWN"
#endif /* _GIT_SHA1 */

const string Globals::build_date_ = BUILD_DATE;
const string Globals::git_version_ = GIT_VERSION;
const string Globals::git_sha1_ = GIT_SHA1;

Globals g;
