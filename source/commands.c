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
    int index = (history_next - 1 - i + HISTORY_SIZE) % HISTORY_SIZE;
    printf("%d: %s\n", i+1, history_array[index]);
}
}


// invoke_history: invokes a command from the history
char* invoke_history(char* userinput){

  // if userinput is !! return the last command
  if(strcmp(userinput, "!!") == 0){
    if(history_count == 0){
      printf("Error: No commands in history\n");
      return "\n";
    }
    return history_array[(history_next - 1 + HISTORY_SIZE) % HISTORY_SIZE];
  }

  // if userinput is a history command:
  int command_index = atoi(&userinput[1]); // convert string to int
  if(command_index == 0){
    printf("Error: Invalid history command\n");
    return "\n";
  }

  // if command index is greater than size of history 
  if(command_index > history_count){
    printf("Error: Command not in history\n");
    return "\n";
  }

  // less than zero - get command from end of history
  if(command_index < 0){
    command_index = history_count + command_index + 1;
    if (command_index < 1){
      printf("Error: Command not in history\n");
      return "\n";
    } else {
      return history_array[command_index - 1];
    }
  }
  // else 
  return history_array[command_index - 1];
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

void saveHistory(){
  FILE *fptr;
  chdir(getenv("HOME"));
  fptr = fopen(".hist_list", "w"); 
  if (fptr == NULL){
    printf("Error: Failure saving History!\n");
  }
  fprintf(fptr, "%d\n", history_next); //save history next to first line
  fprintf(fptr, "%d\n", history_count);//save history count to second
  for(int i = 0; i < history_count; i++){
    fprintf(fptr, "%s \n", history_array[i]);
  }
  fclose(fptr);
}

void loadHistory(){
  FILE *fptr;
  char str[512];
  fptr = fopen(".hist_list", "r");
  if (fptr == NULL){
    perror("Could not open history file\n");
  }
  fscanf(fptr, "%d\n", &history_next);//load history next from first line
  fscanf(fptr, "%d\n", &history_count);// load history count from second line
  for (int i = 0; i < history_count; i++) {
    if (fgets(str, sizeof(str), fptr) != NULL) {
      str[strcspn(str, "\n")] = '\0';  // Remove newline
      strncpy(history_array[i], str, sizeof(history_array[i]) - 1);//copy file line to history array
      history_array[i][sizeof(history_array[i]) - 1] = '\0';//manually place null value
    }
  }
  fclose(fptr);
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

char* str_trim(char* str){
  if(str == NULL){
    return str;
  }
  char* start = str;
  while(*start == ' ' || *start == '\n' || *start == '\t'){
    start++; // move pointer in 
  }
  char* end = start + strlen(start)-1;
  while(end > start && (*end == ' ' || *end == '\n' || *end == '\t')){
    *end = '\0'; // null term
    end--; // move pointer back
  }
  // return trimmed string
  return start;
}
