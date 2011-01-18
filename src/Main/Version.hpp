#ifndef VERSION_HPP_
#define VERSION_HPP_

#ifndef _BUILD_DATE
#define _BUILD_DATE "UNKNOWN"
#endif /* _BUILD_DATE */
#ifndef _GIT_VERSION
#define _GIT_VERSION "UNKNOWN"
#endif /* _GIT_VERSION */
#ifndef _GIT_SHA1
#define _GIT_SHA1 "UNKNOWN"
#endif /* _GIT_SHA1 */

#include <string>
using namespace std;

extern const string g_build_date;
extern const string g_git_version;
extern const string g_git_sha1;

#endif /* VERSION_HPP_ */
