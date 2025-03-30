#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


#include "commands.h"

int main() {

  /* SAVE THE CURRENT PATH */
  char* originalEnvPath = getenv("PATH"); // fetches and stores the original path to restore later
  chdir(getenv("HOME")); // changes directory to user's home path - shell running in user's HOME - good starting point

  /* LOAD HISTORY: */

  if (access(".hist_list", F_OK) == 0) { // file exists
    loadHistory();
  }

  /* LOAD ALISASES:  */
  if (access(".aliases", F_OK) == 0) { // file exists
    loadAlias();
  }
  
  /* DO WHILE SHELL HAS NOT TERMINATED: */


  char userinput[512]; // complain if anything more than 512 chars

  while (1) {
    char cwd[1024]; // Creates buffer
    // get working directory and save it to cwd
    if((getcwd(cwd, sizeof(cwd))) == NULL){
      printf("Failure getting current working directory\n"); // Failure
      return 1;
    }

    /* DISPLAY PROMPT: */
    printf("%s>$ ", cwd); // command line
    fflush(stdout); // prompt appears immediately


    /* READ AND PARSE USER INPUT: */
    fgets(userinput, sizeof(userinput), stdin); // getting user input
    str_trim(userinput);


    // for printing the command prompt upon empty input
    if(userinput[0] == '\n'){
      continue;
    }

    // prepare userinput for adding to history:
    char originalinput[512];
    strcpy(originalinput, userinput);

    // search for new line to replace with null
    char *newline = strchr(originalinput, '\n');
    if (newline){
      *newline = '\0'; // replacing newline from fgets() with null
    }


    char* tokenList[100];
    int token_count = 0;
    char* token = strtok(userinput, " < \t | > & ;"); // tokens to look for
    while (token != NULL) {
      char *newline = strchr(token, '\n'); // searching for '\n'
      if (newline) {
	      *newline = '\0'; // setting NULL position
      }
      tokenList[token_count++] = token;
      token = strtok(NULL, " < \t | > & ;");
    }
    tokenList[token_count++] = NULL; //last has to be null for execvp to work


    /* IF COMMAND IS BUILT-IN INVOKE APPROPRIATE FUNCTIONS: */

    // alias function:
    
    // invoking and retokenizing the alias command:


    if(tokenList[0][0] == '!' ){
      char* historyCommand = invoke_history(tokenList[0]);
      if (strcmp(historyCommand, "\n") == 0) {
        continue;
      }
      if (historyCommand != NULL) {
        token_count = 0;
        token = strtok(historyCommand, " < \t | > & ;");
        while (token != NULL) {
          char *newline = strchr(token, '\n');
          if (newline) {
        *newline = '\0';
          }
          tokenList[token_count++] = token;
          token = strtok(NULL, " < \t | > & ;");
        }
        tokenList[token_count++] = NULL;    
      } else {
        printf("Error: Invalid history command\n");
        continue;
      }
      //break;
    }else{
     //Add user input to history:
      char commandString[512] = "";
      for (int i = 0; tokenList[i] != NULL; i++) {
        strcat(commandString, tokenList[i]);
        if (tokenList[i + 1] != NULL) {
          strcat(commandString, " ");
        }
      }
      add_to_history(commandString);
    }

    char* temp = invokeAlias(tokenList);

    if (temp != NULL) {
      if (strcmp(temp, "") == 0) {
        continue;
      }

      char appendedCommand[512];
      strcpy(appendedCommand, temp);

      for (int i = 1; tokenList[i] != NULL; i++) {
        strcat(appendedCommand, " ");
        strcat(appendedCommand, tokenList[i]);
      }
      temp = strdup(appendedCommand); 
      token_count = 0;
      token = strtok(temp, " < \t | > & ;");
      while (token != NULL) {
      char *newline = strchr(token, '\n');
      if (newline) {
        *newline = '\0';
      }
      tokenList[token_count++] = token;
      token = strtok(NULL, " < \t | > & ;");
      }
      tokenList[token_count++] = NULL;
    }
    
    // To ensure we don't hit external commands with '\n'
    if (strcmp(tokenList[0], "\n") == 0) {
      continue;
    }

    if (feof(stdin)) { // ctrl+d -> exit program
      printf("\n");
      setenv("PATH", originalEnvPath, 1); // reset path to original

      /* SAVE HISTORY: */
      saveHistory();

      /* SAVE ALIASES: */
      saveAlias();
      break;
    }

    else if (strcmp(tokenList[0], "wipe") == 0){
      if (tokenList[1] == NULL) {
        clearHistory();
        clearAlias();
      } else if (strcmp(tokenList[1], "history") == 0) {
        clearHistory();
      } else if (strcmp(tokenList[1], "alias") == 0) {
	      clearAlias(); 
      } else {  
        printf("Error: Invalid argument. Usage: clear history OR alias\n");
      }
    }

    else if (strcmp(tokenList[0], "alias") == 0){
      if (tokenList[1] == NULL) {
        printAlias();
      } else {
        createAlias(tokenList);
      }
    }
    
    else if (strcmp(tokenList[0], "unalias") == 0) {
      if (tokenList[1] == NULL) {
        printf("Error: Too few arguments. Usage unalias <name>\n");
      } else if (tokenList[2] != NULL) {
        printf("Error: Too many arguments. Usage unalias <name>\n");
      } else {
        removeAlias(tokenList);
      }
    }

    // exit function:
    else if (strcmp(tokenList[0], "exit") == 0) { //exit -> exit program
      if(tokenList[1] == NULL){
      // tokenList[0] contains the first argument - 'exit'

	/* RESTORE ORIGINAL PATH */
      setenv("PATH", originalEnvPath, 1); // reset path
      saveHistory();
      saveAlias();
      break;
    } else{
	    printf("Error: Too many arguments. Usage exit\n");
    }
      
    } // getpath function:
    else if (strcmp(tokenList[0], "getpath") ==  0) { // getpath -> print path
      getpath(tokenList);
    }
    
    // setpath function: 
    else if (strcmp(tokenList[0], "setpath") == 0){ // setpath -> set path to first args
      setpath(tokenList); // SHOULD ONLY ACCEPT bin:c/.... NO WHITESPACE
    }

    // cd Command:
    else if (strcmp(tokenList[0], "cd") == 0) {
      cd(tokenList);
    }

    // print history function:
    else if (strcmp(tokenList[0], "history") == 0){
      print_history(); 
    }
    // runing external commands 
    else{
      externalcommand(tokenList);
    }
  }
}
