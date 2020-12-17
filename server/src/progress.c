#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>

#include "../include/progress.h"
#include "../include/main.h"

#define BAR_LENGTH 30

void non_verbose_progressbar(int files_backed, int total_files)
{
	int hashes, barlen = BAR_LENGTH;

	if (files_backed < 1)
		hashes = 0;
	else
		hashes = barlen * ((double) files_backed / total_files);

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

void verbose_progressbar(const char *filename, int files_backed, int total_files)
{
	printf(" (%d/%d)", files_backed, total_files);
	printf(" Recieving file: %s...", filename);
	fflush(stdout);
}
