#ifndef USER_H
#define USER_H
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
void usersystem()
{
char hostname[101];
gethostname(hostname, sizeof(hostname));
struct passwd *p = getpwuid(getuid());
printf("\033[1;35m");
printf("%s@%s:", p->pw_name, hostname);
}
#endif