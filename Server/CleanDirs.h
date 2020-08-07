#ifndef _CLEANDIRS_H_
#define _CLEANDIRS_H_

#define _XOPEN_SOURCE 500
#include <ftw.h>

static int unlink_cb(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf);
int CleanDirectory(const char *path);

#endif /* _CLEANDIRS_H_ */
