#ifndef _CLEANDIRS_H_
#define _CLEANDIRS_H_

/* Needed to acces function nftw */
#define _XOPEN_SOURCE 500

#include <ftw.h>
#include "../../Shared/Helper.h"

extern int verbose;

extern int CleanDirectory(const char *path);

#endif /* _CLEANDIRS_H_ */
