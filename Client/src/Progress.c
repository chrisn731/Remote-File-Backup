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
	while (barlen-- > 0) {
		if (hashes-- != 0)
			putchar('#');
		else
			putchar('-');
	}
	putchar(']');
	printf(" (%d/%d)", files_backed, total_files);
	putchar('\r');
	fflush(stdout);

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

int progress_bar(long done, long total)
{
	int barlen = 20;
	int hashes = barlen * ((float) done / total);

	fputs(" [", stdout);
	while (barlen-- > 0) {
		if (hashes > 0) {
			fputc('#', stdout);
			hashes--;
		} else {
			fputc('-', stdout);
		}
	}
	fputc(']', stdout);
	fputc('\r', stdout);

	return 0;
}
