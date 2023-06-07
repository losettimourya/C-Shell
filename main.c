#include "headers.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <pwd.h>
#include <sys/time.h>
#include <termios.h>
#include <ctype.h>
#include "echo.h"
#include "cd.h"
#include "pwdd.h"
#include "usersystem.h"
#include "ls.h"
#include "pinfo.h"
#include "history.h"
#include "discover.h"
#include "foreandback.h"
#include "tokenizer1.h"
#define CLEAR "\033[0;37m"
long taim = 0;
char prevdir[100];
char prevprevdir[100];
char home[100];
char real[1000];
long long inputWait = 0;
/*void commandhandler(char** parsed,int flag)
{
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
    cd(parsed,home,prevdir);
    else if(strcmp(parsed[0],"pwd")==0)
    pwd(parsed);
    else if(strcmp(parsed[0],"echo")==0)
    echo(parsed);
    else if(strcmp(parsed[0],"ls")==0)
    ls(parsed,home);
    else if(strcmp(parsed[0],"pinfo")==0)
    pinfo(parsed,home);
    else if(strcmp(parsed[0],"history")==0)
    print(home);
    else if(strcmp(parsed[0],"discover")==0)
    discover(parsed,home);
    else
    execarg(parsed);
    }
    }
    //printf("Invalid command\n");

}*/
/*void parsespace(char* str,char** parsed,int flag)
{
    int i;
    for(i=0;i<100;i++)
    {
        parsed[i] = strsep(&str," \t");
        if(parsed[i]==NULL)
        break;
        if(!strlen(parsed[i]))
        i--;
        else
        {
        int backin = dup(STDIN_FILENO);
        int backout = dup(STDOUT_FILENO);
        checkpipe(parsed[i],flag,home,prevdir);
        dup2(backin,0);
        close(backin);
        dup2(backout,1);
        close(backout);
        }
    }
}*/
// void processstring(char* str,char** parsed,int flag)
//{
//     //int flag=0;
//     parsespace(str,parsed,flag);
//     //printf("%ld\n", strlen(parsed[0]));
//     /*if(parsed[0] != NULL)
//     {
//     struct timeval begin,end;
//     gettimeofday(&begin,0);
//     commandhandler(parsed,flag);
//     gettimeofday(&end,0);
//     taim += end.tv_sec-begin.tv_sec;
//     }*/
//     return;
// }
void parsecol(char *str, char **parsed, int flag)
{
    int i;
    for (i = 0; i < 100; i++)
    {
        parsed[i] = strsep(&str, ";");
        if (parsed[i] == NULL)
            break;
        if (!strlen(parsed[i]))
            i--;
        else
        {
            char *parsed1[100];
            int backin = dup(STDIN_FILENO);
            int backout = dup(STDOUT_FILENO);
            checkpipe(parsed[i], flag, home, prevdir);
            dup2(backin, 0);
            close(backin);
            dup2(backout, 1);
            close(backout);
            // free(parsed);
            // processstring(parsed[i],parsed1,flag);
        }
    }
}
void die(const char *s)
{
    perror(s);
    exit(1);
}

struct termios orig_termios;

void disableRawMode()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}
void enableRawMode()
{
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
        die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
        die("tcsetattr");
}
void siginthandler(int sig_num)
{
    //signal(SIGINT, siginthandler);
   // printf("%d\n", fpid);
    if (fpid != -1)
    {
        kill(fpid, SIGINT);

        fpid = -1;
        return;
    }
    signal(SIGINT,siginthandler);
    /* else
     {
     signal(SIGINT,siginthandler);
     fflush(stdout);
     }*/
}
void sigtstphandler(int sig_num)
{
      printf("%d\n", fpid);
         if(fpid != -1)
         {
             printf("lmao\n");
            // kill(fpid,SIGTTIN);
            // kill(fpid,SIGTSTP);
            kill(fpid,9);
             printf("lmao\n");
                 /*for(int i=0;i<100;i++)
                 {
                     if(stack[i] == 0)
                     {
                         stack[i] = fpid;
                         char* temp = malloc(1000);
                         if(parsed[1] == NULL)
                         strcpy(temp,parsed[0]);
                         else
                         sprintf(temp,"%s %s",parsed[0],parsed[1]);
                         strcpy(cmds[i],temp);
                         break;
                     }
                 }*/
                 fpid = -1;
                 printf("lmao\n");
                 return;
         }
        else
        {
            printf("lmao\n");
        signal(SIGSTOP,sigtstphandler);
        fflush(stdout);
        }
}
void sigtstohandler(int sig_num)
{
    signal(SIGTSTP,sigtstohandler);
    printf("Yoo\n");
   // kill(fpid,SIGTTIN);
   // kill(fpid,SIGTSTP);
   //kill(fpid,SIGINT);
    usersystem();
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        printf("\033[1;36m");
        if (strcmp(cwd, home) == 0)
            printf("~");
        else if (strlen(cwd) > strlen(home))
            printf("~%s", &cwd[strlen(home)]);
        else
            printf("%s", cwd);
        printf("\033[0;37m");
        printf("$ ");
        return;
   /* int p = getpid();
    if(fpid != -1)
    {
        //kill(fpid,SIGTTIN);
        //kill(fpid,SIGTSTP);
        return;
    }
    //signal(SIGTSTP,sigtstohandler);
    return;*/
}
void nub()
{
    return;
}
int main()
{
    signal(SIGINT, siginthandler);
    signal(SIGTSTP, nub);
    taim = 0;
    fpid = -1;
    char input[1000];
    char ch[1024];
    getcwd(ch, sizeof(ch));
    strcpy(home, ch);
    while (1)
    {
        usersystem();
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        printf("\033[1;36m");
        if (strcmp(cwd, home) == 0)
            printf("~");
        else if (strlen(cwd) > strlen(home))
            printf("~%s", &cwd[strlen(home)]);
        else
            printf("%s", cwd);
        printf("\033[0;37m");
        printf("$ ");
        char c;
        char *buf = malloc(sizeof(char) * 100);
        int flag = 0;
        setbuf(stdout, NULL);
        enableRawMode();
        // printf("Prompt>");
        memset(buf, '\0', 100);
        int pt = 0;
        inputWait = 1;
        while (read(STDIN_FILENO, &c, 1) == 1)
        {
            if (iscntrl(c))
            {
                if (c == 10)
                    break;
                else if (c == 27)
                {
                    char buff[3];
                    buff[2] = 0;
                    if (read(STDIN_FILENO, buff, 2) == 2)
                    { // length of escape code
                        printf("\rarrow key: %s", buff);
                    }
                }
                else if (c == 127)
                { // backspace
                    if (pt > 0)
                    {
                        if (buf[pt - 1] == 9)
                        {
                            for (int i = 0; i < 7; i++)
                            {
                                printf("\b");
                            }
                        }
                        buf[--pt] = '\0';
                        printf("\b \b");
                    }
                }
                else if (c == 9)
                { // TAB character
                    struct dirent **dir;
                    int j;
                    for (int i = 0; i < pt; i++)
                    {
                        if (buf[i] == ' ')
                        {
                            j = i;
                            break;
                        }
                    }
                    char *temp = malloc(1000);
                    strcpy(temp, &buf[j + 1]);
                    char *list[1024];
                    for (int i = 0; i < 1024; i++)
                    {
                        list[i] = (char *)malloc(100);
                    }
                    int no = scandir(".", &dir, 0, alphasort);
                    int k = 0;
                    if (no)
                    {
                        char cwd[1024];
                        getcwd(cwd, 1024);
                        char *path = malloc(1000);
                        for (int i = 0; i < no; i++)
                        {
                            sprintf(path, "%s/%s", cwd, dir[i]->d_name);
                            struct stat info;
                            stat(path, &info);
                            if (strncasecmp(temp, dir[i]->d_name, strlen(temp)) == 0)
                            {
                                strcpy(list[k], dir[i]->d_name);
                                if (S_ISDIR(info.st_mode))
                                    list[k] = strcat(list[k], "/");
                                k++;
                            }
                        }
                    }
                    if (k == 0)
                        continue;
                    else if (k == 1)
                    {
                        j = strlen(temp);
                        int kk = strlen(buf);
                        for (int x = j; x < strlen(list[0]); x++)
                        {
                            buf[pt++] = list[0][x];
                            printf("%c", list[0][x]);
                        }
                        if (buf[pt - 1] != '/')
                        {
                            buf[pt++] = ' ';
                            printf(" ");
                        }
                        // printf("%s\n", buf);
                    }
                    else
                    {
                        printf("\n");
                        for (int i = 0; i < k; i++)
                        {
                            printf("%s\n", list[i]);
                        }
                        usersystem();
                        char cwd[1024];
                        getcwd(cwd, sizeof(cwd));
                        printf("\033[1;36m");
                        if (strcmp(cwd, home) == 0)
                            printf("~");
                        else if (strlen(cwd) > strlen(home))
                            printf("~%s", &cwd[strlen(home)]);
                        else
                            printf("%s", cwd);
                        printf("\033[0;37m");
                        printf("$ ");
                        j = strlen(temp);
                        int kk = strlen(buf);
                        // printf("%d\n", k);
                        while (1)
                        {
                            int flag = 0;
                            // printf("%d\n", k);
                            for (int jj = 0; jj < k; jj++)
                            {
                                // printf("%d\n", j);
                                if (list[jj][j] != list[0][j])
                                {
                                    flag = 1;
                                    break;
                                }
                            }
                            if (flag)
                                break;
                            else
                            {
                                buf[pt++] = list[0][j];
                                j++;
                            }
                        }
                        printf("%s", buf);
                    }
                }
                else if (c == 4)
                {
                    exit(0);
                }
                else
                {
                    printf("%d\n", c);
                }
            }
            else
            {
                buf[pt++] = c;
                printf("%c", c);
            }
        }
        disableRawMode();
        // if(flag)
        // continue;
        printf("\n");
        // printf("\nInput Read: [%s]\n", buf);

        size_t bufsize = 32;
        size_t characters;
        // char* buf = (char*)malloc(1000);
        // char* line = malloc(1000);
        /*char ch;
        line = fgetc(stdin);
        if(line == NULL)
        exit(0);*/
        inputWait = 1;
        if (strlen(buf) > 1)
        {
            strcpy(real, buf);
            history(real, home);
        }
        strtok(buf, "\n");
        char *buff = (char *)malloc(1000);
        if (strlen(buf))
            strcpy(buff, buf);
        char *token = (char *)malloc(1000);
        char *temp = (char *)malloc(1000);
        if (strlen(buf))
        {
            strcpy(temp, buf);
            int flag1 = 0;
            if (temp[strlen(temp) - 1] == '&')
                flag1 = 1;
            token = strtok(buf, "&");
            // printf("%s %s\n", temp, token);
            int flag = 0;
            if (strcmp(token, buff))
            {
                flag = 1;
                while (token != NULL)
                {
                    char *parsed[100];
                    char token1[100];
                    strcpy(token1, token);
                    token = strtok(NULL, "&");
                    // printf("%s %s\n", token, token1);
                    if (token == NULL && flag1 == 0)
                        parsecol(token1, parsed, 0);
                    else
                        parsecol(token1, parsed, 1);
                }
            }
            if (flag == 0)
            {
                char *parsed[100];  
                parsecol(token, parsed, 0);
            }
        }
        inputWait = 1;
        if (taim >= 1)
        {
            printf("The process took %lds\n", taim);
            taim = 0;
        }
        fpid = -1;
    }
}