#ifndef PIN_H
#define PIN_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <math.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/utsname.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>
#include <grp.h>
#include <pwd.h>
#define MAX_BUF_LEN 1024
char base_dir[100000];
int min(int a,int b)
{
        return (a<b)?a:b;
}
int pinfo(char** parsed,char home[]) 
{
        int c;
        int len=0;
        pid_t cpid;
        char path[1024]; // for generating path of proc/pid file
        char buf[1024];  // for reading file line by line
        char status;
        if(parsed[1] == NULL){
                 cpid = getpid();
        }
        else
        cpid = atoi(parsed[1]);
        printf("pid : %d\n", cpid);
        sprintf(path, "/proc/%d/stat", cpid);
        FILE* fp = fopen(path, "r");
        if(!fp) {
                perror("Error while opening the proc/pid/status file\n"); 
        }
        else {
        fscanf(fp,"%s",buf);
	fscanf(fp,"%s",buf);
	fscanf(fp," %s",buf);
	printf("process status : %s", buf);
        pid_t console_pid = tcgetpgrp(STDOUT_FILENO);
        pid_t my_pid = getpgrp();
        if(console_pid == my_pid)
        printf("+\n");
        else
        printf("\n");
	fclose(fp);
        }
        
        sprintf(path, "/proc/%d/statm", cpid);
        fp = fopen(path, "r");
        if(!fp) 
        {
        perror("Error while opening the proc/pid/statm file\n");

        }
        else 
        {
        while((c=fgetc(fp))!=' ')
        buf[len++] = c;
        buf[len] = '\0';
        printf("memory : %s\n", buf);
        fclose(fp);
        }
        
        sprintf(path, "/proc/%d/exe", cpid);
        len = -1;
        len = readlink(path, buf, sizeof(buf));
        if(len==-1) 
        {
        perror("Error while opening the proc/pid/exe file\n");
        }
        else 
        {
                buf[len] = '\0';
                char* temp = (char*)malloc(1000);
               // temp = showpath(home,buf);
               int lhome = strlen(home);
               int lbuf = strlen(buf);
               int k = min(lhome,lbuf);
               int i;
               for(i=0;i<k;i++)
               {
                       if(home[i] != buf[i])
                       break;
               }
               if(i==lhome)
               {
                       temp[0] = '~';
                       int j;
                       for(j=0;j<(lbuf-lhome);j++)
                       temp[j+1] = buf[j+lhome];
                       temp[j+1] = '\0';
               }
               else
               strcpy(temp,buf);
                printf("executable Path : %s\n", temp);
        }
}
#endif