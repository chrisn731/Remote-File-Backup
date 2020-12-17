#ifndef PROGRESS_H_G
#define PROGRESS_H_G

extern unsigned int num_of_files(const char *path);
extern void verbose_progressbar(const char *filename, long done, long total);
extern void non_verbose_progressbar(int files_backed, int total_files);

#endif /* PROGRESS_H_G */
