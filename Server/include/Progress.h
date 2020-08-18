#ifndef SERVBAR_H_G
#define SERVBAR_H_G

extern int non_verbose_progressbar(int files_backed, int total_files);
extern int verbose_progressbar(const char *filename, long done, long total);

#endif
