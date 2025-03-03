#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


#include "commands.h"

// TO FIX:
// exit command should exit firt time on exit - not sure if this is working or not 
// delete trailing whitespace on commands "ls   " should be accpeted - tokens?
// deal with reading in >512 char inputs - error deals with this?

// DECLARING GLOBAL HISTORY VARS:

char history_array[HISTORY_SIZE][512]; // store up to 20 commands of size 512 each
int history_count = 0; // number of commands entered
int history_next = 0; //stores next available position in array


// when we reach the end we invoke circular apsect using modulo to overwrite from start

// command is user inputted string 
void add_to_history(char* command){
  /*
  // if begins with !, ignore, don't add to history:
  if(strcmp(command, "!") == 0){
    return;
  }
  */
   
  // stores command into next avavailable position in history_array, leave space for null terminator:
  strncpy(history_array[history_next], command, sizeof(history_array[history_next]) - 1); // copies 511 bytes, 1 for null  

  // manually add null terminator:
  history_array[history_next][sizeof(history_array[history_next]) - 1 ] = '\0'; // manually adding in null terminator 

  history_next = (history_next + 1) % HISTORY_SIZE;  // Circular buffer
  if(history_count < HISTORY_SIZE){
    history_count ++;
  }
}

// print_history: prints the history
void print_history(){
  // no commands in history:
  if(history_count == 0){
    printf("No commands in history!\n");
    return;
  }
  for(int i = 0; i < history_count; i++){
    printf("%d: %s\n", i+1, history_array[i]);
  }
}


// invoke_history: invokes a command from the history
char* invoke_history(char* userinput){
  // userinput[0] guarenteed to be '!'
      if(strcmp(userinput[1],"!") == 0){
	// last command:
	return history_array[(history_next - 1 + HISTORY_SIZE) % HISTORY_SIZE];
      }
      // nth newest branch:
      else if (userinput[1] == "-"){
	if(userinput[2] <= 0 || userinput[2] > history_count){
	  // invalid number
	  printf("Error: Invalid Arugment: Usage !-<number> or !<number>\n");
	} else{
	  // valid number:
	  int i = (history_next - userinput[2] + HISTORY_SIZE) % HISTORY_SIZE;
	  return history_array[i];	  
	}
      }
      // nth oldest branch:
      else if(userinput[1] > 0 && userinput[1] <= history_count){
	// valid
	int index = (history_next - history_count + (userinput[1]+1) + HISTORY_SIZE) % HISTORY_SIZE;
	return history_array[index];
      }else{
	// invalid number:
	printf("Error: Invalid Arugment: Usage !-<number> or !<number>\n");	
      }
	return NULL;
}


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
            exit(1);
        } else {
            wait(NULL); // Parent waiting for child process to complete
        }
}
