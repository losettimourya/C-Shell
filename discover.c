#ifndef DISC_H
#define DISC_H
#include "headers.h"
/*void recurs(char* currdir,char* start)
{
    struct dirent** d;
    int no = scandir(currdir,&d,0,alphasort);
    printf("%ssuii\n", currdir);
    if(currdir[strlen(currdir)-1]=='.')
    no = 0;
    if(no)
    {
        for(int i=0;i<no;i++)
        {
            char path1[1024];
            if(strcmp(currdir,".")==0)
            continue;
            strcpy(path1,currdir);
            strcat(path1,"/");
            strcat(path1,d[i]->d_name);
            struct stat info;
            stat(path1,&info);
            if(S_ISDIR(info.st_mode) && path1[strlen(path1)-1]=='.')
            recurs(path1,start);
            else 
            printf("%s\n", path1);
        }
    }
    
}*/
void recurs(char* basePath,char* start,int dflag,int fflag,char* reqstr)
{
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);
    if (!dir)
        return;

    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, dp->d_name);
            struct stat info;
            stat(path,&info);
            //printf("%s %s\n", reqstr,path);
            char start1[100];
            strcpy(start1,start);
            if(strlen(start1))
            strcat(start1,"/");
            strcat(start1,dp->d_name);
            if(strstr(start1,reqstr))
            {
            if(S_ISDIR(info.st_mode) && dflag)
            {
            if(strlen(start))
            printf("%s/%s\n", start,dp->d_name);
            else
            printf("%s\n", dp->d_name);
            }
            else if(fflag && !(S_ISDIR(info.st_mode)))
            {
            if(strlen(start))
            printf("%s/%s\n", start,dp->d_name);
            else
            printf("%s\n", dp->d_name);
            }
            }
            recurs(path,start1,dflag,fflag,reqstr);
        }
    }

    closedir(dir);
}
void discover(char** parsed,char home[])
{
    int i=1;
    int dflag=0;
    int fflag=0;
    char currdir[1024];
    char reqstr[100];
    char tilde[100];
    int flag=0;
    strcpy(reqstr,"");
    strcpy(currdir,".");
    while(parsed[i] != NULL)
    {
        if(parsed[i][0]=='-')
        {
            for(int j=1;j<strlen(parsed[i]);j++)
            {
                if(parsed[i][j]=='d')
                dflag=1;
                if(parsed[i][j]=='f')
                fflag=1;
            }
        }
        else if(parsed[i][0] != '\"' && parsed[i][0] != '~')
        {
            strcpy(currdir,parsed[i]);
        }
        else if(parsed[i][0] == '~')
        {
            strcpy(currdir,home);
            strcat(currdir,&parsed[i][1]);
            strcpy(tilde,parsed[i]);
            flag=1;
        }
        else
        {
            strcpy(reqstr,parsed[i]);
        }
        i++;
    }
    char cwd[1024];
    getcwd(cwd,sizeof(cwd));
    char start[1000];
    if(!flag)
    strcpy(start,currdir);
    else
    strcpy(start,tilde);
    if(currdir == NULL)
    {
        strcpy(currdir,cwd);
        strcpy(start,currdir);
    }
    else
    {
        if(currdir[0] != '/')
        {
        strcat(cwd,"/");
        strcat(cwd,currdir);
        strcpy(currdir,cwd);
        }
    }
    //strcat(start,"/");
    //printf("%s\n", currdir);
    //printf("%s\n", reqstr);
    char* token = (char*)malloc(1000);
    if(strlen(reqstr))
    token = strtok(reqstr,"\"");
    //printf("%s\n", token);
    if(fflag==0 && dflag==0)
    {
        fflag=1;
        dflag=1;
    }
    recurs(currdir,start,dflag,fflag,token);
}
#endif