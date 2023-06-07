#ifndef PWD_H
#define PWD_H
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
void pwd(char** parsed)
{
if(parsed[1] == NULL)
        {
        char buff[100];
        getcwd(buff,FILENAME_MAX);
        printf("%s\n", buff);
        }
        else
        {
            printf("Invalid number of commands\n");
        }
}
#endif