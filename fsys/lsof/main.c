#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <unistd.h>

#include <dirent.h>
#include <fcntl.h> 

#include <string.h>

#define buf_len 128

int main(int argc, char const *argv[])
{
	DIR *dir_proc, *dir_fd;
	struct dirent *entry_proc, *entry_fd;
	struct stat status_buf;

	FILE *fd;

	char way_str[buf_len], *current_dir_end;
	char link_buf[buf_len];
	
	int PID, len;

	if (NULL == (dir_proc = opendir("/proc")))
	{
		printf("can't open /proc\n");
		return 0;
	}

	printf("%6s\t%6s\n", "UID", "FILE");

	chdir("/proc/");

	while (NULL != (entry_proc = readdir(dir_proc)))	//read /proc
	{
		PID = atoi(entry_proc->d_name);					// =0 if it is not a number
		
		if (0 < PID)									//if it is a PID
		{
			strcpy(way_str, entry_proc->d_name);
				
			strcat(way_str, "/fd/");

			current_dir_end = &(way_str[strlen(way_str)]);

			if (NULL == (dir_fd = opendir(way_str)))
			{
				;//printf("no access or error in %d\n", PID);
			}
			else
			{
				while (NULL != (entry_fd = readdir(dir_fd)))	//read proc/PID/fd
					if ('.' != entry_fd->d_name[0])
					{
						strcpy(current_dir_end, entry_fd->d_name);

						lstat(way_str, &status_buf);

						len = readlink(way_str, link_buf, buf_len);

						link_buf[len] = '\0';

						printf("%6d\t%s\n", PID, link_buf);
					}

				printf("\n");

				closedir(dir_fd);
			}

		}
	}

	closedir(dir_proc);

	return 0;
}