#ifndef FG_H
#define FG_H
#include "headers.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include <signal.h>
int next=1;
void checkchild()
{
	int c;
	int stat;
	stat = c;
	pid_t cpid;
	cpid = waitpid(-1, &stat, WNOHANG);
	if(cpid>0)
	{
	if(WIFEXITED(stat))
	fprintf(stderr,"Process with pid [%d] exited normally\n",cpid);
	else
	fprintf(stderr,"Process with pid [%d] exited abnormally\n",cpid);
	}
}
void execarg(char** parsed)
{
    pid_t pid = fork(); 
    if (pid == -1) 
	{
        printf("Couldn't fork child\n");
        return;
    } 
	else if (pid == 0) 
	{
        if (strlen(parsed[0])==0 || execvp(parsed[0], parsed)<0) 
        printf("No Such Command\n");
		exit(1);
    } 
	else 
	{
		fpid = pid;
		/*for(int i=0;i<100;i++)
		{
			if(stack[i]==0)
			{
				stack[i] = pid;
				cmds[i] = malloc(1000);
				strcpy(cmds[i],parsed[0]);
				break;
			}
		}*/
		int status;
		waitpid(pid,&status,WUNTRACED);
		if(WIFSTOPPED(status))
		{
			printf("Process with pid %d suspended\n", pid);
			for(int i=0;i<100;i++)
		{
			if(stack[i]==0)
			{
				stack[i] = pid;
				cmds[i] = malloc(1000);
				strcpy(cmds[i],parsed[0]);
				if(parsed[1] != NULL)
				sprintf(cmds[i],"%s %s", cmds[i],parsed[1]);
				break;
			}
		}
		}
        return;
    }
}
void background(char** parsed)
{
    //parsed[i-1] = NULL;
    pid_t pid;
	pid = fork();
	int cpid=getpid();
	if(pid == 0)
	{
		cpid=getpid();
		printf("Process with pid [%d] pushed to background\n",cpid);
		if (strlen(parsed[0])==0 || execvp(parsed[0], parsed)<0) 
		{
			printf("Background command not found");
		}
		//exit(EXIT_FAILURE);
	} 
	else if(pid < 0) 
	{
		perror("Command");
	}
	else
	{
		for(int i=0;i<100;i++)
		{
			if(stack[i]==0)
			{
				stack[i] = pid;
				cmds[i] = malloc(1000);
				strcpy(cmds[i],parsed[0]);
				if(parsed[1] != NULL)
				sprintf(cmds[i],"%s %s", cmds[i],parsed[1]);
				break;
			}
		}
	}
}
void foreorback(char** parsed)
{
    int i=0;
    while(parsed[i] != NULL)
    {
        i++;
    }
}
int pidtojob(int pid)
{
    for(int i=0;i<100;i++)
    {
        if(stack[i] == pid)
        return i+1;
    }
    return -1;
}
int jobtopid(int job)
{
    if((job>0 && job<=100) && stack[job])
	return stack[job-1];
	else
	return -1;
}
char* run[100];
int rno=0;
char* stop[100];
int sno=0;
char* all[100];
int jobno=0;
int pidno[100];
static int compare(const void *p1, const void *p2)
{
	return strcmp(* (char * const *) p1, * (char * const *) p2);
}

void printdata(int stackpid,int rflag,int sflag)
{
	int job = pidtojob(stackpid);
	char file[1000];
	sprintf(file,"/proc/%d/stat", stackpid);
	FILE* f = fopen(file,"r");
	if(f == NULL)
	{
		printf("process with pid %d not found\n", stackpid);
		return;
	}
	int k;
	char buff[100];
	char state[100];
	char word[100];
	fscanf(f,"%d", &k);
	fscanf(f,"%s",buff);
	fscanf(f,"%s",state);
	//printf("%s\n", state);
	if(strcmp(state,"R")==0 || strcmp(state,"S")==0)
	{
		//printf("hi\n");
	all[jobno] = malloc(1000);
	run[rno] = malloc(1000);
	sprintf(all[jobno],"Running %s [%d]",cmds[job-1],stackpid);
	pidno[jobno] = stackpid;
	sprintf(run[rno],"Running %s [%d]",cmds[job-1],stackpid);
	rno++;
	}
	else if(strcmp(state,"T")==0)
	{
	all[jobno] = malloc(1000);
	pidno[jobno] = stackpid;
	stop[sno] = malloc(1000);
	sprintf(all[jobno],"Stopped %s [%d]",cmds[job-1],stackpid);
	sprintf(stop[sno],"Stopped %s [%d]",cmds[job-1],stackpid);
	sno++;
	}
	else if(strcmp(state,"Z")==0)
	{
	sprintf(all[jobno],"Zombie %s [%d]",cmds[job-1],stackpid);
	pidno[jobno] = stackpid;
	}
	else
	{
	sprintf(all[jobno],"Unknown %s [%d]",cmds[job-1],stackpid);
	pidno[jobno] = stackpid;
	}
	jobno++;
	qsort(all,jobno,sizeof(char*),compare);
	char* temp = malloc(1000);
	for(int i=0;i<jobno;i++)
	{
		for(int j=0;j<strlen(all[i]);j++)
		{
			if(all[i][j] == '[')
			{
				k = j;
				break;
			}
		}
		char* a = malloc(1000);
		char* b = malloc(1000);
		char* c = malloc(1000);
		strcpy(a,all[i]);
		a = strtok(a," ");
		a = strtok(NULL," ");
		strcpy(b,a);
		a = strtok(NULL," ");
		sprintf(cmds[i],"%s %s", b,a);
		strcpy(temp,&all[i][k+1]);
		temp = strtok(temp,"]");
		//printf("%s\n", temp);
		stack[i] = atoi(temp);
		//printf("%s %s %s %d\n",a,b,all[i],stack[i]);
	}
	fclose(f);
	//printf("[%d] %s %s [%d]\n", jobno, word, cmds[job-1], stackpid);
}
void flaghandler(int rflag,int sflag)
{
	if(rflag && !sflag)
	{
		char all1[rno][100];
		qsort(run,rno,sizeof(char*),compare);
		int y=0;
		for(int i=0;i<rno;i++)
		{
			int flag=0;
		for(int j=0;j<i;j++)
		{
		if(strcmp(run[i],run[j])==0)
		{
			flag=1;
		}
		}
		if(!flag){
		printf("[%d] %s\n", y+1, run[i]);
		strcpy(all1[y],run[i]);
		y++;
		}
		}
		for(int i=0;i<rno;i++)
		{
			if(i<y)
			strcpy(run[i],all1[i]);
			else
			run[i] = NULL;
		}
	}
	else if(sflag && !rflag)
	{
		int y=0;
		char all1[sno][100];
		qsort(stop,sno,sizeof(char*),compare);
		for(int i=0;i<sno;i++)
		{
			int flag=0;
		for(int j=0;j<i;j++)
		{
		if(strcmp(stop[i],stop[j])==0)
		{
			flag=1;
		}
		}
		if(!flag){
		printf("[%d] %s\n", y+1, stop[i]);
		strcpy(all1[y],stop[i]);
		y++;
		}
		}
		for(int i=0;i<sno;i++)
		{
			if(i<y)
			strcpy(stop[i],all1[y]);
			else
			stop[i] = NULL;
		}
	}
	else
	{
		//printf("%d\n", jobno);
		char all1[jobno][100];
		qsort(all,jobno,sizeof(char*),compare);
		int y=0;
		//printf("%d\n", jobno);
		for(int i=0;i<jobno;i++)
		{
			int flag=0;
		for(int j=0;j<i;j++)
		{
		if(strcmp(all[i],all[j])==0)
		{
			flag=1;
		}
		}
		if(!flag){
		printf("[%d] %s\n", y+1, all[i]);
		strcpy(all1[y],all[i]);
		y++;
		}
		}
		for(int i=0;i<jobno;i++)
		{
			if(i<y)
			strcpy(all[i],all1[i]);
			else
			all[i] = NULL;
		}
	}
}
void bgg(char** parsed)
{
	if(parsed[1] == NULL || parsed[2] != NULL)
	{
		printf("Invalid Command\n");
		return;
	}
	int k = atoi(parsed[1]);
	pid_t pid;
	int j=0;
	char state[100];
	for(int i=0;i<100;i++)
	{
		//printf("%d\n", stack[i]);
		if(stack[i])
		{
		char file[1000];
		sprintf(file,"/proc/%d/stat", stack[i]);
		FILE* f = fopen(file,"r");
		if(f != NULL)
		{
			char buff[100];
			char word[100];
			int kk;
			fscanf(f,"%d", &kk);
			fscanf(f,"%s",buff);
			fscanf(f,"%s",state);
			//printf("%s\n", state);
			j++;
			pid = stack[i];
		}
		if(j == k)
		break;
		}
	}
	if(j != k)
	{
		printf("No such process\n");
		return;
	}
	if(strcmp(state,"R")==0 || strcmp(state,"S")==0)
	return;
	printf("%d\n", j);
	printf("%d\n", pid);
	if(pid==0)
	{
		printf("No such process\n");
		return;
	}
	kill(pid,SIGCONT);
}
void fgg(char** parsed)
{
	if(parsed[1] == NULL || parsed[2] != NULL)
	{
		printf("Invalid Command\n");
		return;
	}
	int k = atoi(parsed[1]);
	pid_t pid;
	int j=0;
	for(int i=0;i<100;i++)
	{
		//printf("%d\n", stack[i]);
		if(stack[i])
		{
		//printf("%d\n", stack[i]);
		char file[1000];
		sprintf(file,"/proc/%d/stat", stack[i]);
		FILE* f = fopen(file,"r");
		if(f != NULL)
		{
			j++;
			pid = stack[i];
		}
		if(j == k)
		break;
		}
	}
	if(j != k)
	{
		printf("No such process\n");
		return;
	}
	//printf("%d\n", pid);
	if(pid==0)
	{
		printf("No such process\n");
		return;
	}
	pid_t pid1;
	int stat;
	do{
	pid1 = waitpid(pid,&stat,WUNTRACED);
	} while(!WIFEXITED(stat) && !WIFSIGNALED(stat));
}
void sigg(char** parsed)
{
	if(parsed[1] == NULL || parsed[2] == NULL || parsed[3] != NULL)
	{
		printf("Invalid Command\n");
		return;
	}
	int k = atoi(parsed[1]);
	pid_t pid;
	int j=0;
	for(int i=0;i<100;i++)
	{
		//printf("%d\n", stack[i]);
		if(stack[i])
		{
		char file[1000];
		sprintf(file,"/proc/%d/stat", stack[i]);
		FILE* f = fopen(file,"r");
		if(f != NULL)
		{
			j++;
			pid = stack[i];
		}
		if(j == k)
		break;
		}
	}
	j = atoi(parsed[2]);
	kill(pid,j);
}
void printjobs(char** parsed,int rflag,int sflag)
{
	jobno=0;
	rno=0;
	sno=0;
	//int jobno=1;
    for(int i =0;i<100;i++)
    {
        if(stack[i])
		{
		char file[1000];
		sprintf(file,"/proc/%d/stat", stack[i]);
		FILE* f = fopen(file,"r");
		if(f != NULL)
		{
		fclose(f);
        printdata(stack[i],rflag,sflag);
		//jobno++;
		}
		}
    }
	if(strcmp(parsed[0],"jobs")==0)
	flaghandler(rflag,sflag);
	else if(strcmp(parsed[0],"fg")==0)
	fgg(parsed);
	else if(strcmp(parsed[0],"bg")==0)
	bgg(parsed);
	else
	sigg(parsed);
}
	//printf("hii\n");
#endif
