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

  /* DO WHILE SHELL HAS NOT TERMINATED: */

  while (1) {

    char cwd[1024]; // Creates buffer
    // get working directory and save it to cwd
    if((getcwd(cwd, sizeof(cwd))) == NULL){
      perror("Failure getting current working directory\n"); // Failure
      return 1;
    }

    /* DISPLAY PROMPT: */
    printf("%s>$ ", cwd); // command line
    fflush(stdout); // prompt appears immediately

    char userinput[512]; // complain if anything more than 512 chars

    /* READ AND PARSE USER INPUT: */
    fgets(userinput, sizeof(userinput), stdin); // getting user input



    // for printing the command prompt upon empty input
    if(userinput[0] == '\n'){
      continue;
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


    if (feof(stdin)) { // ctrl+d -> exit program
      printf("\n");
      setenv("PATH", originalEnvPath, 1); // reset path to original
      break;
    }

    // exit function:
    else if (strcmp(tokenList[0], "exit") == 0) { //exit -> exit program
      if(tokenList[1] == NULL){
      // tokenList[0] contains the first argument - 'exit'
      setenv("PATH", originalEnvPath, 1); // reset path
      break;
    }else{
	printf("Error: Too many arguments. Usage exit\n");
    }
      
    } // getpath function:
    else if (strcmp(tokenList[0], "getpath") ==  0) { // getpath -> print path
      getpath();
    }
    
    // setpath function: 
    else if (strcmp(tokenList[0], "setpath") == 0){ // setpath -> set path to first args
      setpath(tokenList); // SHOULD ONLY ACCEPT bin:c/.... NO WHITESPACE
    }

    // cd Command:
    else if (strcmp(tokenList[0], "cd") == 0) {
      cd(tokenList);
    }

    // runing external commands 
    else{
      externalcommand(tokenList);
    }

  }
}
