#include <dirent.h>
#include <sys/stat.h>
#include "../../Shared/Helper.h"

static inline int file_to_count(const char *filename)
{
	return (strcmp(filename, ".") &&
		strcmp(filename, ".."));
}

int num_of_files(const char *path)
{
	DIR *dr;
	struct dirent *entry;
	int total = 0;

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

int main(void)
{
	int total = num_of_files(".");

	printf("%d\n", total);
	return 0;
}
