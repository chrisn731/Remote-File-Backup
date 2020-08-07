#include "CleanDirs.h"
#include <unistd.h>
#include <stdio.h>


static int unlink_cb(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
	printf("Attempting to remove: %s\n", fpath);
	int rv = remove(fpath);

	return rv;
}

int CleanDirectory(const char *path)
{
	nftw(path, unlink_cb, 64, FTW_DEPTH | FTW_PHYS);

	return 0;
}
