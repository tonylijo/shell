#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<assert.h>
#include<strings.h>
#include<errno.h>
char *path_array[10];
char *new_argv[10];
int new_argc;
int path_count;
char cmdline[100];
char buf[200];
int bg_stack[10];
int bg_flag;
int bg_top=-1;

void fill_path_array()
{
	int fd,i=0;
	char *temp_array;
	fd = open("/bin/path.txt",O_RDONLY);
	assert(fd>0);
	read(fd,buf,200);
	close(fd);
	path_array[i++] = strtok(buf,":\n");
	while(path_array[i-1]) {
		path_array[i++] = strtok(NULL,":\n");
	}
	path_count = (--i);
}

void fill_argv(char *cmdline_string)
{
	int i = 0;
	new_argv[i++] = strtok(cmdline_string," \n&>|");
	while(new_argv[i-1]) {
		new_argv[i++] = strtok(NULL," \n&");
	}
	new_argc = (--i);
	#ifdef __DEBUG
	for(i=0; i < new_argc;i++) {
	  printf("%s \n",new_argv[i]);
	}
	#endif
}	

void read_cmd()
{
	int last_pos;
	printf(">>");
	fflush(stdout);
	bzero(cmdline,100);
	fgets(cmdline,100,stdin);
	last_pos = (strlen(cmdline) - 2);
	#ifdef __DEBUG
	printf("the last_pos contains %c,%c",cmdline[last_pos],cmdline[last_pos-1]);
	#endif
	if(cmdline[last_pos] == '&') {
		#ifdef __DEBUG
		printf("background option set\n");
		#endif
		bg_flag = 1;
		cmdline[last_pos] = '\0';
	} else {
		bg_flag = 0;
	}
	fill_argv(cmdline);
}

int find_path()
{	
	int i,fd;
	char temp_cmd[100];

	if(!strcmp(new_argv[0],"fg")) {
		wait(bg_stack[atoi(new_argv[1])]);
		return -2;
	}

	for(i = 0;i < path_count; i++) {
		bzero(temp_cmd,100);
		#ifdef __DEBUG
		printf("the path is %s\n", path_array[i]);
		#endif
		strcpy(temp_cmd,path_array[i]);
		strcat(temp_cmd,new_argv[0]);
		#ifdef __DEBUG
		printf("temp_cmd: %s,%d\n",temp_cmd,strlen(temp_cmd));
		#endif
		fd = open(temp_cmd,O_RDONLY);
		#ifdef __DEBUG
		printf("%d\n",fd);
		#endif
		if(fd > 0) {
			close(fd);
			return i;
		}
	}

	return -1;
}

int main(int argc,char *argv[],char *envp)
{
	int path_index,i,pid;
	char execfile[100];
	fill_path_array();
	while(1) {
		read_cmd();
		path_index = find_path();
		if(path_index >= 0) {
			bzero(execfile,100);
			#ifdef __DEBUG
			printf("path is:%s\n",path_array[path_index]);
			#endif
			strcpy(execfile,path_array[path_index]);
			strcat(execfile,new_argv[0]);
			#ifdef __DEBUG
			printf("execfile is:%s",execfile);
			#endif
			if((pid = fork()) == 0)  {
				execve(execfile,new_argv,envp);
				perror("execve");
			}
			if(bg_flag) {
				bg_stack[++bg_top] = pid;
			} else {
				wait(pid);
			}
			#ifdef __DEBUG
			printf("The command %s is found in directory %s\n",new_argv[0],path_array[path_index]);
			#endif
		} else if(path_index == -1) {
			printf("no such command found\n");
		}
		
	}
}
