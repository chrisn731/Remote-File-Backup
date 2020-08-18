#include <dirent.h>
#include <sys/stat.h>
#include "../include/Progress.h"

#define BAR_LENGTH 30

int non_verbose_progressbar(int files_backed, int total_files)
{
	int barlen = BAR_LENGTH;
	int hashes;

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
		putchar('\n');
	} else {
		putchar('\r');
		fflush(stdout);
	}

	return 0;
}

int verbose_progressbar(const char *filename, int files_backed, int total_files)
{
	if (files_backed <= 0)
		return 1;

	printf(" (%d/%d)", files_backed, total_files);
	printf(" Recieving file: %s...", filename);
	fflush(stdout);

	return 0;
}
