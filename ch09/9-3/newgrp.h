#ifndef NG_NEWGRP_H
#define NG_NEWGRP_H

// for gid_t
#include <sys/types.h>

extern int ng_initgroups(const char * user, gid_t group);

#endif

