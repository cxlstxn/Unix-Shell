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
// FUCKED:
// setpath function: Sets new PATH:
void setpath(char* tokenList[]) {
    if(tokenList[1] == NULL){
	perror(tokenList[1]);
    } // lead with what caused error
      else if(tokenList[2] != NULL){
	perror(tokenList[0]);
      }
      else{
        setenv("PATH", tokenList[1], 1);
      }
    }

// cd function:
void cd(char* tokenList[]) {
  // ".." automatically works - thanks UNIX
    if(tokenList[1] == NULL){
      // type 1 - no args -> put user in home directory:
      chdir(getenv("HOME"));
      } else if (access(tokenList[1], F_OK) == 0) {
	// type 3 - 'filepath' args -> go to directory:
      chdir(tokenList[1]); // chdir check output
    } else {
      perror("Error: directory does not exist\n");
      }
    }

/*
  else if(strcmp(tokenList[1], "..") == 0){
	// type 2 - '..' args -> go to parent directory:
        chdir("..");
*/

//chuck into array -> loop through array checking -- POTENTIAL HARD

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
	    perror(tokenList[0]);
            return;
        } else {
            wait(NULL); // Parent waiting for child process to complete
        }
    }
