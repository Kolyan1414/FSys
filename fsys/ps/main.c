#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

#define buf_len 128

char buf_str[buf_len];

void print_cmdline(char *way_str)
{
	FILE *fd;

	if (NULL == (fd = fopen(way_str, "r")))
	{
		printf("can't open cmdline file\n");
		exit(0);
	}

	if (NULL != fgets(buf_str, buf_len, fd))
		printf("%s", buf_str);

	printf("\n");

	fclose(fd);
};

void print_stat(char *way_str)
{
	int PID, PPID, PGRP, SESSION, TTY_NR;
	char R;

	FILE *fd;

	if (NULL == (fd = fopen(way_str, "r")))
	{
		printf("can't open stat file\n");
		exit(0);
	}

	if (7 <= fscanf(fd, "%d %s %c %d %d %d %d", 
			&PID, buf_str, &R, &PPID, &PGRP, &SESSION, &TTY_NR))
		printf("%6d\t%6c\t%6d\t", PPID, R, TTY_NR);
	else
		printf("\t\t\t");

	fclose(fd);
};

int main(int argc, char const *argv[])
{
	DIR *dir_proc;
	struct dirent *entry_proc;

	char *proc_str = "/proc", way_str[buf_len], *current_dir_end;
	
	int PID;

	if (NULL == (dir_proc = opendir(proc_str)))
	{
		printf("can't open /proc\n");
		return 0;
	}

	printf("%6s\t%6s\t%6s\t%6s\t%s\n", "PID", "PPID", "R", "TTY", "CMD");

	while (NULL != (entry_proc = readdir(dir_proc)))
	{
		PID = atoi(entry_proc->d_name);					// =0 if it is not a number
		
		if (0 < PID)									//if it is a PID
		{
			strcpy(way_str, proc_str);
			strcat(way_str, "/");						// "/proc" + '/' = "/proc/"
			strcat(way_str, entry_proc->d_name);		// "/proc/" + "PID" = "/proc/PID"

			current_dir_end = &(way_str[strlen(way_str)]);//pointer to '\0'

			printf("%6d\t", PID);
				
			strcpy(current_dir_end, "/stat");
			print_stat(way_str);

			strcpy(current_dir_end, "/cmdline");
			print_cmdline(way_str);
		}
	}

	closedir(dir_proc);
	//user id with lstat() fex
	return 0;
}