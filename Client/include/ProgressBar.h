#ifndef PROGBAR_H_G
#define PROGBAR_H_G

#include <dirent.h>
#include <sys/stat.h>
#include "Helper.h"

extern unsigned int num_of_files(const char *path);
extern int verbose_progressbar(const char *filename, long done, long total);
extern int non_verbose_progressbar(int files_backed, int total_files);

#endif /* PROGBAR_H_G */
