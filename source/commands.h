// shell.h
#ifndef COMMANDS_H // avoids redefinition errors
#define COMMANDS_H

void getpath();
void setpath(char* tokenList[]);
void cd(char* tokenList[]);
void externalcommand(char* tokenList[]);

#endif
