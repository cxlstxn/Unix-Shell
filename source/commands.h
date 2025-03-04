// shell.h
#ifndef COMMANDS_H // avoids redefinition errors
#define COMMANDS_H

// Number of history items to store:
#define HISTORY_SIZE 20 

char*str_trim(char* str);

void getpath();
void setpath(char* tokenList[]);
void cd(char* tokenList[]);
void externalcommand(char* tokenList[]);

// History Functions:
void add_to_history(char*command);
void print_history();
char* invoke_history(char* userinput);

#endif
