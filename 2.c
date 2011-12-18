#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<assert.h>

char *path_array[10];
char *argv[10];
int argc;

void fill_path_array()
{
	int fd,i=0;
	char buf[200];
	fd = open("path.txt",O_RDONLY);
	assert(fd>0);
	read(fd,buf,200);
	path_array[i++] = strtok(buf,":");
	while(path_array[i-1]) {
		path_array[i++] = strtok(NULL,":");
	}
}

void fill_argv(char *cmdline_string)
{
	int i = 0;
	argv[i++] = strtok(cmdline_string," ");
	while(argv[i-1]) {
		argv[i++] = strtok(NULL," ");
	}
	argc = i;
	i = 0;
	while(argv[i])
		printf("this is %d\n,%s\n",argv[i++][0],argv[i]);
}	

void read_cmd()
{
	char cmdline[100];
	printf(">>");
	fflush(stdout);
	fgets(cmdline,100,stdin);	
	fill_argv(cmdline);
}

int main()
{
	fill_path_array();
	read_cmd();
}
