#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include "../../Shared/Helper.h"
#include "../include/ProgressBar.h"


#define BAR_LENGTH 30

static inline int file_to_count(const char *filename)
{
	return (strcmp(filename, ".") &&
		strcmp(filename, ".."));
}

/*
 * Function only called in non verbose procedure of program.
 * Output: [###---] (Completed/Total)
 */
int non_verbose_progressbar(int files_backed, int total_files)
{
	unsigned int barlen = BAR_LENGTH;
	unsigned int hashes;

	if (files_backed <= 0)
		hashes = 0;
	else
		hashes = barlen * ((float) files_backed / total_files);

	fputs(" [", stdout);
	while (barlen-- != 0) {
		if (hashes > 0) {
			putchar('#');
			hashes--;
		} else {
			putchar('-');
		}
	}
	putchar(']');
	printf(" (%d/%d)", files_backed, total_files);

	if (files_backed == total_files) {
		printf("\n");
	} else {
		putchar('\r');
		fflush(stdout);
	}

	return 0;
}

/*
 * Goes through directory and subdirectories recursively
 * returning amount of files counted up NOT including directories.
 */
unsigned int num_of_files(const char *path)
{
	DIR *dr;
	struct dirent *entry;
	unsigned int total = 0;

	if (!(dr = opendir(path)))
		die("Could not open directory: %s", path);
	if (chdir(path))
		die("Error changing directory to: %s", path);

	while ((entry = readdir(dr)) != NULL) {
		if (file_to_count(entry->d_name)) {
			if (entry->d_type == DT_DIR) {
				total += num_of_files(entry->d_name);
				chdir("..");
			} else {
				total++;
			}
		}
	}
	closedir(dr);
	return total;
}

/*
 * Function only called in verbose procedure of program.
 * Output: (File being backed up) [###---] (Percentage Complete)
 */
int verbose_progressbar(const char *fname, long done, long total)
{
	int barlen, hashes, percent_done;

	barlen = BAR_LENGTH;
	if (done < 1) {
		/* This case will only be triggered for empty files */
		if (done == total) {
			hashes = barlen;
			percent_done = 100;
		} else {
			hashes = 0;
			percent_done = 0;
		}
	} else {
		hashes = barlen * ((float) done / total);
		percent_done = 100 * ((float) done / total);
	}

	printf(" Backing up %-130s\t", fname);
	fputs(" [", stdout);
	while (barlen-- > 0) {
		if (hashes > 0) {
			putchar('#');
			hashes--;
		} else {
			putchar('-');
		}
	}
	putchar(']');
	printf(" %d%%", percent_done);
	fputc('\r', stdout);
	fflush(stdout);

	return 0;
}
