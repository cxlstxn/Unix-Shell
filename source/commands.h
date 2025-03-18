// shell.h
#ifndef COMMANDS_H // avoids redefinition errors
#define COMMANDS_H

// Number of history items to store:
#define HISTORY_SIZE 20 

// Alias Functions:
typedef struct ALIAS {   // Structure declaration
  int alias_num; // alias num
  char* original_name; // name of command
  char* new_name; // alias name for above command
} Alias;

char*str_trim(char* str);

void getpath();
void setpath(char* tokenList[]);
void cd(char* tokenList[]);
void externalcommand(char* tokenList[]);

// History Functions:
void add_to_history(char*command);
void print_history();
char* invoke_history(char* userinput);


void add_to_history(char*command);
void print_history();
void saveHistory();
void loadHistory();


#endif
