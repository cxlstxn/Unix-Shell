#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


#include "commands.h"


// Start to History 

char history_Array[HISTORY_SIZE][512]; // store up to 20 commandsof size 512 each
int history_count = 0; // number of commands entered
int history_next = 0; //stores next available position in array

// next holds next available position in array
// when we reach the end we invoke circular apsect using modulo to overwrite from start

void add_to_history(char * command){
  if(command[0] == '!'){
    printf("no commands stored in history");   // dont store history commands- return to prompt / reprint path 
  }
  // store the command
  strncpy(history_Array[history_next], command, sizeof(history_Array[history_next]) -1); // stores command into next avavailable spot in history array , leaves space for null terminator 

  history_Array[history_next][sizeof(history_Array[history_next])] = '\0'; // manually adding in null terminator 

  history_next = (history_next + 1) % HISTORY_SIZE;  
}



// TO FIX:
// exit command should exit firt time on exit - not sure if this is working or not 
// delete trailing whitespace on commands "ls   " should be accpeted - tokens?
// deal with reading in >512 char inputs - error deals with this?


// getpath function: Prints the current PATH:
void getpath(char* tokenList[]) {
  if(tokenList[1] != NULL){
    // whitepsace after the function (tokenList[0]) should be treated as valid - currently isn't
    printf("Error: Too many arguments. Usage getpath\n"); // fires when more than one arg (one arg being getpath)
  }else{
    printf("%s\n", getenv("PATH"));
  }
}


// setpath function: Sets new PATH:
void setpath(char* tokenList[]) {
  if(tokenList[1] == NULL){
      // lead with what caused error:
	printf("Error: Too few arguments. Usage setpath <pathname>\n");
    } 
      else if(tokenList[2] != NULL){
	printf("Error: Too many arguments. Usage setpath <pathname>\n"); // printing success - confused
      }
      else{
        setenv("PATH", tokenList[1], 1);
      }
}


// cd function:
void cd(char* tokenList[]) {
  // ".." automatically works - thanks UNIX!
    if(tokenList[1] == NULL){
      // no args -> HOME
      chdir(getenv("HOME"));
    }else if (tokenList[2] != NULL) {
      // Too many args
	printf("Error: Too many arguments. Usage cd directory\n");
    }else if(chdir(tokenList[1]) != 0){
      // Assignemnt failed
	perror(tokenList[1]);
      }
    }

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
