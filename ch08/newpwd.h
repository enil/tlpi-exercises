#ifndef NP_NEWPWD_H
#define NP_NEWPWD_H

// for struct passwd
#include <pwd.h>

extern struct passwd * np_getpwnam(const char * name);

#endif

