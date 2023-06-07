#ifndef ECHO_H
#define ECHO_H
#include <stdio.h>
#include <stdlib.h>
void echo(char** parsed)
{
    int i=1;
    while(parsed[i] != NULL)
    {
        printf("%s ",parsed[i]);
        i++;
    }
        printf("\n");
}
#endif
