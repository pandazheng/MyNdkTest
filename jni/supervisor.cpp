#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUFFER_LENGTH 1024
#define FLAG_FILE "/data/local/tmp/tmp.lock"
#define SLEEP_INTERVEL 10   // every x seconds to check if process is running
int isProcessExist(char* busyboxPath, char *processName);
void runProcess(char* packageName, char* serviceName);

int main(int argc, char **argv)
{
 if (argc < 4)
 {
	 fprintf(stderr, "Usage:%s <packagename> <ServiceNameOrActivityName> <busyboxpath>\n", argv[0]);
	 return -1;
 }
 // check if self already running.
 int ret = -1;
 FILE * g_lockfile = NULL;

 //检查是否已经有一个supervisor进程在运行
 g_lockfile = fopen(FLAG_FILE, "a+");
 if (g_lockfile == NULL)
 {
	 fprintf(stderr, "fopen() failed:%s!\n", strerror(errno));
	 return -1;
 }

 ret = flock(fileno(g_lockfile), LOCK_EX |LOCK_NB);
 if (ret != 0)
 {
	 fprintf(stderr, "flock() failed:%s!\n", strerror(errno));
	 printf("this program already running\n");
	 return -1;
 }

 //循环检测android进程是否在运行
 while(1)
 {
	 //判断程序是佛已经被卸载，如果已被卸载，则退出supervisor
	 if (access(argv[3], 0) == -1)
	 {
			 printf("file not exist. file:%s\n", argv[3]);
			 exit(-1);
	 }
	 //判断android进程是否在运行，如果不在，则运行之
	 if (!isProcessExist(argv[3], argv[1]))
	 {
		 runProcess(argv[1], argv[2]);
	 }

	 sleep(SLEEP_INTERVEL);
 }

 return 0;
}

// 使用busybox的pidof命令来检测目标进程是否运行
int isProcessExist(char* busyboxPath, char *processName)
{
 char buf[BUFFER_LENGTH];
 char command[BUFFER_LENGTH];
 FILE *fp;
 int ret = 0;
 sprintf(command, "%s pidof %s", busyboxPath, processName);

 if ((fp = popen(command,"r")) == NULL)
 {
	 printf("popen failed\n");
	 exit(1);
 }

 if ((fgets(buf,BUFFER_LENGTH,fp))!= NULL )
 {
	 ret = 1;
	 printf("pid is:%s\n", buf);
 }

 pclose(fp);
 return ret;
}

//使用am命令来启动android程序
void runProcess(char* packageName, char* serviceName)
{
 FILE *fp;
 char command[BUFFER_LENGTH];
 sprintf(command, "am start -n %s/%s", packageName, serviceName);
 //sprintf(command, "am startservice -n %s/%s", packageName, serviceName);
 printf("run cmd: %s\n", command);
 if ((fp = popen(command,"r")) == NULL)
 {
	 printf("popen failed\n");
	 exit(1);
 }

 pclose(fp);
}
