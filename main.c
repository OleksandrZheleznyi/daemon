#include <sys/types.h>
#include <sys/stat.h> //umask(3)
#include <stdio.h> //printf(3)
#include <stdlib.h> //exit(3)
#include <fcntl.h>
#include <errno.h>
#include <unistd.h> //fork(3), chdir(3), sysconf(3)
#include <string.h>
//#include <syslog.h>   //syslog(3), openlog(3), closelog(3)
//#include <signal.h>   //signal(3)

/*****************************/
int writeLog(char msg[256]);


int main(void)//int argc, char * argv[]
{
	pid_t pid, sid;

	/* Fork off the parent process */
	pid = fork();
    if (pid < 0)
    {	
    	//printf("Error: Start Daemon failed\n");
    	exit(EXIT_FAILURE);// exit(1); return -1;
    }
    /* If we got a good PID, then we can exit the parent process. */
    else if (pid > 0)
    {
    	//printf("Daemon Start\n");
    	exit(EXIT_SUCCESS);// exit(0);
    }

    /* Change the file mode mask */
    //umask(0);//даем права на работу с фс 

    /* 
    * Open any logs here 
    */

    /* Create a new SID for the child process */
    sid = setsid();//генерируем уникальный индекс процесса
    if (sid < 0)
   	{
    	/* Log any failure here */
        exit(EXIT_FAILURE);// exit(1);
    }

    /* Ensure we are not session leader */
    pid = fork();
   	if (pid < 0)
    {	
    	exit(EXIT_FAILURE);// exit(1); return -1;
    }
    else if (pid > 0)
    {
    	exit(EXIT_SUCCESS);// exit(0);
    }

    /* Change the file mode mask */
    umask(0);//даем права на работу с фс 


    /* Change the current working directory */
    if ((chdir("/")) < 0)//выходим в корень фс
    {
    	/* Log any failure here */
        exit(EXIT_FAILURE);// exit(1);
    }

    /* Close out the standard file descriptors */
    //Для дебаг вресии можно открить и вывод лог в теринал
    //Для релиз надо закрить и открыть вывод в лог файл
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    //writeLog("Daemon Start");
    /* Daemon-specific initialization goes here */
    /* The Big Loop */
    for (;;)
    {
        /* Do some task here ... */


       	sleep(30); /* wait 30 seconds */
    }

	return 0;
}

int writeLog(char msg[256])//функция записи строки в лог
{ 
    FILE * pLog;
    pLog = fopen("/home/sasha/Desktop/Sirin/Solution/daemon.log", "a");
    if (pLog == NULL)
    {
        return 1;
    }
    char str[312];
    bzero(str, 312);
    strcpy(str, msg);
    strcat(str, " ==========================\n");
    strcat(str, "\n");
    fputs(str, pLog);
    fclose(pLog);
    return 0;
}
