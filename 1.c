#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<assert.h>
char *path_array[10];
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
	i = 0;
	while(path_array[i])
		printf("%s\n",path_array[i++]);
}
	
int main()
{
	fill_path_array();
}
