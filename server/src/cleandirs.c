/* Needed to access nftw function */
#define _XOPEN_SOURCE 500
#include <ftw.h>
#include <stdio.h>

#include "../include/cleandirs.h"
#include "../include/helper.h"
#include "../include/main.h"

/* Used to supress unused paramater warnings */
#define UNUSED(x) (void)(x)

/* For each file passed into this function, it will be deleted */
static int unlink_cb(const char *fpath, const struct stat *sb,
		     int typeflag, struct FTW *ftwbuf)
{
	int rv;
	UNUSED(sb);
	UNUSED(typeflag);
	UNUSED(ftwbuf);

	if (verbose)
		v_log("Attempting to remove: %s", fpath);
	if ((rv = remove(fpath)))
		die("Error removing: %s", fpath);
	return rv;
}

/*
 * Given a path, will delete all files located in the path,
 * and the path name from the directory.
 */
int CleanDirectory(const char *path)
{
	if (verbose)
		v_log("Attempting to delete dir: %s", path);
	nftw(path, unlink_cb, 64, FTW_DEPTH | FTW_PHYS);
	return 0;
}
