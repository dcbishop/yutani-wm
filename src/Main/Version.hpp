#ifndef VERSION_HPP_
#define VERSION_HPP_

#ifndef BUILD_DATE
#define BUILD_DATE "UNKNOWN"
#endif /* BUILD_DATE */
#ifndef GIT_VERSION
#define GIT_VERSION "UNKNOWN"
#endif /* GIT_VERSION */
#ifndef GIT_SHA1
#define GIT_SHA1 "UNKNOWN"
#endif /* GIT_SHA1 */

#include <string>
using namespace std;

extern const string g_build_date;
extern const string g_git_version;
extern const string g_git_sha1;

#endif /* VERSION_HPP_ */
