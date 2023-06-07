#ifndef LS_H
#define LS_H
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
#include <errno.h>
#define GREEN "\033[1;32m"
#define BLUE "\033[1;34m"
#define WHITE "\033[1;37m"
#define CLEAR "\033[0;37m"
int alphasortnocase(const struct dirent** d1,const struct dirent** d2)
{
    return(strcasecmp((*(const struct dirent**)d1)->d_name,(*(const struct dirent**)d2)->d_name));
}
int isDirectory(const char *path) {
   struct stat info1;
   if (stat(path, &info1) != 0)
       return 0;
   return S_ISDIR(info1.st_mode);
}
void ls(char** parsed,char home[])
{
    int dirno=0;
    int i=1;
    int lflag=0,aflag=0;
    while (parsed[i] != NULL)
    {
        if(parsed[i][0] == '-')
        {
            for(int j=1;j<strlen(parsed[i]);j++)
            {
                if(parsed[i][j]=='l')
                lflag=1;
                if(parsed[i][j]=='a')
                aflag=1;
            }
        }
        else
        dirno++;
        i++;
    }
    if(dirno==1)
    {
        i=1;
        char cwd[1024];
        getcwd(cwd,1024);
        while(parsed[i] != NULL)
        {
            char* path = (char*)malloc(1000);
            strcpy(path,cwd);
            strcat(path,"/");
            if(parsed[i][0] != '-')
            {
               //struct dirent *dir;
            if(strcmp(parsed[i],"~")==0)
            strcpy(parsed[i],home);
            else if(parsed[i][0] != '/')
            strcat(path,parsed[i]);
            if(parsed[i][0] == '/')
            strcpy(path,parsed[i]);
            struct dirent **dir;
            int no = 0;
            struct stat info;
            stat(path,&info);
            if(!S_ISDIR(info.st_mode) && access(path,F_OK)==0)
            {
                int flag=0;
            if(info.st_mode & S_IXUSR)
            { 
            flag=1;
            }
            if(lflag)
            {
            if(S_ISDIR(info.st_mode)) printf("d");
			else printf("-");
			
			if(info.st_mode & S_IRUSR) printf("r");
			else printf("-");

			if(info.st_mode & S_IWUSR) printf("w");
			else printf("-");

			if(info.st_mode & S_IXUSR)
            { printf("x");
            flag=1;
            }
			else printf("-");

			if(info.st_mode & S_IRGRP) printf("r");
			else printf("-");

			if(info.st_mode & S_IWGRP) printf("w");
			else printf("-");

			if(info.st_mode & S_IXGRP) 
            printf("x");
			else
            printf("-");

			if(info.st_mode & S_IROTH) 
            printf("r");
			else
            printf("-");

			if(info.st_mode & S_IWOTH) 
            printf("w");
			else 
            printf("-");

			if(info.st_mode & S_IXOTH) 
            printf("x");
			else
            printf("-");
			printf("%ld %s %s %ld ",info.st_nlink, (getpwuid(info.st_uid))->pw_name, (getgrgid(info.st_gid))->gr_name, info.st_size);
            char last_mod_time[100];
            strftime(last_mod_time,14,"%h %d %H:%M",localtime(&info.st_mtime));
            printf("%s ",last_mod_time);
            }
            if(!flag)
            printf(GREEN);
            else
            printf(WHITE);
            printf("%s\n", parsed[i]);
            printf(CLEAR);
            no=-1;
            }
            else
            no = scandir(path,&dir,0,alphasortnocase);
            //printf("%d %s\n", no, path);
           // printf("%s\n", parsed[1]);
            if(!no)
            {
                if(access(parsed[i],F_OK)==0)
                printf("%s\n", parsed[i]);
                else
                {
                if(errno == 2)
                printf("Directory doesn't exist\n");
                else
                {
                    printf("Unable to read directory");
                }
                }
            }
            else if(no > 0)
            {
                strcat(path,"/");
                for(int j=0;j<no;j++)
                {
                    if(aflag || dir[j]->d_name[0] != '.')
                    {
                        char path1[1000];
                    char filename[255];
                    strcpy(filename,dir[j]->d_name);
                    strcpy(path1,path);
                    strcat(path1,filename);
                    strcpy(filename,path1);
                    int flag=0;
                    if(lflag)
                    {
                        struct stat info;
                        if(stat(filename,&info)==0)
		                {
			            //st_mode gives us the file mode
			            if(S_ISDIR(info.st_mode)){
                             printf("d");
                             flag--;
                        }
			            else printf("-");

			            if(info.st_mode & S_IRUSR) printf("r");
			            else printf("-");

			            if(info.st_mode & S_IWUSR) printf("w");
			            else printf("-");

			            if(info.st_mode & S_IXUSR) 
                        {
                        printf("x");
                        flag+=2;
                        }
			            else printf("-");

			            if(info.st_mode & S_IRGRP) printf("r");
			            else printf("-");

			            if(info.st_mode & S_IWGRP) printf("w");
			            else printf("-");

			            if(info.st_mode & S_IXGRP) printf("x");
			            else printf("-");

			            if(info.st_mode & S_IROTH) printf("r");
			            else printf("-");

			            if(info.st_mode & S_IWOTH) printf("w");
			            else printf("-");

			            if(info.st_mode & S_IXOTH) printf("x");
			            else printf("-");
                        printf("%ld %s %s %ld ",info.st_nlink, (getpwuid(info.st_uid))->pw_name, (getgrgid(info.st_gid))->gr_name, info.st_size);
                        char time[100];
                        strftime(time,14,"%h %d %H:%M",localtime(&info.st_mtime));
                        printf("%s ",time);
                        }
                    }
                    //printf("%s\n", filename);
                    struct stat info;
                    if(flag==2)
                    {
                    printf(WHITE);
                    printf("%s ", dir[j]->d_name);
                    printf(CLEAR);
                    }
                    else if(isDirectory(filename))
                    {
                    printf(BLUE);
                    printf("%s ", dir[j]->d_name);
                    printf(CLEAR);
                    }
                    else if(access(filename,F_OK)==0)
                    {
                    printf(GREEN);
                    printf("%s ", dir[j]->d_name); 
                    printf(CLEAR);
                    }
                    printf("\n");
                    }
                }
                
            }
            }
            i++;
        }
    }
    else if(dirno == 0)
    {
            struct dirent **dir;
            int no = scandir(".",&dir,0,alphasortnocase);
            for(i=0;i<no;i++)
            {
            if(aflag || dir[i]->d_name[0] != '.')
            {
            char filename[255];
            strcpy(filename,dir[i]->d_name);
            int flag=0;
            if(lflag)
                    {
                        struct stat info;
                        if(stat(filename,&info)==0)
		                {
			            //st_mode gives us the file mode
			            if(S_ISDIR(info.st_mode)){
                             printf("d");
                             flag--;
                        }
			            else printf("-");

			            if(info.st_mode & S_IRUSR) printf("r");
			            else printf("-");

			            if(info.st_mode & S_IWUSR) printf("w");
			            else printf("-");

			            if(info.st_mode & S_IXUSR){
                             printf("x");
                             flag += 2;
                        }
			            else printf("-");

			            if(info.st_mode & S_IRGRP) printf("r");
			            else printf("-");

			            if(info.st_mode & S_IWGRP) printf("w");
			            else printf("-");

			            if(info.st_mode & S_IXGRP) printf("x");
			            else printf("-");

			            if(info.st_mode & S_IROTH) printf("r");
			            else printf("-");

			            if(info.st_mode & S_IWOTH) printf("w");
			            else printf("-");

			            if(info.st_mode & S_IXOTH) printf("x");
			            else printf("-");
                        printf("%ld %s %s %ld ",info.st_nlink, (getpwuid(info.st_uid))->pw_name, (getgrgid(info.st_gid))->gr_name, info.st_size);
                        char time[100];
                        strftime(time,14,"%h %d %H:%M",localtime(&info.st_mtime)); // Time of last modification
                        printf("%s ",time);
                        }
                    }
                flag=0;
            struct stat sb;
            stat(filename,&sb);
            if(!S_ISDIR(sb.st_mode) && (sb.st_mode & S_IXUSR))
            flag=2;
            if(flag==2)
            {
            printf(WHITE);
            printf("%s ", dir[i]->d_name);
            printf(CLEAR);
            }
            else if(isDirectory(filename))
            {
            printf(BLUE);
            printf("%s ", dir[i]->d_name);
            printf(CLEAR);
            }
            else if(access(filename,F_OK)==0)
            {
            printf(GREEN);
            printf("%s ", dir[i]->d_name); 
            printf(CLEAR);
            }
            printf("\n");
            }
            }
            //if(!lflag)
            //printf("\n"); 
    }
    else{
        i=1;
        char cwd[1024];
        getcwd(cwd,1024);
        while(parsed[i] != NULL)
        {
            char* path = (char*)malloc(1000);
            strcpy(path,cwd);
            strcat(path,"/");
            if(parsed[i][0] != '-')
            {
               //struct dirent *dir;
            if(strcmp(parsed[i],"~")==0)
            strcpy(parsed[i],home);
            else if(parsed[i][0] != '/')
            strcat(path,parsed[i]);
            if(parsed[i][0] == '/')
            strcpy(path,parsed[i]);
            struct dirent **dir;
            int no = 0;
            //printf("%s\n", path);
            printf("%s:\n", parsed[i]);
            struct stat info;
            stat(path,&info);
            if(!S_ISDIR(info.st_mode) && access(path,F_OK)==0)
            {
                int flag=0;
                if(info.st_mode & S_IXUSR)
                flag=1;
            if(lflag)
            {
            if(S_ISDIR(info.st_mode)) printf("d");
			else printf("-");
			
			if(info.st_mode & S_IRUSR) printf("r");
			else printf("-");

			if(info.st_mode & S_IWUSR) printf("w");
			else printf("-");

			if(info.st_mode & S_IXUSR)
            { printf("x");
            flag=1;
            }
			else printf("-");

			if(info.st_mode & S_IRGRP) printf("r");
			else printf("-");

			if(info.st_mode & S_IWGRP) printf("w");
			else printf("-");

			if(info.st_mode & S_IXGRP) printf("x");
			else printf("-");

			if(info.st_mode & S_IROTH) printf("r");
			else printf("-");

			if(info.st_mode & S_IWOTH) printf("w");
			else printf("-");

			if(info.st_mode & S_IXOTH) printf("x");
			else printf("-");
            printf("%ld %s %s %ld ",info.st_nlink, (getpwuid(info.st_uid))->pw_name, (getgrgid(info.st_gid))->gr_name, info.st_size);
            char time[100];
            strftime(time,14,"%h %d %H:%M",localtime(&info.st_mtime)); // Time of last modification
            printf("%s ",time);
            }
            if(!flag)
            printf(GREEN);
            else
            printf(WHITE);
            printf("%s\n", parsed[i]);
            printf(CLEAR);
            no=-1;
            }
            else
            no = scandir(path,&dir,0,alphasortnocase);
            if(!no)
            {
                if(access(parsed[i],F_OK)==0)
                printf("%s\n", parsed[i]);
                else
                {
                if(errno == 2)
                printf("Directory doesn't exist\n");
                else
                {
                    printf("Unable to read directory");
                }
                }
            }
            else
            {
                strcat(path,"/");
                for(int j=0;j<no;j++)
                {
                    if(aflag || dir[j]->d_name[0] != '.')
                    {
                        char path1[1000];
                    char filename[255];
                    strcpy(filename,dir[j]->d_name);
                    strcpy(path1,path);
                    strcat(path1,filename);
                    strcpy(filename,path1);
                    int flag=0;
                    struct stat info;
                    if(stat(filename,&info)==0)
                    {
                        if(info.st_mode & S_IXUSR){
                        flag += 2;
                        }
                        if(S_ISDIR(info.st_mode))
                        flag--;
                    }
            if(lflag)
            {
            struct stat info;
            if(stat(filename,&info)==0)
		    {
			//st_mode gives us the file mode
			if(S_ISDIR(info.st_mode))
            {
            printf("d");
            //flag--;
            }
			else printf("-");
			
			if(info.st_mode & S_IRUSR) printf("r");
			else printf("-");

			if(info.st_mode & S_IWUSR) printf("w");
			else printf("-");

			if(info.st_mode & S_IXUSR){
             printf("x");
             //flag += 2;
            }
			else printf("-");

			if(info.st_mode & S_IRGRP) printf("r");
			else printf("-");

			if(info.st_mode & S_IWGRP) printf("w");
			else printf("-");

			if(info.st_mode & S_IXGRP) printf("x");
			else printf("-");

			if(info.st_mode & S_IROTH) printf("r");
			else printf("-");

			if(info.st_mode & S_IWOTH) printf("w");
			else printf("-");

			if(info.st_mode & S_IXOTH) printf("x");
			else printf("-");

			printf("%2ld ",info.st_nlink); // Number of hard links
			printf("%s ",(getpwuid(info.st_uid))->pw_name); // owner name
            printf("%s ",(getgrgid(info.st_gid))->gr_name); // group name
                
            printf("%10ld ",info.st_size); // Total size, in bytes
            char last_mod_time[100];
            strftime(last_mod_time,14,"%h %d %H:%M",localtime(&info.st_mtime)); // Time of last modification
            printf("%s ",last_mod_time);
                    }
                    }
                    //printf("%s\n", filename);
                    if(flag == 2)
                    {
                    printf(WHITE);
                    printf("%s ", dir[j]->d_name);
                    printf(CLEAR);
                    }
                    else if(isDirectory(filename))
                    {
                    printf(BLUE);
                    printf("%s ", dir[j]->d_name);
                    printf(CLEAR);
                    }
                    else if(access(filename,F_OK)==0)
                    {
                    printf(GREEN);
                    printf("%s ", dir[j]->d_name); 
                    printf(CLEAR);
                    }
                    printf("\n");
                    }
                }
                
            }
            }
            i++;
        }
    }
}
    
#endif