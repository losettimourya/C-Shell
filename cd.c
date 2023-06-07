#ifndef CD_H
#define CD_H
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<pwd.h>
void cd(char** parsed, char home[],char prevdir[])
{
    char prevprevdir[1024];
    if(parsed[1] == NULL || strcmp(parsed[1],"~")==0 || strcmp(parsed[1],"--")==0)
        {
        chdir(home);
        //strcpy(prevprevdir,prevdir);
        strcpy(prevdir,home);
        }
        else if(strcmp(parsed[1],"-")==0)
        {
            printf("%s\n", prevdir);
            char cwd[1024];
            strcpy(prevprevdir,getcwd(cwd,sizeof(cwd)));
            chdir(prevdir);
            strcpy(prevdir,prevprevdir);
        }
        else if(parsed[1][0]=='~')
        {
            char cwd[1024];
            strcpy(prevdir,getcwd(cwd,sizeof(cwd)));
            char req[1000];
            char temp[100];
            strcpy(temp,home);
            strcpy(req,strcat(temp,&parsed[1][1]));
            chdir(req);
        }
        else
        {
        //printf("%s\n", prevdir);
        struct stat sb;
        if(stat(parsed[1],&sb)==0 && S_ISDIR(sb.st_mode))
        {
        strcpy(prevprevdir,prevdir);
        char cwd[1024];
        strcpy(prevdir,getcwd(cwd,sizeof(cwd)));
        chdir(parsed[1]);
        }
        else
        perror("Directory does not exist");
        }
}
#endif