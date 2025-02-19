// shell.h
#ifndef COMMANDS_H // avoids redefinition errors
#define COMMANDS_H
#define HISTORY_SIZE 20 



void getpath();
void setpath(char* tokenList[]);
void cd(char* tokenList[]);
void externalcommand(char* tokenList[]);

void add_to_history(char*command);
void print_history();
char* get_history(char*input);

#endif
