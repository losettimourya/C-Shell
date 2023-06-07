#ifndef TOK1_H
#define TOK1_H
#include "headers.h"
#include "echo.h"
#include "cd.h"
#include "pwdd.h"
#include "usersystem.h"
#include "ls.h"
#include "pinfo.h"
#include "history.h"
#include "discover.h"
#include "foreandback.h"
void commandhandler(char** parsed,int flag,char home[],char prevdir[])
{
    //printf("%s\n", home);
    checkchild();
    //printf("%s\n", parsed[0]);
    if(parsed[0] != NULL)
    {
    if(flag)
    background(parsed);
    else
    {
    if(strcmp(parsed[0],"exit")==0)
    exit(0);
    else if(strcmp(parsed[0],"cd")==0)
    {
        /*int i=0;
        while(parsed[i] != NULL)
        {
            printf("%s\n", parsed[i]);
            i++;
        }*/
    cd(parsed,home,prevdir);
    }
    else if(strcmp(parsed[0],"pwd")==0)
    pwd(parsed);
    else if(strcmp(parsed[0],"echo")==0)
    echo(parsed);
    else if(strcmp(parsed[0],"ls")==0)
    {
        //printf("hey\n");
    ls(parsed,home);
    }
    else if(strcmp(parsed[0],"pinfo")==0)
    pinfo(parsed,home);
    else if(strcmp(parsed[0],"history")==0)
    print(home);
    else if(strcmp(parsed[0],"discover")==0)
    discover(parsed,home);
    else if(strcmp(parsed[0],"jobs")==0)
    {
    if(parsed[2] == NULL)
    {
    if(parsed[1]==NULL)
    printjobs(parsed,0,0);
    else if(strcmp(parsed[1],"-r")==0)
    printjobs(parsed,1,0);
    else if(strcmp(parsed[1],"-s")==0)
    printjobs(parsed,0,1);
    }
    else
    printf("Invalid Command\n");
    }
    else if(strcmp(parsed[0],"fg")==0)
    printjobs(parsed,0,0);
    else if(strcmp(parsed[0],"bg")==0)
    printjobs(parsed,0,0);
    else if(strcmp(parsed[0],"sig")==0)
    printjobs(parsed,0,0);
    else
    execarg(parsed);
    }
    }
    //printf("Invalid command\n");

}
void parsespace(char* str,char** parsed,int flag)
{
    int i;
    for(i=0;i<100;i++)
    {
        parsed[i] = strsep(&str," \t");
        if(parsed[i]==NULL)
        break;
        if(!strlen(parsed[i]))
        i--;
    }
}
void processstring(char* str,char** parsed,int flag,char home[],char prevdir[])
{
    //int flag=0;
    parsespace(str,parsed,flag);
    //printf("%ld\n", strlen(parsed[0]));
    if(parsed[0] != NULL)
    {
    //struct timeval begin,end;
    //gettimeofday(&begin,0);
    commandhandler(parsed,flag,home,prevdir);
    //gettimeofday(&end,0);
    //taim += end.tv_sec-begin.tv_sec;
    }
    return;
}
int tokenize(char* token,char* str, char tokens[100][1000],char home[],char prevdir[])
{
    int n = strlen(token), m = strlen(str);
    int i,j,k;
    int cnt=0,sum=0;
    int flag=1;
    for(i=0;i<(m-n+1);i++)
    {
        flag=1;
        for(j=0;j<n;j++)
        {
            if(token[j]-str[i+j])
            flag=0;
        }
        if(flag)
        {
            for(j=i;j<(i+n);j++)
            str[j] = '\0';
            strcpy(tokens[cnt++],&str[sum]);
            strcpy(tokens[cnt++],token);
            sum = i+n;
            i += (n-1);
        }
    }
    strcpy(tokens[cnt],&str[sum]);
    cnt++;
    return cnt;
}
void redirecttype(char* token, char* file,char home[],char prevdir[])
{
    if(strcmp(token,">")==0)
    {
        int f = open(file,O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if(f == -1)
        perror("can't perform output redirection");
        else
        {
            close(STDOUT_FILENO);
            dup(f);
            close(f);
        }
    }
    if(strcmp(token,">>")==0)
    {
        int f = open(file,O_WRONLY | O_CREAT | O_APPEND, 0644);
        if(f == -1)
        {
            perror("can't perform output redirection");
            close(STDOUT_FILENO);
            dup(f);
            close(f);
        }
        else
        {
            close(STDOUT_FILENO);
            dup(f);
            close(f);
        }
    }
    if(strcmp(token,"<")==0)
    {
        int f = open(file,O_RDONLY);
        if(f == -1)
        {
            perror("can't perform input redirection");
            close(STDIN_FILENO);
            dup(f);
            close(f);
        }
        else
        {
            close(STDIN_FILENO);
            dup(f);
            close(f);
        }
    }
}
void redirect(char* str,int flag,int* piped, int prev,char home[],char prevdir[])
{
    char* cmds[1000];
    int i,j,k;
    char* input = (char*)malloc(1000);
    strcpy(input,str);
    cmds[0] = strtok(str," \t\n");
    i=0;
    while(cmds[i] != NULL)
    {
        i++;
        cmds[i] = strtok(NULL," \t");
    }
    if(!i)
    return;
    char initial[100][1000];
    char differ[100][1000];
    char final[100][1000];
    j=0;
    int l;
    int x=0;
    for(j=0;j<i;j++)
    {
        char parsed[100][1000];
        k = tokenize(">>",cmds[j],parsed,home,prevdir);
        for(l=0;l<k;l++)
        {
            if(strlen(parsed[l]))
            {
                strcpy(initial[x],parsed[l]);
                x++;
            }
        }
    }
    i=x;
    x=0;
    for(j=0;j<i;j++)
    {
        if(strcmp(initial[j],">>")==0)
        {
            strcpy(differ[x],initial[j]);
            x++;
        }
        else
        {
        char parsed[100][1000];
        k = tokenize(">",initial[j],parsed,home,prevdir);
        for(l=0;l<k;l++)
        {
            if(strlen(parsed[l]))
            {
                strcpy(differ[x],parsed[l]);
                x++;
            }
        }
        }
    }
    i=x;
    x=0;
    for(j=0;j<i;j++)
    {
        char parsed[100][1000];
        k = tokenize("<",differ[j],parsed,home,prevdir);
        for(l=0;l<k;l++)
        {
            if(strlen(parsed[l]))
            {
                strcpy(final[x],parsed[l]);
                x++;
            }
        }
    }
    i=x;
    int num=0;
    char* finaltoks[100];
    for(j=0;j<i;j++)
    {
        if(strcmp(final[j],">")==0 || strcmp(final[j],"<")==0 || strcmp(final[j],">>")==0)
        {
            if(j+1 == i)
            {
                printf("Last token can't be %s", final[j]);
            }
            else if(final[j+1] == NULL)
            {
                printf("Token after %s can't be null\n", final[j]);
            }
            else
            {
                redirecttype(final[j],final[j+1],home,prevdir);
                j++;
            }
        }
        else
        {
            finaltoks[num] = malloc(1000);
            strcpy(finaltoks[num],final[j]);
            num++;
        }
    }
    /*i=0;
    while(finaltoks[i] != NULL)
    {
        printf("%s\n", finaltoks[i]);
        i++;
    }*/
    commandhandler(finaltoks,flag,home,prevdir);
    i=0;
    while(finaltoks[i] != NULL)
    {
        finaltoks[i] = NULL;
        i++;
    }

}
void checkpipe(char* parsed, int flag,char home[],char prevdir[])
{
    int no=0,i,j,k;
    for(i=0;i<strlen(parsed);i++)
    {
        if(parsed[i] == '|')
        {
            no++;
            if(i==0 || i==strlen(parsed)-1)
            {
                printf("Invalid Command\n");
                return;
            }
        }
    }
    if(no==0)
    {
        redirect(parsed,flag,NULL,-1,home,prevdir);
        return;
    }
    char cmds[100][1024];
    i=0;
    char* token = strtok(parsed,"|");
    if(token != NULL)
    strcpy(cmds[0],token);
    while(token != NULL)
    {
        strcpy(cmds[i],token);
        token = strtok(NULL,"|");
        i++;
    }
    int prev=-1;
    int output = dup(1);
    int input = dup(0);
    for(j=0;j<(i-1);j++)
    {
        int piped[2];
        if(prev+1)
        {
            dup2(prev,0);
            close(prev);
        }
        if(pipe(piped)==-1)
        {
            printf("Can't open pipe\n");
            return;
        }
        dup2(piped[1],1); close(piped[1]);
        redirect(cmds[j],flag,piped,prev,home,prevdir);
        prev = piped[0];
    }
    dup2(output,1);
    close(output);
    if(prev+1)
    {
        dup2(prev,0);
        close(prev);
    }
    redirect(cmds[i-1],flag,NULL,prev,home,prevdir);
    dup2(input,0);
    close(input);
}
#endif