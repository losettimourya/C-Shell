#ifndef TOK1_H
#define TOK1_H
void commandhandler(char** parsed,int flag,char home[],char prevdir[]);
int tokenize(char* token,char* str, char tokens[100][1000],char home[],char prevdir[]);
void checkpipe(char* parsed, int flag,char home[],char prevdir[]);
void redirect(char* str,int flag,int* piped, int prev,char home[],char prevdir[]);
void redirecttype(char* token, char* file,char home[],char prevdir[]);
#endif