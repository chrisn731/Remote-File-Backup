#ifndef SERVBAR_H_G
#define SERVBAR_H_G

extern void non_verbose_progressbar(int files_backed, int total_files);
extern void verbose_progressbar(const char *fname, int files_backed, int total_files);

#endif
