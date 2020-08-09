#include "../include/CleanDirs.h"


/* For each file passed into this function, it will be deleted */
static int unlink_cb(const char *fpath, const struct stat *sb,
		     int typeflag, struct FTW *ftwbuf)
{
	if (verbose)
		printf("Attempting to remove: %s\n", fpath);

	return remove(fpath);
}

/*
 * Given a path, will delete all files located in the path,
 * and the path name from the directory.
 */
int CleanDirectory(const char *path)
{
	nftw(path, unlink_cb, 64, FTW_DEPTH | FTW_PHYS);

	return 0;
}
