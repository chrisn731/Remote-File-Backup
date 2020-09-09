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
void non_verbose_progressbar(int files_backed, int total_files)
{
	int barlen = BAR_LENGTH;
	int hashes;

	if (files_backed > 0)
		hashes = barlen * ((double) files_backed / total_files);
	else
		hashes = 0;

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

	if (files_backed != total_files) {
		putchar('\r');
		fflush(stdout);
	} else {
		putchar('\n');
	}
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
void verbose_progressbar(const char *fname, long done, long total)
{
	int barlen, hashes, percent_done;

	barlen = BAR_LENGTH;
	if (done > 0) {
		hashes = barlen * ((float) done / total);
		percent_done = 100 * ((float) done / total);
	} else {
		/* This case will only be triggered for empty files */
		if (done == total) {
			hashes = barlen;
			percent_done = 100;
		} else {
			hashes = 0;
			percent_done = 0;
		}
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
	putchar('\r');
	fflush(stdout);
}
