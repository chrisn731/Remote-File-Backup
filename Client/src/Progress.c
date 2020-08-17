#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include "../../Shared/Helper.h"
#include "../include/ProgressBar.h"


#define BAR_LENGTH 20

static inline int file_to_count(const char *filename)
{
	return (strcmp(filename, ".") &&
		strcmp(filename, ".."));
}

int non_verbose_progressbar(int files_backed, int total_files)
{
	unsigned int barlen = BAR_LENGTH;
	unsigned int hashes = barlen * ((float) files_backed/total_files);

	putchar('[');
	while (barlen-- != 0) {
		if (hashes-- != 0)
			putchar('#');
		else
			putchar('-');
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
			if (entry->d_type == DT_DIR)
				total += num_of_files(entry->d_name);
			else
				total++;
		}
	}
	closedir(dr);
	return total;
}

int verbose_progressbar(const char *fname, long done, long total)
{
	int barlen = BAR_LENGTH;
	int hashes = barlen * ((float) done / total);
	int percent_done = 100 * ((float) done / total);

	printf("Backing up %s\t", fname);

	fputs(" [", stdout);
	while (barlen-- != 0) {
		if (hashes-- != 0)
			putchar('#');
		else
			fputc('-', stdout);
	}
	putchar(']');
	printf(" %d%%", percent_done);
	fputc('\r', stdout);
	fflush(stdout);

	return 0;
}
