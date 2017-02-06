#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "make_log.h"
#include <sys/wait.h>

int main(void)
{
	pid_t fd[2];
	int ret=pipe(fd);
	if(ret==-1)
		{
			perror("pipe");
			exit(1);
		}
	pid_t pid=fork();
	if(pid<0)
		{
			perror("fork");
			exit(1);
		}
	if(pid==0)
		{
			//子进程关闭读端
			close(fd[0]);
			//将标准输出重定向为写端
			dup2(fd[1],STDOUT_FILENO);
			execlp("/usr/bin/fdfs_upload_file","fdfs_upload_file","/etc/fdfs/client.conf.sample","/etc/fdfs/storage.conf",NULL);
			perror("execlp");
			exit(1);
		}
	if(pid>0)
		{
			//父进程关闭写端
			close(fd[1]);
			wait(NULL);
			dup2(fd[0],STDIN_FILENO);
			char buf[1024]={0};
			read(fd[0],buf,1024);
			LOG("test","test","%s",buf);
		}
		return 0;
}
