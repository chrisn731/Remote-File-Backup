#ifndef _CLEANDIRS_H_
#define _CLEANDIRS_H_

/* Needed to acces function nftw */
#define _XOPEN_SOURCE 500

#include <ftw.h>
#include "Helper.h"

extern int CleanDirectory(const char *path);

#endif /* _CLEANDIRS_H_ */
