#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>


void main()
{
	char *buffer = NULL;
	unsigned buffer_size = 0;

	char temp_name[6];
	char name[sizeof("/proc/%u/fd/0123456789") + sizeof(int)*3];
	unsigned printed_count;

	DIR *fd_dir;
	DIR *proc_dir;

	struct dirent *entry;
	struct stat status_buffer;


	chdir("/proc/");
	proc_dir = opendir("/proc/");
	while ((entry = readdir(proc_dir)) != NULL) {
        	lstat(entry->d_name, &status_buffer);
        	strcpy(temp_name, entry->d_name);
		printed_count = sprintf(name, "/proc/%u/fd/",
					atoi(entry->d_name));
		fd_dir = opendir(name);
		if (fd_dir) {
			while ((entry = readdir(fd_dir)) != NULL) {
				if (entry->d_name[0] == '.')
					continue;

				strncpy(name + printed_count, entry->d_name, 10);
				lstat(name, &status_buffer);
				buffer_size = status_buffer.st_size + 1;
				buffer = malloc(buffer_size * sizeof(char));
				readlink(name, buffer, buffer_size);
				printf("%s\t%s\n", temp_name, buffer);
			}
			closedir(fd_dir);
			printf("\n\n");
		}
	}
}