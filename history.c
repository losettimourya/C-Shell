#ifndef HIST_H
#define HIST_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char line[25][100];
void history(char real[],char home[])
{
    char* temp = (char*)malloc(1000);
    strcpy(temp,home);
    temp = strcat(temp,"/file.txt");
    FILE* fptr = fopen(temp,"r");
    int i=0;
    while(fgets(line[i],100,fptr))
    {
        line[i][strlen(line[i])-1] = '\0';
        i++;
    }
    fclose(fptr);
    fptr = fopen(temp,"w");
    real = strtok(real,"\n");
    if(strcmp(line[19],real))
    {
    for(int j=1;j<20;j++)
    {
        fprintf(fptr,"%s\n", line[j]);
    }
    fprintf(fptr,"%s\n",real);
    }
    fclose(fptr);
}
void print(char home[])
{
    char* temp = (char*)malloc(1000);
    strcpy(temp,home);
    temp = strcat(temp,"/file.txt");
    FILE* fptr = fopen(temp,"r");
    int i=0;
    while(fgets(line[i],100,fptr))
    {
        line[i][strlen(line[i])-1] = '\0';
        i++;
    }
    for(int j=10;j<20;j++)
    {
        if(strcmp(line[j],"-"))
        printf("%s\n", line[j]);
    }
}
#endif