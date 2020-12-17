#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "../include/helper.h"
#include "../include/progress.h"

#define NONV_BAR_LENGTH 50
#define VERBOSE_BAR_LENGTH 30
/*
 * Number of characters to print of the file.
 * Useful so if we have a file that is really long it doesnt just fill up the screen.
 */
#define NUM_CHARS_TO_PRINT 29

static inline int file_to_count(const char *filename)
{
	return (strcmp(filename, ".") &&
		strcmp(filename, ".."));
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
 * Function only called in non verbose procedure of program.
 * Output: [###---] (Completed/Total)
 */
void non_verbose_progressbar(int files_backed, int total_files)
{
	unsigned int hashes, barlen = NONV_BAR_LENGTH;

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
 * Function only called in verbose procedure of program.
 * Output: (File being backed up) [###---] (Percentage Complete)
 */
void verbose_progressbar(const char *fname, long amt_done, long total)
{
	unsigned int hashes, percent_done, barlen = VERBOSE_BAR_LENGTH;
	unsigned int fname_len;

	if (amt_done) {
		hashes = barlen * ((float) amt_done / total);
		percent_done = 100 * ((float) amt_done / total);
	} else {
		/* This case will only be triggered for empty files */
		if (amt_done == total) {
			hashes = barlen;
			percent_done = 100;
		} else {
			hashes = 0;
			percent_done = 0;
		}
	}

	printf(" Backing up ");
	printf("%.*s", NUM_CHARS_TO_PRINT, fname);
	if ((fname_len = strlen(fname)) < NUM_CHARS_TO_PRINT)
		printf("%*s", NUM_CHARS_TO_PRINT - fname_len, "");

	printf("  [");
	while (barlen--) {
		if (hashes) {
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
