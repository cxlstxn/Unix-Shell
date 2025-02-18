#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


#include "commands.h"
// getpath function: Prints the current PATH:
void getpath() {
    char* path = getenv("PATH");
    printf("%s\n", path);
}

// setpath function: Sets new PATH:
void setpath(char* tokenList[]) {
    if(tokenList[1] == NULL){
	printf("Error: Missing argument. Usage: setpath <directory>\n");
      }
      else if(tokenList[2] != NULL){
	printf("Error: Too many arguments. Usage setpath <directory>\n");
      }
      else{
        setenv("PATH", tokenList[1], 1);
      }
    }

// cd function:
void cd(char* tokenList[]) {
    if(tokenList[1] == NULL){
      // type 1 - no args -> put user in home directory:
      chdir(getenv("HOME"));
      } else if(strcmp(tokenList[1], "..") == 0){
	// type 2 - '..' args -> go to parent directory:
        chdir("..");
      } else if (access(tokenList[1], F_OK) == 0) {
	// type 3 - 'filepath' args -> go to directory:
        chdir(tokenList[1]);
      } else {
        printf("Error: directory does not exist\n");
      }
    }


// external commands:
void externalcommand(char* tokenList[]) {
    /* ELSE EXECUTE COMMAND AS AN EXTERNAL PROCESS: */
        pid_t pid;
        pid = fork();

        if (pid < 0) { // negative pid -> failure
            fprintf(stderr, "Fork failed\n");
            return;
        } else if (pid == 0) { // signifies child process
            execvp(tokenList[0], tokenList); // passes userinput[0] (function) and rest of string as arg
            // tokenList[0] == NULL, or cannot execute program - Failure
            fprintf(stderr, "Command not found!\n");
            return;
        } else {
            wait(NULL); // Parent waiting for child process to complete
        }
    }
