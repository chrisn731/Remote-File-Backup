#ifndef PROGBAR_H_G
#define PROGBAR_H_G


unsigned int num_of_files(const char *path);
int non_verbose_progressbar(int files_backed, int total_files);
int verbose_progressbar(const char *filename, long done, long total);

#endif /* PROGBAR_H_G */
