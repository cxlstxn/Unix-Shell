// shell.h
#ifndef COMMANDS_H // avoids redefinition errors
#define COMMANDS_H

typedef struct{
  int comnum;
  char com[1024];
}History;

void getpath();
void setpath(char* tokenList[]);
void cd(char* tokenList[]);
void externalcommand(char* tokenList[]);

#endif
